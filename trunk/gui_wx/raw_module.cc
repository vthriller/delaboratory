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

bool deRawModule::loadRAW(const std::string& fileName, deStaticImage& image, bool half, bool srgb, bool brighten)
{
    if (loader)
    {
        logError("can't load RAW - loader already active");
        return false;
    }

    bool status = false;

    mutex.lock();

    loader = new deRawLoader(fileName, image, half, srgb, brighten);

    status = loader->getStatus();

    mutex.unlock();

    return status;
}

bool deRawModule::updateRawLoading(bool& failure)
{
    if (!loader)
    {
        return false;
    }

    if (failure)
    {
        logError("failure detected before load ?!");
    }

    bool result = loader->load(failure);

    if (failure)
    {
        logError("failure detected in deRawModule");
    }

    if ((result) || (failure))
    {
        delete loader;
        loader = NULL;
    }

    return result;
}

bool deRawModule::isActive() const
{
    return (loader);
}
