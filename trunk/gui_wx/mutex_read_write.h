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

#ifndef _DE_MUTEX_READ_WRITE_H
#define _DE_MUTEX_READ_WRITE_H

#include "mutex.h"
#include "semaphore.h"

class deMutexReadWrite
{
    private:
        int maxReaders;
        deSemaphore readSemaphore;
        deMutex writeMutex;

        deMutexReadWrite(const deMutexReadWrite& m);
        deMutexReadWrite& operator = (const deMutexReadWrite& m);

    public:
        deMutexReadWrite(int _maxReaders);
        ~deMutexReadWrite();

        void lockRead();
        void unlockRead();
        void lockWrite();
        void unlockWrite();

};


#endif
