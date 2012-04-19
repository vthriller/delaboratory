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

#ifndef _DE_CONVERSION_BW_LAYER_H
#define _DE_CONVERSION_BW_LAYER_H

#include "conversion_layer.h"
#include "mixer.h"
#include "property_value.h"
#include <vector>
class deViewManager;

class deFilm
{
    private:
        std::string name;
        deValue r;
        deValue g;
        deValue b;
    public:
        deFilm(const std::string& _name, deValue _r, deValue _g, deValue _b)
        :name(_name), r(_r), g(_g), b(_b)
        {
        }
        virtual ~deFilm()
        {
        }

        deValue getR() {return r / 100.0;};
        deValue getG() {return g / 100.0;};
        deValue getB() {return b / 100.0;};

        std::string getName() const {return name;};
};


class deConversionBWLayer:public deConversionLayer
{
    private:
        dePropertyValue add0;
        dePropertyValue add1;
        dePropertyValue add2;
        dePropertyValue add3;
        dePropertyValue overlay0;
        dePropertyValue overlay1;
        dePropertyValue overlay2;
        dePropertyValue overlay3;
        std::vector<deFilm> films;

        virtual std::string getType() const {return "conversion_bw";};
    public:
        deConversionBWLayer(int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager, int n);
        virtual ~deConversionBWLayer();

        virtual bool updateMainImageNotThreadedWay();

        dePropertyValue& getAdd0() {return add0;};
        dePropertyValue& getAdd1() {return add1;};
        dePropertyValue& getAdd2() {return add2;};
        dePropertyValue& getAdd3() {return add3;};
        dePropertyValue& getOverlay0() {return overlay0;};
        dePropertyValue& getOverlay1() {return overlay1;};
        dePropertyValue& getOverlay2() {return overlay2;};
        dePropertyValue& getOverlay3() {return overlay3;};

        virtual std::string getActionName() {return "mixer";};

        void resetM();
        void presetM(int c);

        virtual void load(xmlNodePtr root);
        virtual void save(xmlNodePtr root);

        const std::vector<deFilm>& getFilms() const {return films;};

        void applyFilm(int f);

};

#endif
