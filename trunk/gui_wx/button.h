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

#ifndef _DE_BUTTON_H
#define _DE_BUTTON_H

#include <wx/wx.h>
#include "value.h"
class dePanel;
class deWindow;
class deLayerProcessor;

class deButtonImpl;

class deButton
{
    private:
        deButtonImpl* impl;
    public:
        deButton(deWindow& window, const std::string& _name);
        virtual ~deButton();

        virtual void onClick() = 0;

        deWindow& getWindow();
};


#endif
