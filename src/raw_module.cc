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
#include "logger.h"

deRawModule::deRawModule()
:mutex(wxMUTEX_RECURSIVE)
{
    dcraw_version = "";
    loader = NULL;
}

deRawModule::~deRawModule()
{
}

void deRawModule::onInit()
{
    dcraw_version = getDcrawVersion();
}    

std::string deRawModule::getVersion() const
{
    return dcraw_version;
}

bool deRawModule::loadRAW(const std::string& fileName, deStaticImage& image, deColorSpace colorSpace, bool half)
{
    if (loader)
    {
        return false;
    }

    bool status = false;

    lockWithLog(mutex, "raw module mutex");

    loader = new deRawLoader(fileName, image, colorSpace, half);

    status = loader->getStatus();

    mutex.Unlock();

    return status;
}

bool deRawModule::update(bool& failure)
{
    if (!loader)
    {
        return false;
    }

    bool result = loader->load(failure);

    if ((result) || (failure))
    {
        delete loader;
        loader = NULL;
    }

    return result;
}
