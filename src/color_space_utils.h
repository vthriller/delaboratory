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

#ifndef _DE_COLOR_SPACE_UTILS_H
#define _DE_COLOR_SPACE_UTILS_H

#include "color_space.h"
#include "value.h"
#include <string>
#include <vector>

int getColorSpaceSize(const deColorSpace& colorSpace);
void getColorSpaceChannelRanges(const deColorSpace& colorSpace, int index, deValue& min, deValue& max);
std::string getColorSpaceName(deColorSpace colorSpace);
std::string getChannelName(deColorSpace colorSpace, int channel);
deColorSpace colorSpaceFromString(const std::string& name);

void getSupportedColorSpaces(std::vector<deColorSpace>& result);
void getSupportedConversions(const deColorSpace& colorSpace, std::vector<deColorSpace>& result);

deValue getPresentationValue(deColorSpace colorSpace, int channel, deValue v);

bool isChannelLuminance(deColorSpace colorSpace, int channel);
bool shouldChannelBeSharpened(deColorSpace colorSpace, int channel);

int getEqualizerChannel(deColorSpace colorSpace);
bool isChannelWrapped(deColorSpace colorSpace, int channel);

bool isActionSupported(const std::string& action, deColorSpace colorSpace);

bool shouldUseAutoLevels(deColorSpace colorSpace, int channel);

#endif
