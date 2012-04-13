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

#ifndef _DE_ACTION_LAYER_H
#define _DE_ACTION_LAYER_H

#include "layer.h"
#include "blend_mode.h"
#include "size.h"
#include "image.h"
#include "channel.h"
class deLayerStack;
class deChannelManager;
class deViewManager;
#include <set>

class deActionLayer:public deLayer
{
    private:

        // global enabled flag
        bool enabled;

        // separate enabled flag for each channel
        std::set<int> channels;

        // blend settings - mode and opacity
        deBlendMode blendMode;
        deValue opacity;

        // set from thread
        bool errorOnUpdate;

        virtual bool hasAction() const {return true;};
        virtual bool hasBlending() const {return true;};
        virtual bool canDisable() const {return true;};

        virtual bool simpleActionProcessing() const {return false;};
        virtual bool onlyFullProcessing() const {return false;};

        virtual const deImage& getSourceImage() const;

        virtual bool isEnabled() const;
        virtual void setEnabled(bool e);

        virtual const deImage& getImage() const;

        virtual bool isChannelNeutral(int index) = 0;

        bool updateAction(int i);

        bool isBlendingEnabled() const;

        bool updateActionAllChannels();
        bool updateBlendAllChannels();

    protected:
        deLayerStack& layerStack;

    private:

        deChannelManager& channelManager;

    protected:
        deViewManager& viewManager;

        deImage imageActionPass;

        void disableNotLuminance();
        void disableNotForSharpen();

    private:        
        deImage imageBlendPass;

        bool fullProcessing();
        bool updateBlend(int i);

        virtual bool processAction(int i) {return false;};
        virtual bool processAction(int i, const deChannel& sourceChannel, deChannel& channel, deSize size) {return false;};
        virtual bool processAction4(int i, const deChannel* s1, const deChannel* s2, const deChannel* s3, const deChannel* s4, deChannel& channel, int channelSize) {return false;};
        virtual bool processActionFull(deValue** sp, deValue** dp, int channelSize) {return false;};

        virtual bool singleChannelProcessing() const = 0;

        virtual bool updateImage();

    public:
        deActionLayer(deColorSpace _colorSpace, int _index, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, deViewManager& _viewManager);
        virtual ~deActionLayer();

        deChannel* getSourceChannel(int index);
        deSize getChannelSize() const;
        void setOpacity(deValue _alpha);
        void setBlendMode(deBlendMode mode);
        deBlendMode getBlendMode() const {return blendMode;};
        deValue getOpacity();

        virtual void updateChannelUsage(std::map<int, int>& channelUsage) const;

        deViewManager& getViewManager() {return viewManager;};

        bool isChannelEnabled(int index) const;
        void enableChannel(int index);
        void disableChannel(int index);

        virtual void loadBlend(xmlNodePtr root);
        virtual void saveBlend(xmlNodePtr root);

        bool updateImageInActionLayer(bool action, bool blend, int channel);
        bool updateActionOnThread(int i);
        void updateBlendOnThread(int i);

        virtual void processChannel(int channel);
        virtual void processBlend();

        void setError();

        void setHistogramChannel(int channel);


};

#endif
