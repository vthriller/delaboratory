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

#include "source_image_layer.h"
#include "channel_manager.h"
#include <iostream>
#include "view_manager.h"
#include "logger.h"
#include "static_image.h"
#include "str.h"
#include "property_boolean.h"
#include "property_choice.h"
#include "property_numeric.h"

deSourceImageLayer::deSourceImageLayer(deChannelManager& _previewChannelManager, deViewManager& _viewManager, deStaticImage& _sourceImage, deColorSpace _colorSpace)
:deBaseLayerWithProperties(_colorSpace, _previewChannelManager) ,
viewManager(_viewManager),
sourceImage(_sourceImage)
{
    std::vector<std::string> r;
    r.push_back("0");
    r.push_back("90");
    r.push_back("180");
    r.push_back("270");
    createPropertyChoice("rotate", r);
    createPropertyBoolean("horizontal mirror");
    createPropertyBoolean("vertical mirror");
    getPropertyChoice("rotate")->setSize();

    createPropertyNumeric("contrast", 0, 1.0);
    if (colorSpace == deColorSpaceProPhoto)
    {
        getPropertyNumeric("contrast")->set(0.9);
    }        
    else
    {
        getPropertyNumeric("contrast")->set(1.0);
    }
}

deSourceImageLayer::~deSourceImageLayer()
{
}

bool deSourceImageLayer::updateMainImageNotThreadedWay()
{
    const deSize ds = mainLayerImage.getChannelSize();
    if (ds.getW() == 0)
    {
        return false;
    }

    deValue z_x1;
    deValue z_y1;
    deValue z_x2;
    deValue z_y2;
    viewManager.getZoom(z_x1, z_y1, z_x2, z_y2);

    bool mirrorX = getPropertyBoolean("horizontal mirror")->get();
    bool mirrorY = getPropertyBoolean("vertical mirror")->get();
    deValue contrast = getNumericValue("contrast");
    int rotate = getInt(getPropertyChoice("rotate")->get());

    int channel;
    for (channel = 0; channel < 3; channel++)
    {
        deValue* destination = mainLayerImage.startWrite(channel);

        if (destination)
        {
            sourceImage.copyToChannel(channel, destination, z_x1, z_y1, z_x2, z_y2, ds, mirrorX, mirrorY, rotate, contrast);
        }            

        mainLayerImage.finishWrite(channel);
    }

    return true;
}

deValue deSourceImageLayer::getAspect() const
{
    deValue aspect = sourceImage.getAspect();

    if (aspect == 0)
    {
        return 0;
    }

    int rotate = getInt(getPropertyChoice("rotate")->get());
    if ((rotate == 90) || (rotate == 270))
    {
        aspect = 1.0 / aspect;
    }
    return aspect;
}
