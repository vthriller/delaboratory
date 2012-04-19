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

#ifndef _DE_SLIDER_H
#define _DE_SLIDER_H

#include <wx/wx.h>
#include "value.h"
class deLayerProcessor;

class deSlider:public wxPanel
{
    private:
        wxSizer* sizer;
        wxStaticText* label;
        wxStaticText* labelValue;
        wxButton* reset;
        wxSlider* slider;
        int sliderRange;
        deValue valueMin;
        deValue valueMax;
        deValue defaultValue;

        void moveSlider(wxCommandEvent &event);
        void finishMoveSlider(wxCommandEvent &event);

        void updateValueFromSlider(bool finished);

        void setEdit(deValue v);
        void setSlider(deValue v);

        void click(wxCommandEvent &event);

    public:
        deSlider(wxWindow *parent, const std::string& labelString, int _sliderRange, deValue _valueMin, deValue _valueMax, deValue defaultValue);
        virtual ~deSlider();

        void setValue(deValue v);

        virtual void onValueChange(deValue value, bool finished) = 0;
};        

#endif
