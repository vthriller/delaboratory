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

#ifndef _DE_CURVES_LAYER_H
#define _DE_CURVES_LAYER_H

#include "layer.h"
#include "curves.h"
#include "color_space.h"
class dePreview;

class deCurvesLayer:public deLayer
{
    private:
        deCurves curves;

        void resetCurves();
        void processCurves(const dePreview& source, dePreview& destination);
        void deleteCurves();

    public:
        deCurvesLayer(deLayerStack& _stack, const std::string& _name);
        virtual ~deCurvesLayer();

        virtual bool canChangeSourceLayer() const {return true;};

        virtual void onChangeColorSpace();
        virtual dePreview* createPreview(dePreviewStack& previewStack);

        virtual deActionFrame* createActionFrame(wxWindow* parent, int layerNumber, deProject* project);

        deCurves& getCurves() {return curves;};

};

#endif
