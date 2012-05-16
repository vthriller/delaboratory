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

#ifndef _DE_PROPERTY_CURVES_H
#define _DE_PROPERTY_CURVES_H

#include "property.h"
#include "value.h"
#include <vector>
#include "curve.h"

class dePropertyCurves:public deProperty
{
    private:
        std::vector<deCurveOld*> curves;
        int size;

    public:
        dePropertyCurves(const std::string& _name, int _size);
        virtual ~dePropertyCurves();

        int getSize() const;

        const deCurveOld* getCurve(int index) const {return (curves[index]);};
        deCurveOld* getCurve(int index) {return (curves[index]);};

        virtual void save(xmlNodePtr root) const;
        virtual void load(xmlNodePtr root);
};

#endif
