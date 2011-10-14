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

#ifndef _DE_MIXER_H
#define _DE_MIXER_H

#include <libxml/parser.h>
#include "value.h"
class deChannel;

class deMixer
{
    private:
        int size;
        deValue* weights;

    public:
        deMixer(int _size);
        virtual ~deMixer();

        deValue getValue(int c) const;
        void setValue(int c, deValue value);

        void process(const deChannel* s1, const deChannel* s2, const deChannel* s3, const deChannel* s4, deChannel& destination, int n);

        bool isNeutral(int index) const;

        void reset(int index);

        void save(xmlNodePtr node);
        void load(xmlNodePtr node);
    
};

#endif
