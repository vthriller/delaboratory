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
#include "logger.h"

class deUpdateActionThread:public wxThread
{
    private:
        virtual void *Entry()
        {
            bool result = layer.updateActionOnThread(channel);
            if (!result)
            {
                logError("update action failed");
                layer.setError();
            }
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

bool deActionLayer::updateActionAllChannels()
{
    logMessage("update action all channels start");

    int n = getColorSpaceSize(colorSpace);
    int i;

    wxSemaphore semaphore(0, n);

    errorOnUpdate = false;

    for (i = 0; i < n; i++)
    {
        logMessage("creating update action thread for channel " + str(i));
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
        logMessage("waiting for update action thread for channel " + str(i));
        semaphore.Wait();
    }

    logMessage("update action all channels end");

    if (errorOnUpdate)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool deActionLayer::updateBlendAllChannels()
{
    logMessage("update blend all channels start");

    int n = getColorSpaceSize(colorSpace);
    int i;

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

    logMessage("update blend all channels end");

    return true;

}

bool deActionLayer::updateImageInActionLayer(bool action, bool blend, int channel)
{
    if (action)
    {
        if (channel >= 0)
        {
            if (!updateAction(channel))
            {
                return false;
            }
        }
        else
        {
            if (!updateActionAllChannels())
            {
                return false;
            }
        }
    }        

    if (blend)
    {
        if ((blendMask) || (blendMaskShow))
        {
            if (!renderBlendMask())
            {
                return false;
            }
        }

        if (channel >= 0)
        {
            if (!updateBlend(channel))
            {
                return false;
            }
        }
        else
        {   
            if (!updateBlendAllChannels())
            {
                return false;
            }
        }            
    }    

    if (!updateApply())
    {
        return false;
    }

    return true;
}


deActionLayer::deActionLayer(const std::string& _name, deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deLayer(_name, _colorSpace, _index, _sourceLayer),
 layerStack(_layerStack),
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

bool deActionLayer::updateApply()
{
    logMessage("update apply start");

    if (!enabled)
    {
        return true;
    }

    if (applyMode == deApplyLuminanceAndColor)
    {
        imageApplyPass.disableAllChannels();
        return true;
    }

    imageApplyPass.enableAllChannels();

    deLayer* source = layerStack.getLayer(sourceLayer);
    const deImage& sourceImage = source->getImage();

    int channelSize = channelManager.getChannelSize().getN();

    deChannel* sc1 = channelManager.getChannel(sourceImage.getChannelIndex(0));
    if (!sc1)
    {
        return false;
    }
    deChannel* sc2 = channelManager.getChannel(sourceImage.getChannelIndex(1));
    if (!sc2)
    {
        return false;
    }
    deChannel* sc3 = channelManager.getChannel(sourceImage.getChannelIndex(2));
    if (!sc3)
    {
        return false;
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

    logMessage("update apply end");

    return true;
}

void deActionLayer::setApplyMode(deApplyMode mode)
{
    applyMode = mode;
}


bool deActionLayer::updateBlend(int i)
{
    logMessage("update blend start");

    if (!enabled)
    {
        return true;
    }

    deLayer* source = layerStack.getLayer(sourceLayer);
    
    if (!source)
    {
        return false;
    }

    const deImage& sourceImage = source->getImage();

    int channelSize = channelManager.getChannelSize().getN();

    int s = sourceImage.getChannelIndex(i);

    if (!isBlendingEnabled())
    {
        return true;
    }

    if (isChannelNeutral(i))
    {
        if (blendMode == deBlendNormal)
        {
            imageBlendPass.disableChannel(i, s);
            return true;
        }
    }

    if (!isChannelEnabled(i))
    {
        imageBlendPass.disableChannel(i, s);
        return true;
    }

    deChannel* sourceChannel = channelManager.getChannel(s);
    if (!sourceChannel)
    {
        return false;
    }

    int c = imageActionPass.getChannelIndex(i);
    deChannel* channel = channelManager.getChannel(c);
    if (!channel)
    {
        return false;
    }

    imageBlendPass.enableChannel(i);
    int b = imageBlendPass.getChannelIndex(i);
    deChannel* blendChannel_ = channelManager.getChannel(b);
    if (!blendChannel_)
    {
        return false;
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

    logMessage("update blend end");

    return true;

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

bool deActionLayer::updateImage()
{
    return updateImageInActionLayer(true, true, -1);
}

bool deActionLayer::updateAction(int i)
{
    bool actionResult = false;

    logMessage("update action start i:" +str(i));

    if (!enabled)
    {
        return true;
    }

    logMessage("update action 2 i:" +str(i));

    deLayer* source = layerStack.getLayer(sourceLayer);

    logMessage("update action 3 i:" +str(i));

    const deImage& sourceImage = source->getImage();

    int channelSize = channelManager.getChannelSize().getN();

    int s = sourceImage.getChannelIndex(i);

    if (simpleActionProcessing())
    {
        return processAction(i);
    }

    logMessage("update action 4 i:" +str(i));

    if ((isChannelNeutral(i)) || (!isChannelEnabled(i)))
    {
        logMessage("update action 4a i:" +str(i));
        deChannel* sourceChannel = channelManager.getChannel(s);
        sourceChannel->lockRead();
        sourceChannel->unlockRead();
        imageActionPass.disableChannel(i, s);
        return true;
    }
    else
    {
        if (singleChannelProcessing())
        {
            logMessage("update action 5 i:" +str(i));
            deChannel* sourceChannel = channelManager.getChannel(s);
            if (sourceChannel)
            {
                imageActionPass.enableChannel(i);
                int c = imageActionPass.getChannelIndex(i);
                deChannel* channel = channelManager.getChannel(c);
                logMessage("update action 6 i:" +str(i));

                if (channel)
                {
                    logMessage("update action 7 i:" +str(i));
                    channel->lockWrite();
                    logMessage("update action 8 i:" +str(i));
                    sourceChannel->lockRead();
                    logMessage("update action 9 i:" +str(i));

                    actionResult = processAction(i, *sourceChannel, *channel, channelManager.getChannelSize());

                    logMessage("update action 10 i:" +str(i));

                    sourceChannel->unlockRead();
                    channel->unlockWrite();
                }
            }
        }
        else
        {
            logMessage("update action 999 i:" +str(i));
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

                actionResult = processAction4(i, sc1, sc2, sc3, sc4, *channel, channelSize);

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

    logMessage("update action end i:" +str(i));

    return actionResult;

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

bool deActionLayer::renderBlendMask()
{
    logMessage("render blend mask");

    enableBlendMaskChannel();

    deLayer* maskLayer = layerStack.getLayer(blendMaskLayer);
    const deImage& maskImage = maskLayer->getImage();
    int m = maskImage.getChannelIndex(blendMaskChannel);
    deChannel* maskChannel = channelManager.getChannel(m);
    if (!maskChannel)
    {
        return true;
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
        deValue r = viewManager.getRealScale() * blendBlurRadius;
        bool result = blurChannel(maskPixels, allocatedMaskPixels, channelManager.getChannelSize(), r, r, type, t);
        if (!result)
        {
            return false;
        }
    }       

    processLinear(allocatedMaskPixels, allocatedMaskPixels, channelManager.getChannelSize().getN(), blendMaskMin, blendMaskMax, false);

    allocatedMaskChannel->unlockWrite();
    maskChannel->unlockRead();

    logMessage("render blend mask done");

    return true;

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
}

void deActionLayer::disableChannel(int index)
{
    channels.erase(index);
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

bool deActionLayer::updateActionOnThread(int i)
{
    return updateAction(i);
}

void deActionLayer::updateBlendOnThread(int i)
{
    updateBlend(i);
}


void deActionLayer::processChannel(int channel)
{
    updateImageInActionLayer(true, true, channel);
}    

void deActionLayer::processBlend()
{
    updateImageInActionLayer(false, true, -1);
}

void deActionLayer::setError()
{
    errorOnUpdate = true;
}

void deActionLayer::setHistogramChannel(int channel)
{
    viewManager.setHistogramChannel(channel);
}

