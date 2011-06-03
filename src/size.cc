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

#include "size.h"
#include <sstream>

deSize::deSize(int _w, int _h)
{
    w = _w;
    h = _h;
}

deSize::~deSize()
{
}

bool operator ==(const deSize& a, const deSize& b)
{
    return ( ( a.getW() == b.getW() ) && ( a.getH() == b.getH() ) );
}

bool operator !=(const deSize& a, const deSize& b)
{
    return !( ( a.getW() == b.getW() ) && ( a.getH() == b.getH() ) );
}

deSize fitInside(const deSize& area, const deSize& rect)
{
    if ((rect.getW() == 0) && (rect.getH() == 0))
    {
        return deSize(area.getW(), area.getH());
    }

    int w = area.getW();
    int h = w/rect.getAspect();

    if (h > area.getH())
    {
        h = area.getH();
        w = h * rect.getAspect();
    }

    return deSize(w,h);
}

std::string deSize::str() const
{
    std::ostringstream oss;
    oss << "[w: " << w << " h: " << h << "]";
    return oss.str();
}
