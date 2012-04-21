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

#ifndef _DE_IMAGE_H
#define _DE_IMAGE_H

#include "color_space.h"
#include "value.h"
#include <map>
class deChannelManager;
class deSize;

class deImage
{
    private:
        const deColorSpace colorSpace;
        int channelsVisible[MAX_COLOR_SPACE_SIZE];
        int channelsAllocated[MAX_COLOR_SPACE_SIZE];
        deChannelManager& channelManager;

        deImage(const deImage& i);
        deImage& operator = (const deImage& i);

    public:
        deImage(const deColorSpace& _colorSpace, deChannelManager& _channelManager);

        virtual ~deImage();

        deColorSpace getColorSpace() const;

        int getChannelIndex(int n) const;

        void enableChannel(int n);
        void disableChannel(int n, int c);

        void disableAllChannels();
        void enableAllChannels();

        void updateChannelUsage(std::map<int, int>& channelUsage, int index) const;

        void lockRead() const;
        void unlockRead() const;

        const deValue* getValues(int channel) const;
        deValue* getValues(int channel);
        const deSize getChannelSize() const;


};

#endif
