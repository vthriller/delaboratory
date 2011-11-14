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

class deProject;
class deFrame;
class deBlendFrame;
class deImage;

class deLayer
{
    private:
        deLayer(const deLayer& layer);
        deLayer& operator = (const deLayer& layer);
        std::string name;

    protected:
        deColorSpace colorSpace;
        deFrame* actionFrame;
        deBlendFrame* blendFrame;
        unsigned int index;
        unsigned int sourceLayer;

        void saveCommon(xmlNodePtr node);

    public:
        deLayer(const std::string& _name, deColorSpace _colorSpace, int _index, int _sourceLayer);
        virtual ~deLayer();

        deColorSpace getColorSpace() const;
        virtual const deImage& getImage() const = 0;
        virtual void updateImage() = 0;

        std::string getName() const;

        virtual bool hasAction() const;
        virtual bool hasBlending() const;
        virtual bool canDisable() const;

        virtual bool isEnabled() const;
        virtual void setEnabled(bool e);

        void closeActionFrame();
        void setActionFrame(deFrame* frame);

        void closeBlendFrame();
        void setBlendFrame(deBlendFrame* frame);

        virtual bool onImageClick(deValue x, deValue y);

        virtual void updateChannelUsage(std::map<int, int>& channelUsage) const = 0;

        int getIndex() const {return index;};

        virtual void onKey(int key);
       
        virtual void load(xmlNodePtr root) = 0;
        virtual void save(xmlNodePtr root) = 0;

        virtual std::string getActionName() {return "action";};

        virtual std::string getType() const = 0;

        bool checkActionFrame() const;
        bool checkBlendFrame() const;

        virtual void updateAll() {};

        void onUpdateProperties();

};

#endif
