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

#include "property_curves.h"
//#include "curves_panel.h"
#include "curves_editor.h"
#include "curves_layer.h"
#include "layer.h"

dePropertyCurves::dePropertyCurves(deLayer& _parent, int _panelSize)
:deProperty(_parent), panelSize(_panelSize)
{
}

dePropertyCurves::~dePropertyCurves()
{
    deleteCurves();
}

void dePropertyCurves::resetCurves(deColorSpace colorSpace)
{
    int n = getColorSpaceSize(colorSpace);
    deleteCurves();
    int i;
    for (i = 0; i < n; i++)
    {
        curves.push_back(new deCurve(panelSize));
    }
}    

void dePropertyCurves::deleteCurves()
{
    while (curves.size() > 0)
    {
        deCurves::iterator i = curves.begin();
        delete (*i);
        curves.erase(i);
    }
}

deCurve* dePropertyCurves::getCurve(int index)
{
    deCurve* curve = curves.at(index);
    return curve;
}        

void dePropertyCurves::addPanelContent(wxPanel* panel, wxSizer* sizer, dePreviewStack& stack)
{
    deCurvesLayer& curvesLayer = dynamic_cast<deCurvesLayer&>(parent);
    wxPanel* curvesEditor = new deCurvesEditor(panel, curvesLayer, stack, parent.getIndex());
    sizer->Add(curvesEditor);
}    
