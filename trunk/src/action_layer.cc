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
#include "layer_processor.h"
#include "view_manager.h"
#include "channel_manager.h"
#include "apply_luminance_color.h"
#include "blend_frame.h"
#include "blur.h"
#include "copy_channel.h"
#include "process_linear.h"
#include "str.h"
#include "xml.h"
#include "blend_channel.h"

class deUpdateActionThread:public wxThread
{
    private:
        virtual void *Entry()
        {
            layer.updateActionOnThread(channel);
            semaphore.Post();
            return NULL;
        }
        deActionLayer& layer;
        int channel;
        wxSemaphore& semaphore;
    public:    
        deUpdateActionThread(deActionLayer& _layer, int _channel, wxSemaphore& _semaphore)
        :layer(_layer),
         channel(_channel),
         semaphore(_semaphore)
        {
        }
        virtual ~deUpdateActionThread()
        {
        }
};

class deUpdateBlendThread:public wxThread
{
    private:
        virtual void *Entry()
        {
            layer.updateBlendOnThread(channel);
            semaphore.Post();
            return NULL;
        }
        deActionLayer& layer;
        int channel;
        wxSemaphore& semaphore;
    public:    
        deUpdateBlendThread(deActionLayer& _layer, int _channel, wxSemaphore& _semaphore)
        :layer(_layer),
         channel(_channel),
         semaphore(_semaphore)
        {
        }
        virtual ~deUpdateBlendThread()
        {
        }
};

void deActionLayer::updateActionAllChannels()
{
    int n = getColorSpaceSize(colorSpace);
    int i;

    if (layerProcessor.isMultithreadingEnabled())
    {
        wxSemaphore semaphore(0, n);

        for (i = 0; i < n; i++)
        {
            deUpdateActionThread* thread = new deUpdateActionThread(*this, i, semaphore);

            if ( thread->Create() != wxTHREAD_NO_ERROR )
            {
                std::cout << "creating thread... CREATE ERROR" << std::endl;
            }

            if ( thread->Run() != wxTHREAD_NO_ERROR )
            {
                std::cout << "creating thread... RUN ERROR" << std::endl;
            }
        }

        for (i = 0; i < n; i++)
        {
            semaphore.Wait();
        }
    }
    else
    {
        for (i = 0; i < n; i++)
        {
            updateActionOnThread(i);
        }
    }
}

void deActionLayer::updateBlendAllChannels()
{
    int n = getColorSpaceSize(colorSpace);
    int i;

    if (layerProcessor.isMultithreadingEnabled())
    {
        wxSemaphore semaphore(0, n);

        for (i = 0; i < n; i++)
        {
            deUpdateBlendThread* thread = new deUpdateBlendThread(*this, i, semaphore);

            if ( thread->Create() != wxTHREAD_NO_ERROR )
            {
                std::cout << "creating thread... CREATE ERROR" << std::endl;
            }

            if ( thread->Run() != wxTHREAD_NO_ERROR )
            {
                std::cout << "creating thread... RUN ERROR" << std::endl;
            }
        }

        for (i = 0; i < n; i++)
        {
            semaphore.Wait();
        }
    }
    else
    {
        for (i = 0; i < n; i++)
        {
            updateBlendOnThread(i);
        }
    }        

}

void deActionLayer::updateImageInActionLayer(bool action, bool blend, int channel)
{

    if (action)
    {
        if (channel >= 0)
        {
            updateAction(channel);
        }
        else
        {
            updateActionAllChannels();
        }
    }        

    if (blend)
    {
        if ((blendMask) || (blendMaskShow))
        {
            renderBlendMask();
        }

        if (channel >= 0)
        {
            updateBlend(channel);
        }
        else
        {
            updateBlendAllChannels();
        }            
    }    

    updateApply();
}


deActionLayer::deActionLayer(const std::string& _name, deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deLayerProcessor& _processor,  deChannelManager& _channelManager, deViewManager& _viewManager)
:deLayer(_name, _colorSpace, _index, _sourceLayer),
 layerStack(_layerStack),
 layerProcessor(_processor),
 channelManager(_channelManager),
 viewManager(_viewManager),
 imageActionPass(_colorSpace, _channelManager), 
 imageBlendMask(deColorSpaceBW, _channelManager),
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
    imageBlendMask.enableChannel(0);
}

