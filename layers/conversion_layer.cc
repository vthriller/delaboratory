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

#include "conversion_layer.h"
#include "layer_stack.h"
#include "conversion_processor.h"
#include "str.h"
#include "preset.h"
#include "films.h"
#include "logger.h"

deConversionLayer::deConversionLayer(deColorSpace _colorSpace, deChannelManager& _channelManager, int _sourceLayer, deLayerStack& _layerStack)
:deBaseLayerWithSource( _colorSpace, _channelManager, _sourceLayer, _layerStack)
{
    dePreset* reset = createPreset("reset");

    deColorSpace scs = getSourceColorSpace();

    if ((scs == deColorSpaceProPhoto) || (scs == deColorSpaceLAB) || (scs == deColorSpaceLCH))
    {
        deValue c = 1.0;
        createPropertyNumeric("contrast", 0, 1.0);
        reset->addNumericValue("contrast", c);
    }

    if ((scs == deColorSpaceLAB) || (scs == deColorSpaceLCH))
    {
        createPropertyNumeric("saturation", 0, 1);
        reset->addNumericValue("saturation", 1);
    }

    if (colorSpace == deColorSpaceCMYK)
    {
        createPropertyNumeric("GCR substract", 0, 1);
        reset->addNumericValue("GCR substract", 0.25);
        createPropertyNumeric("GCR Key max", 0, 1);
        reset->addNumericValue("GCR Key max", 1.0);
        createPropertyNumeric("GCR CMY min", 0, 3);
        reset->addNumericValue("GCR CMY min", 0.0);
    }

    if (colorSpace == deColorSpaceBW)
    {
        createPropertyNumeric("mixer red", -3, 3);
        reset->addNumericValue("mixer red", 0.3);
        createPropertyNumeric("mixer green", -3, 3);
        reset->addNumericValue("mixer green", 0.6);
        createPropertyNumeric("mixer blue", -3, 3);
        reset->addNumericValue("mixer blue", 0.1);

    //    addFilms();
    }

    if (scs == deColorSpaceBW)
    {
        createPropertyNumeric("pseudogrey", 0, 1.0);
        reset->addNumericValue("pseudogrey", 0);
    }

    applyPreset("reset");

}

deConversionLayer::~deConversionLayer()
{
}

void deConversionLayer::addFilms()
{
    std::vector<deFilm> films;
    getFilms(films);

    std::vector<deFilm>::iterator i;
    for (i = films.begin(); i != films.end(); i++)
    {
        //deFilm& f = *i;
        //dePreset* p = createPreset(f.getName());

    }
}

bool deConversionLayer::updateMainImageNotThreadedWay()
{
    logInfo("conversion layer start");

    deConversionProcessor p;

    deConversionCPU cpu(4);

    cpu.registers[CPU_REGISTER_OVERFLOW] = 0;
    cpu.registers[CPU_REGISTER_CMYK_KEY_SUB] = getNumericValue("GCR substract");
    cpu.registers[CPU_REGISTER_CMYK_KEY_MAX] = getNumericValue("GCR Key max");
    cpu.registers[CPU_REGISTER_CMYK_MIN_SUM] = getNumericValue("GCR CMY min");
    cpu.registers[CPU_REGISTER_BW_MIXER_R] = getNumericValue("mixer red");
    cpu.registers[CPU_REGISTER_BW_MIXER_G] = getNumericValue("mixer green");
    cpu.registers[CPU_REGISTER_BW_MIXER_B] = getNumericValue("mixer blue");
    cpu.registers[CPU_REGISTER_CONTRAST] = getNumericValue("contrast");
    cpu.registers[CPU_REGISTER_SATURATION] = getNumericValue("saturation");
    cpu.registers[CPU_REGISTER_PSEUDOGREY] = getNumericValue("pseudogrey");

    p.convertImage(getSourceImage(), mainLayerImage, cpu);

    deValue overflow = cpu.registers[CPU_REGISTER_OVERFLOW];
    int n = getSourceImage().getChannelSize().getN();
    int percentage = overflow * 10000 / n;

    if (percentage == 0)
    {
        warning = "OK";
    }
    else 
    {
        deValue p = percentage / 100.0;
        warning = "conversion OVERFLOW " + str(p) + "%";
    }

    logInfo("conversion layer DONE");

    return true;
}

void deConversionLayer::updateChannelUsage(std::map<int, int>& channelUsage, int layerIndex) const
{
    getSourceImage().updateChannelUsage(channelUsage, layerIndex);

    deBaseLayer::updateChannelUsage(channelUsage, layerIndex);
}

