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

#ifndef _DE_EQUALIZER_H
#define _DE_EQUALIZER_H

#include <libxml/parser.h>
#include "value.h"
#include "mutex.h"
class deChannel;

class deEqualizer
{
    private:
        int bands;
        deValue* values;

        mutable deMutex mutex;

        deEqualizer(const deEqualizer& mixer);
        deEqualizer operator=(const deEqualizer& mixer);

        void lock() const; 
        void unlock() const;

    public:
        deEqualizer(int _size);
        virtual ~deEqualizer();

        deValue getValue(int c) const;
        void setValue(int c, deValue value);

        void process(const deChannel& s, const deChannel& e, deChannel& destination, int n, bool wrap);

        void reset();

        void save(xmlNodePtr node);
        void load(xmlNodePtr node);
    
};

#endif