void deActionLayer::disableBlendMaskChannel()
{
    imageBlendMask.disableChannel(0, -1);
}

void deActionLayer::setBlendBlurRadius(deValue r)
{
    blendBlurRadius = r;
}

const deImage& deActionLayer::getSourceImage() const
{
    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();
    return sourceImage;
}

bool deActionLayer::isEnabled() const
{
    return enabled;
}

void deActionLayer::setEnabled(bool e)
{
    enabled = e;
    int index = getIndex();
    layerProcessor.markUpdateAllChannels(index);
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
    // FIXME lock channels
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

    sc1->lockRead();
    sc2->lockRead();
    sc3->lockRead();

    deValue* r1 = sc1->getPixels();
    deValue* g1 = sc2->getPixels();
    deValue* b1 = sc3->getPixels();

    deChannel* bc1 = NULL;
    deChannel* bc2 = NULL;
    deChannel* bc3 = NULL;

    if (isBlendingEnabled())
    {
        bc1 = channelManager.getChannel(imageBlendPass.getChannelIndex(0));
        bc2 = channelManager.getChannel(imageBlendPass.getChannelIndex(1));
        bc3 = channelManager.getChannel(imageBlendPass.getChannelIndex(2));
    }
    else
    {
        bc1 = channelManager.getChannel(imageActionPass.getChannelIndex(0));
        bc2 = channelManager.getChannel(imageActionPass.getChannelIndex(1));
        bc3 = channelManager.getChannel(imageActionPass.getChannelIndex(2));
    }        

    bc1->lockRead();
    bc2->lockRead();
    bc3->lockRead();

    deValue* r2 = bc1->getPixels();
    deValue* g2 = bc2->getPixels();
    deValue* b2 = bc3->getPixels();

    deChannel* dc1 = channelManager.getChannel(imageApplyPass.getChannelIndex(0));
    deChannel* dc2 = channelManager.getChannel(imageApplyPass.getChannelIndex(1));
    deChannel* dc3 = channelManager.getChannel(imageApplyPass.getChannelIndex(2));

    dc1->lockWrite();
    dc2->lockWrite();
    dc3->lockWrite();

    deValue* r = dc1->getPixels();
    deValue* g = dc2->getPixels();
    deValue* b = dc3->getPixels();

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

    sc1->unlockRead();
    sc2->unlockRead();
    sc3->unlockRead();

    bc1->unlockRead();
    bc2->unlockRead();
    bc3->unlockRead();

    dc1->unlockWrite();
    dc2->unlockWrite();
    dc3->unlockWrite();
}

void deActionLayer::setApplyMode(deApplyMode mode)
{
    applyMode = mode;
}


void deActionLayer::updateBlend(int i)
{
    if (!enabled)
    {
        return;
    }

    deLayer* source = layerStack.getLayer(sourceLayer);
    
    if (!source)
    {
        return;
    }

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
    deChannel* blendChannel_ = channelManager.getChannel(b);
    if (!blendChannel_)
    {
        return;
    }

    sourceChannel->lockRead();
    channel->lockRead();
    blendChannel_->lockWrite();

    deValue* maskPixels = NULL;

    if ((blendMask) || (blendMaskShow))
    {
        if (blendMask)
        {
            deChannel* allocatedMaskChannel = channelManager.getChannel(imageBlendMask.getChannelIndex(0));

            allocatedMaskChannel->lockRead();

            maskPixels = allocatedMaskChannel->getPixels();

        }            
    }
    else
    {
        disableBlendMaskChannel();
    }        

    deValue* sourcePixels = sourceChannel->getPixels();
    deValue* overlayPixels = channel->getPixels();
    deValue* resultPixels = blendChannel_->getPixels();

    blendChannel(sourcePixels, overlayPixels, resultPixels, maskPixels, blendMode, opacity, channelSize);


    sourceChannel->unlockRead();
    channel->unlockRead();
    blendChannel_->unlockWrite();

    if (blendMask)
    {
        deChannel* allocatedMaskChannel = channelManager.getChannel(imageBlendMask.getChannelIndex(0));

        allocatedMaskChannel->unlockRead();
    }            

}

