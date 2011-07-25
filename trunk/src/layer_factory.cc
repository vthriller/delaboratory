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

#include "layer_factory.h"
#include "source_image_layer.h"
#include "mixer_layer.h"
#include "blend_layer.h"
#include "curves_layer.h"
#include "blur_layer.h"
#include "high_pass_layer.h"
#include "grain_layer.h"
#include "nd_layer.h"
#include "conversion_layer.h"

deLayerFactory::deLayerFactory()
{
}

deLayerFactory::~deLayerFactory()
{
}

deLayer* deLayerFactory::createLayer(const std::string& type, deLayerStack& stack, int index, const std::string& name)
{
    if (type == "source_image")
    {
        return new deSourceImageLayer(stack, index, name);
    }

    if (type == "curves")
    {
        return new deCurvesLayer(stack, index, name);
    }

    if (type == "mixer")
    {
        return new deMixerLayer(stack, index, name);
    }

    if (type == "conversion")
    {
        return new deConversionLayer(stack, index, name);
    }

    if (type == "blend")
    {
        return new deBlendLayer(stack, index, name);
    }

    if (type == "blur")
    {
        return new deBlurLayer(stack, index, name);
    }

    if (type == "high_pass")
    {
        return new deHighPassLayer(stack, index, name);
    }

    if (type == "nd")
    {
        return new deNDLayer(stack, index, name);
    }

    if (type == "grain")
    {
        return new deGrainLayer(stack, index, name);
    }

    return NULL;
}

