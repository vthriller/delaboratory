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

#ifndef _DE_STATIC_IMAGE_H
#define _DE_STATIC_IMAGE_H

#include "color_space.h"
#include "mutex_read_write.h"
#include "size.h"
#include "mutex.h"
#include <vector>
class deChannel;

class deStaticImage
{
    private:
        deColorSpace colorSpace;
        std::vector<deChannel*> channels;
        std::vector<deMutexReadWrite*> mutexes;
        deSize size;
        mutable deMutex mutex;
        std::string info;
        
        deStaticImage(const deStaticImage& i);
        deStaticImage& operator = (const deStaticImage& i);

    public:
        deStaticImage();

        virtual ~deStaticImage();

        void setColorSpace(deColorSpace c);
        deColorSpace getColorSpace() const;

        const deValue* startReadStatic(int index);
        deValue* startWriteStatic(int index);
        void finishReadStatic(int index);
        void finishWriteStatic(int index);

        void setSize(const deSize& _size);
        deSize getSize() const;

        void lock();
        void unlock();

        void setInfo(const std::string& s);

        bool isEmpty() const;
};

#endif
