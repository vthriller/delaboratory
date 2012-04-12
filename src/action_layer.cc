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

    return true;
}


deActionLayer::deActionLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager)
:deLayer(_colorSpace, _index, _sourceLayer),
 layerStack(_layerStack),
 channelManager(_channelManager),
 viewManager(_viewManager),
 imageActionPass(_colorSpace, _channelManager), 
 imageBlendPass(_colorSpace, _channelManager)
{
    enabled = true;
    blendMode = deBlendNormal;
    opacity = 1.0;

    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        channels.insert(i);
    }
}

deActionLayer::~deActionLayer()
{
}

void deActionLayer::disableNotLuminance()
{
    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        if (!isChannelLuminance(colorSpace, i))
        {
            channels.erase(i);
        }            
    }
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

    deValue* sourcePixels = sourceChannel->getPixels();
    deValue* overlayPixels = channel->getPixels();
    deValue* resultPixels = blendChannel_->getPixels();

    blendChannel(sourcePixels, overlayPixels, resultPixels, maskPixels, blendMode, opacity, channelSize);


    sourceChannel->unlockRead();
    channel->unlockRead();
    blendChannel_->unlockWrite();

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

    if ((i < 0) || (i>=getColorSpaceSize(colorSpace)))
    {
        logError("updateAction for invalid channel " +str(i)+ " requested");
        return false;
    };

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

        if ((!xmlStrcmp(child->name, BAD_CAST("opacity"))))
        {
            opacity = getValue(getContent(child));
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