void deActionLayer::setBlendMode(deBlendMode mode)
{
    blendMode = mode;
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

void deActionLayer::setOpacity(deValue _opacity)
{
    opacity = _opacity;
}

void deActionLayer::updateImage()
{
    updateImageInActionLayer(true, true, -1);
}

void deActionLayer::updateAction(int i)
{
    // FIXME lock/unlock on channel disable/enable

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
        deChannel* sourceChannel = channelManager.getChannel(s);
        sourceChannel->lockRead();
        sourceChannel->unlockRead();
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
                    channel->lockWrite();
                    sourceChannel->lockRead();

                    processAction(i, *sourceChannel, *channel, channelManager.getChannelSize());

                    sourceChannel->unlockRead();
                    channel->unlockWrite();
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
                if (sc1)
                {
                    sc1->lockRead();
                }                    
                if (sc2)
                {
                    sc2->lockRead();
                }              
                if (sc3)
                {
                    sc3->lockRead();
                }    
                if (sc4)
                {
                    sc4->lockRead();
                }                    
                channel->lockWrite();

                processAction4(i, sc1, sc2, sc3, sc4, *channel, channelSize);

                channel->unlockWrite();

                if (sc1)
                {
                    sc1->unlockRead();
                }
                if (sc2)
                {
                    sc2->unlockRead();
                }
                if (sc3)
                {
                    sc3->unlockRead();
                }
                if (sc4)
                {
                    sc4->unlockRead();
                }                    
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
    layerProcessor.log("render blend mask");

    enableBlendMaskChannel();

    deLayer* maskLayer = layerStack.getLayer(blendMaskLayer);
    const deImage& maskImage = maskLayer->getImage();
    int m = maskImage.getChannelIndex(blendMaskChannel);
    deChannel* maskChannel = channelManager.getChannel(m);
    if (!maskChannel)
    {
        return;
    }

    maskChannel->lockRead();

    deValue* maskPixels = maskChannel->getPixels();

    deChannel* allocatedMaskChannel = channelManager.getChannel(imageBlendMask.getChannelIndex(0));

    allocatedMaskChannel->lockWrite();

    deValue* allocatedMaskPixels = allocatedMaskChannel->getPixels();

    deBlurType type = deGaussianBlur;
    deValue t= 0.0;

    if (blendBlurRadius <= 0)
    {
        copyChannel(maskChannel->getPixels(), allocatedMaskChannel->getPixels(), channelManager.getChannelSize());
    }
    else
    {
        deValue r = viewManager.getRealScale() * blendBlurRadius * 1000;
        blurChannel(maskPixels, allocatedMaskPixels, channelManager.getChannelSize(), r, type, t);
    }       

    processLinear(allocatedMaskPixels, allocatedMaskPixels, channelManager.getChannelSize().getN(), blendMaskMin, blendMaskMax, false);

    allocatedMaskChannel->unlockWrite();
    maskChannel->unlockRead();

    layerProcessor.log("render blend mask done");

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
}

void deActionLayer::setBlendMaskMax(deValue v)
{
    blendMaskMax = v;
}

bool deActionLayer::isChannelEnabled(int index) const
{
    return channels.count(index) > 0;
}

void deActionLayer::enableChannel(int index)
{
    channels.insert(index);

    int l_index = getIndex();
    layerProcessor.markUpdateSingleChannel(l_index, index);
}

void deActionLayer::disableChannel(int index)
{
    channels.erase(index);

    int l_index = getIndex();
    layerProcessor.markUpdateSingleChannel(l_index, index);
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
}

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

void deActionLayer::updateActionOnThread(int i)
{
    updateAction(i);
}

void deActionLayer::updateBlendOnThread(int i)
{
    updateBlend(i);
}
