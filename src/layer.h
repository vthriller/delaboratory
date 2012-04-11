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

#ifndef _DE_LAYER_H
#define _DE_LAYER_H

#include <string>
#include <map>
#include "color_space.h"
#include <libxml/parser.h>
#include <wx/wx.h>

class deProject;
class deFrame;
class deBlendFrame;
class deImage;
class dePropertyValue;
class deProperty;
class dePresetLayer;

enum deLayerProcessType
{   
    deLayerProcessInvalid,
    deLayerProcessFull,
    deLayerProcessSingleChannel,
    deLayerProcessBlend
};

class deLayer
{
    private:
        deLayer(const deLayer& layer);
        deLayer& operator = (const deLayer& layer);
        wxMutex mutex;

        virtual bool updateImage() = 0;

    protected:
        deColorSpace colorSpace;
        unsigned int index;
        unsigned int sourceLayer;

        std::vector<deProperty*> properties;
        std::map<std::string, dePresetLayer*> presets;

        int registerPropertyValue(const std::string& _name);

        void saveCommon(xmlNodePtr node);
        void saveValueProperties(xmlNodePtr node);
        void loadValueProperties(xmlNodePtr root);

    public:
        deLayer(deColorSpace _colorSpace, int _index, int _sourceLayer);
        virtual ~deLayer();

        deColorSpace getColorSpace() const;
        virtual const deImage& getImage() const = 0;

        virtual std::string getLabel() const;

        virtual bool hasAction() const;
        virtual bool hasBlending() const;
        virtual bool canDisable() const;

        virtual bool isEnabled() const;
        virtual void setEnabled(bool e);

        virtual void updateChannelUsage(std::map<int, int>& channelUsage) const = 0;

        int getIndex() const {return index;};

        virtual void load(xmlNodePtr root) = 0;
        virtual void save(xmlNodePtr root) = 0;

        virtual std::string getActionName() {return "action";};

        virtual std::string getType() const = 0;

        bool updateImageThreadCall();

        void lockLayer();
        void unlockLayer();

        void process(deLayerProcessType type, int channel);

        virtual void processFull();
        virtual void processBlend();
        virtual void processChannel(int channel);

        virtual void onUpdateProperties() {};

        int getNumberOfProperties() const {return properties.size();};
        dePropertyValue* getPropertyValue(int index);
        dePropertyValue* getPropertyValue(const std::string& _name);

        bool applyPreset(const std::string& _name);
        void getPresets(std::vector<std::string>& result);


};

#endif
