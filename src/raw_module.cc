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

#include "raw_module.h"
#include "dcraw_support.h"
#include "image_io.h"
#include "str.h"

deRawModule::deRawModule()
{
    dcraw_version = "";
}

deRawModule::~deRawModule()
{
}

void deRawModule::onInit()
{
    dcraw_version = checkDcraw();
}    

std::string deRawModule::getVersion() const
{
    return dcraw_version;
}

bool deRawModule::loadRAW(const std::string& fileName, deStaticImage& image)
{
    std::string tmp = getTmp() + "/delaboratory.ppm";

    if (!execDcraw(fileName, tmp))
    {
        return false;
    }

    return loadPPM(tmp, image, deColorSpaceLAB);
}
