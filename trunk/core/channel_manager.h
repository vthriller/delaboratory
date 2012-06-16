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

#ifndef _DE_CHANNEL_MANAGER_H
#define _DE_CHANNEL_MANAGER_H

#include "size.h"
#include <vector>
#include <set>
#include "mutex.h"
#include "mutex_read_write.h"

class deImage;
class deLogger;

class deChannelManager
{
    private:
        deSize channelSize;
        deMutex mutex;

        std::vector<deValue*> channels;
        std::vector<deMutexReadWrite*> mutexes;
        std::set<int> trashed;

        deChannelManager(const deChannelManager& m);
        deChannelManager& operator =(const deChannelManager& m);

    public:
        deChannelManager();
        virtual ~deChannelManager();

        void setChannelSize(const deSize& size, bool reallocate);
        deSize getChannelSizeFromChannelManager() const;

        int reserveNewChannel();
        void freeChannel(int index);

        void tryAllocateChannel(int index);
        void tryDeallocateChannel(int index);

        bool isImageEmpty() const;

        const deValue* startRead(int index);
        void finishRead(int index);
        deValue* startWrite(int index);
        void finishWrite(int index);

        void lock();
        void unlock();


};

#endif
