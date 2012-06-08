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

#include "mutex_read_write.h"
#include "logger.h"

deMutexReadWrite::deMutexReadWrite(int _maxReaders)
:maxReaders(_maxReaders),
readSemaphore(_maxReaders, _maxReaders)
{
}

deMutexReadWrite::~deMutexReadWrite()
{
}

void deMutexReadWrite::lockRead() 
{
    readSemaphore.wait();
}

void deMutexReadWrite::unlockRead()
{
    readSemaphore.post();
}

void deMutexReadWrite::lockWrite()
{
    int i;
    for (i = 0; i < maxReaders; i++)
    {
        readSemaphore.wait();
    }
    writeMutex.lock();
}

void deMutexReadWrite::unlockWrite()
{
    writeMutex.unlock();
    int i;
    for (i = 0; i < maxReaders; i++)
    {
        readSemaphore.post();
    }
}
