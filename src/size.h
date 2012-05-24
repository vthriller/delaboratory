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

#ifndef _DE_SIZE_H
#define _DE_SIZE_H

#include <string>
#include "value.h"

class deSize
{
    private:
        int w;
        int h;

    public:
        deSize(int _w, int _h);
        virtual ~deSize();

        deSize rotated() const;

        int getW() const {return w;};
        int getH() const {return h;};
    
        int getN() const {return w*h;};

        float getAspect() const {return (float) w / h;};

        std::string str() const;

        bool isEmpty() const;
};

bool operator ==(const deSize& a, const deSize& b);
bool operator !=(const deSize& a, const deSize& b);

deSize fitInside(const deSize& area, deValue aspect);

#endif
