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

#ifndef _DE_PROPERTY_CHANNELS_H
#define _DE_PROPERTY_CHANNELS_H

#include "property.h"
#include <set>

typedef std::set<int> deChannels;

class dePropertyChannels:public deProperty
{
    private:
        deChannels channels;

    public:
        dePropertyChannels(deLayer& _parent);
        virtual ~dePropertyChannels();

        const deChannels& getChannels() const {return channels;};

        void clear();
        void insert(int c);

        virtual void addPanelContent(wxPanel* panel, wxSizer* sizer, dePreviewStack& stack);

};

#endif
