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

#ifndef _DE_CHOICE_H
#define _DE_CHOICE_H

#include <vector>
#include <string>
#include "window.h"

class deChoiceImpl;

class deChoice
{
    private:
        deChoiceImpl* impl;
    public:
        deChoice(deWindow& window, const std::string& _name, const std::vector<std::string>& choices);
        virtual ~deChoice();

        virtual void onChoose(int index) = 0;
        void set(int index);

        deWindow& getWindow();
};


#endif
