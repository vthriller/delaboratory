/*
    delaboratory - color correction utility
    Copyright (C) 2011 Jacek Poplawski

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "action_layer.h"
#include "layer_stack.h"
#include "view_manager.h"
#include "channel_manager.h"
#include "apply_luminance_color.h"
#include "blend_frame.h"
#include "blur.h"
#include "copy_channel.h"
#include "process_linear.h"
#include "str.h"
#include "xml.h"

void blend(deValue* sourcePixels, deValue* overlayPixels, deValue* resultPixels, deValue* maskPixels, deBlendMode blendMode, deValue opacity, int channelSize)
{
    int j;
    if (maskPixels)
    {
        for (j = 0; j < channelSize; j++)
        {
            deValue src = sourcePixels[j];
            deValue ov = overlayPixels[j];
            deValue dst = calcBlendResult(src, ov, blendMode);
            deValue m = maskPixels[j] * opacity;
            deValue result = (1 - m) * src + m * dst;
            resultPixels[j] = result;
        }        
    }
    else
    {
        for (j = 0; j < channelSize; j++)
        {
            deValue src = sourcePixels[j];
            deValue ov = overlayPixels[j];
            deValue dst = calcBlendResult(src, ov, blendMode);
            deValue result = (1 - opacity) * src + opacity * dst;
            resultPixels[j] = result;
        }        
    }
}

deActionLayer::deActionLayer(const std::string& _name, deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deLayer(_name, _colorSpace, _index, _sourceLayer),
 layerStack(_layerStack),
 channelManager(_channelManager),
 viewManager(_viewManager),
 imageActionPass(_colorSpace, _channelManager), 
 imageApplyPass(_colorSpace, _channelManager), 
 imageBlendPass(_colorSpace, _channelManager)
{
    enabled = true;
    blendMode = deBlendNormal;
    applyMode = deApplyLuminanceAndColor;
    opacity = 1.0;
    blendMask = false;
    blendMaskShow = false;
    blendMaskLayer = 0;
    blendMaskChannel = 0;
    blendBlurRadius = 0;
    blendMaskMin = 0;
    blendMaskMax = 1;
    allocatedBlendMaskChannel = -1;

    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        channels.insert(i);
    }
}

deActionLayer::~deActionLayer()
{
    disableBlendMaskChannel();
}

void deActionLayer::enableBlendMaskChannel()
{
    if (allocatedBlendMaskChannel < 0)
    {
        allocatedBlendMaskChannel = channelManager.allocateNewChannel();
    }        
}

void deActionLayer::disableBlendMaskChannel()
{
    if (allocatedBlendMaskChannel >= 0)
    {
        channelManager.freeChannel(allocatedBlendMaskChannel);
        allocatedBlendMaskChannel = -1;
    }
}

void deActionLayer::setBlendBlurRadius(deValue r)
{
    blendBlurRadius = r;
    onBlendSet();
}

const deImage& deActionLayer::getSourceImage() const
{
    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();
    return sourceImage;
}

void deActionLayer::repaint()
{
    viewManager.repaint();
}

bool deActionLayer::isEnabled() const
{
    return enabled;
}

void deActionLayer::setEnabled(bool e)
{
    enabled = e;
    updateImage();
    updateOtherLayers();
}

void deActionLayer::updateOtherLayers()
{
    layerStack.updateImages(index + 1, viewManager.getView());
}

deChannel* deActionLayer::getSourceChannel(int index)
{
    return channelManager.getChannel(getSourceImage().getChannelIndex(index));
}

deSize deActionLayer::getChannelSize() const
{
    return channelManager.getChannelSize();
}

const deImage& deActionLayer::getImage() const
{
    if (!enabled)
    {
        deLayer* source = layerStack.getLayer(sourceLayer);
        const deImage& sourceImage = source->getImage();
        return sourceImage;
    }

    if (applyMode != deApplyLuminanceAndColor)
    {
        return imageApplyPass;
    }

    if (isBlendingEnabled())
    {
        return imageBlendPass;
    }

    return imageActionPass;
}

deValue deActionLayer::getOpacity()
{
    return opacity;
}

void deActionLayer::updateApply()
{
    if (!enabled)
    {
        return;
    }

    if (applyMode == deApplyLuminanceAndColor)
    {
        imageApplyPass.disableAllChannels();
        return;
    }

    imageApplyPass.enableAllChannels();

    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();

    int channelSize = channelManager.getChannelSize().getN();

    deChannel* sc1 = channelManager.getChannel(sourceImage.getChannelIndex(0));
    if (!sc1)
    {
        return;
    }
    deChannel* sc2 = channelManager.getChannel(sourceImage.getChannelIndex(1));
    if (!sc2)
    {
        return;
    }
    deChannel* sc3 = channelManager.getChannel(sourceImage.getChannelIndex(2));
    if (!sc3)
    {
        return;
    }

    deValue* r1 = sc1->getPixels();
    deValue* g1 = sc2->getPixels();
    deValue* b1 = sc3->getPixels();

    deValue* r2;
    deValue* g2;
    deValue* b2;
     
    if (isBlendingEnabled())
    {
        r2 = channelManager.getChannel(imageBlendPass.getChannelIndex(0))->getPixels();
        g2 = channelManager.getChannel(imageBlendPass.getChannelIndex(1))->getPixels();
        b2 = channelManager.getChannel(imageBlendPass.getChannelIndex(2))->getPixels();
    }
    else
    {
        r2 = channelManager.getChannel(imageActionPass.getChannelIndex(0))->getPixels();
        g2 = channelManager.getChannel(imageActionPass.getChannelIndex(1))->getPixels();
        b2 = channelManager.getChannel(imageActionPass.getChannelIndex(2))->getPixels();
    }        

    deValue* r = channelManager.getChannel(imageApplyPass.getChannelIndex(0))->getPixels();
    deValue* g = channelManager.getChannel(imageApplyPass.getChannelIndex(1))->getPixels();
    deValue* b = channelManager.getChannel(imageApplyPass.getChannelIndex(2))->getPixels();

    int i;

    if (applyMode == deApplyLuminance)
    {
        for (i = 0; i < channelSize; i++)
        {
            applyLuminance(r1[i], g1[i], b1[i], r2[i], g2[i], b2[i], r[i], g[i], b[i]);
        }
    }        
    if (applyMode == deApplyColor)
    {
        for (i = 0; i < channelSize; i++)
        {
            applyColor(r1[i], g1[i], b1[i], r2[i], g2[i], b2[i], r[i], g[i], b[i]);
        }
    }        
}

void deActionLayer::setApplyMode(deApplyMode mode)
{
    applyMode = mode;
    updateApply();
    updateOtherLayers();
    repaint();
}


void deActionLayer::createBlendFrame(wxWindow* parent)
{
    if (!blendFrame)
    {
        blendFrame = new deBlendFrame(parent, *this);
        blendFrame->Show(true);
    }        
}

void deActionLayer::updateBlend(int i)
{
    if (!enabled)
    {
        return;
    }

    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();

    int channelSize = channelManager.getChannelSize().getN();

    int s = sourceImage.getChannelIndex(i);

    if (!isBlendingEnabled())
    {
        return;
    }

    if (isChannelNeutral(i))
    {
        if (blendMode == deBlendNormal)
        {
            imageBlendPass.disableChannel(i, s);
            return;
        }
    }

    if (!isChannelEnabled(i))
    {
        imageBlendPass.disableChannel(i, s);
        return;
    }

    deChannel* sourceChannel = channelManager.getChannel(s);
    if (!sourceChannel)
    {
        return;
    }

    int c = imageActionPass.getChannelIndex(i);
    deChannel* channel = channelManager.getChannel(c);
    if (!channel)
    {
        return;
    }

    imageBlendPass.enableChannel(i);
    int b = imageBlendPass.getChannelIndex(i);
    deChannel* blendChannel = channelManager.getChannel(b);
    if (!blendChannel)
    {
        return;
    }

    deValue* maskPixels = NULL;

    if ((blendMask) || (blendMaskShow))
    {
        assert(allocatedBlendMaskChannel>=0);
        if (blendMask)
        {
            deChannel* allocatedMaskChannel = channelManager.getChannel(allocatedBlendMaskChannel);
            maskPixels = allocatedMaskChannel->getPixels();
        }            
    }
    else
    {
        disableBlendMaskChannel();
    }        

    deValue* sourcePixels = sourceChannel->getPixels();
    deValue* overlayPixels = channel->getPixels();
    deValue* resultPixels = blendChannel->getPixels();

    blend(sourcePixels, overlayPixels, resultPixels, maskPixels, blendMode, opacity, channelSize);

}

void deActionLayer::updateOnlyBlend()
{
    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();

    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        updateBlend(i);
    }
}

void deActionLayer::setBlendMode(deBlendMode mode)
{
    blendMode = mode;
    onBlendSet();
}

bool deActionLayer::isBlendingEnabled() const
{
    if (opacity < 1.0)
    {
        return true;
    }

    if (blendMode != deBlendNormal)
    {
        return true;
    }

    if (blendMask)
    {
        return true;
    }

    return false;
}

void deActionLayer::onBlendSet()
{
    if ((blendMask) || (blendMaskShow))
    {   
        renderBlendMask();
    }
    else
    {
    }

    updateOnlyBlend();
    updateApply();
    updateOtherLayers();
    repaint();
}

void deActionLayer::setOpacity(deValue _opacity)
{
    opacity = _opacity;
    onBlendSet();
}


void deActionLayer::updateImage()
{
    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();

    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        updateChannel(i);
    }

    if ((blendMask) || (blendMaskShow))
    {
        renderBlendMask();
    }

    updateOnlyBlend();
    updateApply();
}

void deActionLayer::onChannelChange(int i)
{
    updateChannel(i);
    updateBlend(i);
    updateApply();
}

void deActionLayer::updateChannel(int i)
{
    if (!enabled)
    {
        return;
    }

    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();

    int channelSize = channelManager.getChannelSize().getN();

    int s = sourceImage.getChannelIndex(i);

    if (simpleActionProcessing())
    {
        processAction(i);
        return;
    }

    if ((isChannelNeutral(i)) || (!isChannelEnabled(i)))
    {
        imageActionPass.disableChannel(i, s);
    }
    else
    {

        if (singleChannelProcessing())
        {
            deChannel* sourceChannel = channelManager.getChannel(s);
            if (sourceChannel)
            {
                imageActionPass.enableChannel(i);
                int c = imageActionPass.getChannelIndex(i);
                deChannel* channel = channelManager.getChannel(c);

                if (channel)
                {
                    processAction(i, *sourceChannel, *channel, channelManager.getChannelSize());
                }
            }
        }
        else
        {
            int s1 = sourceImage.getChannelIndex(0);
            int s2 = sourceImage.getChannelIndex(1);
            int s3 = sourceImage.getChannelIndex(2);
            int s4 = sourceImage.getChannelIndex(3);

            deChannel* sc1 = channelManager.getChannel(s1);
            deChannel* sc2 = channelManager.getChannel(s2);
            deChannel* sc3 = channelManager.getChannel(s3);
            deChannel* sc4 = channelManager.getChannel(s4);

            imageActionPass.enableChannel(i);
            int c = imageActionPass.getChannelIndex(i);
            deChannel* channel = channelManager.getChannel(c);

            if (channel)
            {
                processAction4(i, sc1, sc2, sc3, sc4, *channel, channelSize);
            }
        }
    }

}

void deActionLayer::updateChannelUsage(std::map<int, int>& channelUsage) const
{
    if (!enabled)
    {
        return;
    }

    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();
    sourceImage.updateChannelUsage(channelUsage, index);

    imageActionPass.updateChannelUsage(channelUsage, index);

    if (isBlendingEnabled())
    {
        imageBlendPass.updateChannelUsage(channelUsage, index);
    }

    if (applyMode != deApplyLuminanceAndColor)
    {
        imageApplyPass.updateChannelUsage(channelUsage, index);
    }

    if (blendMask)
    {
        deLayer* l = layerStack.getLayer(blendMaskLayer);
        const deImage& im = l->getImage();
        im.updateChannelUsage(channelUsage, index);
    }
}

void deActionLayer::setBlendMask(int l, int c)
{
    blendMaskLayer = l;
    blendMaskChannel = c;
    blendMask = true;
}

void deActionLayer::disableBlendMask()
{
    blendMask = false;
}

deColorSpace deActionLayer::getBlendMaskLayerColorSpace() const
{
    deLayer* maskLayer = layerStack.getLayer(blendMaskLayer);
    const deImage& maskImage = maskLayer->getImage();
    return maskImage.getColorSpace();
}

void deActionLayer::renderBlendMask()
{
    enableBlendMaskChannel();

    deLayer* maskLayer = layerStack.getLayer(blendMaskLayer);
    const deImage& maskImage = maskLayer->getImage();
    int m = maskImage.getChannelIndex(blendMaskChannel);
    deChannel* maskChannel = channelManager.getChannel(m);
    if (!maskChannel)
    {
        return;
    }
    deValue* maskPixels = maskChannel->getPixels();

    deChannel* allocatedMaskChannel = channelManager.getChannel(allocatedBlendMaskChannel);
    deValue* allocatedMaskPixels = allocatedMaskChannel->getPixels();

    deBlurType type = deGaussianBlur;
    deValue t= 0.0;

    if (blendBlurRadius <= 0)
    {
        copyChannel(maskChannel->getPixels(), allocatedMaskChannel->getPixels(), channelManager.getChannelSize());
    }
    else
    {
        blurChannel(maskPixels, allocatedMaskPixels, channelManager.getChannelSize(), blendBlurRadius, type, t);
    }       

    processLinear(allocatedMaskPixels, channelManager.getChannelSize().getN(), blendMaskMin, blendMaskMax, false);

}

void deActionLayer::showBlendMask()
{
    blendMaskShow = true;
}

void deActionLayer::hideBlendMask()
{
    blendMaskShow = false;
}

void deActionLayer::setBlendMaskMin(deValue v)
{
    blendMaskMin = v;
    onBlendSet();
}

void deActionLayer::setBlendMaskMax(deValue v)
{
    blendMaskMax = v;
    onBlendSet();
}

bool deActionLayer::isChannelEnabled(int index) const
{
    return channels.count(index) > 0;
}

void deActionLayer::enableChannel(int index)
{
    channels.insert(index);
    onChannelChange(index);
    updateOtherLayers();
    repaint();
}

void deActionLayer::disableChannel(int index)
{
    channels.erase(index);
    onChannelChange(index);
    updateOtherLayers();
    repaint();
}

void deActionLayer::saveBlend(xmlNodePtr root)
{
    saveChild(root, "enabled", str(enabled));
    saveChild(root, "blend_mode", getBlendModeName(blendMode));
    saveChild(root, "opacity", str(opacity));
    saveChild(root, "blend_mask", str(blendMask));
    saveChild(root, "blend_mask_layer", str(blendMaskLayer));
    saveChild(root, "blend_mask_channel", str(blendMaskChannel));
    saveChild(root, "blend_blur_radius", str(blendBlurRadius));
    saveChild(root, "blend_mask_min", str(blendMaskMin));
    saveChild(root, "blend_mask_max", str(blendMaskMax));

    std::string apply = "normal";
    if (applyMode == deApplyLuminance)
    {
        apply = "luminance";
    }
    if (applyMode == deApplyColor)
    {
        apply = "color";
    }
    saveChild(root, "apply", apply);

    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        bool c = isChannelEnabled(i);
        saveChild(root, "channel", str(c));
    }

};

void deActionLayer::loadBlend(xmlNodePtr root)
{
    xmlNodePtr child = root->xmlChildrenNode;

    int channelIndex = 0;
    channels.clear();

    while (child)
    {

        if ((!xmlStrcmp(child->name, BAD_CAST("channel")))) 
        {
            bool c = getBool(getContent(child));
            if (c)
            {
                channels.insert(channelIndex);
            }
            channelIndex++;
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("enabled")))) 
        {
            enabled = getBool(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("blend_mode")))) 
        {
            blendMode = blendModeFromString(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("blend_mask")))) 
        {
            blendMask = getBool(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("opacity"))))
        {
            opacity = getValue(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("blend_blur_radius"))))
        {
            blendBlurRadius = getValue(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("blend_mask_min"))))
        {
            blendMaskMin = getValue(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("blend_mask_max"))))
        {
            blendMaskMax = getValue(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("blend_mask_layer"))))
        {
            blendMaskLayer = getInt(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("blend_mask_channel"))))
        {
            blendMaskChannel = getInt(getContent(child));
        }

        if ((!xmlStrcmp(child->name, BAD_CAST("apply"))))
        {
            std::string apply = getContent(child);
            applyMode = deApplyLuminanceAndColor;
            if (apply == "luminance")
            {
                applyMode = deApplyLuminance;
            }                
            if (apply == "color")
            {
                applyMode = deApplyColor;
            }                
        }


        child = child->next;

    }        

}
