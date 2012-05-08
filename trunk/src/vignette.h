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

#ifndef _DE_VIGNETTE_H
#define _DE_VIGNETTE_H

#include "value.h"
#include "size.h"

class deEllipse
{
    private:
        deValue centerX;
        deValue centerY;
        deValue radiusX;
        deValue radiusY;
    public:
        deEllipse(deValue _centerX, deValue _centerY, deValue _radiusX, deValue _radiusY);
        virtual ~deEllipse();

        deValue processX(deValue x) const;
        deValue processY(deValue y) const;

        bool isValid() const;

};

void vignetteChannel(deValue* destination, deSize size, deEllipse ellipse, deValue light, deValue darkness, deValue spot);
//void vignetteChannel(deValue* destination, deSize size, deEllipse ellipse1, deEllipse ellipse2, deValue light, deValue darkness, deValue spot);

deEllipse calcEllipse(deValue radX, deValue radY, deValue cenX, deValue cenY, deValue x1, deValue y1, deValue x2, deValue y2);

#endif    
