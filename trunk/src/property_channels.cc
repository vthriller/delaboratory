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

#include "property_channels.h"
#include "channels_selector.h"
#include "color_space.h"
#include "layer.h"
#include <sstream>

dePropertyChannels::dePropertyChannels(deLayer& _parent)
:deProperty(_parent)
{
}

dePropertyChannels::~dePropertyChannels()
{
}

void dePropertyChannels::clear()
{
    channels.clear();
}

void dePropertyChannels::insert(int c)
{
    channels.insert(c);
}

void dePropertyChannels::addPanelContent(wxPanel* panel, wxSizer* sizer)
{
    selector = new deChannelsSelector(panel, *this);
    sizer->Add(selector);
}    

void dePropertyChannels::onColorSpaceChange()
{
    fill();
    selector->rebuild();
}

void dePropertyChannels::fill()
{
    clear();
    deColorSpace c = parent.getColorSpace();
    int n = getColorSpaceSize(c);
    int i;
    for (i = 0; i < n; i++)
    {
        insert(i);
    }
}

void dePropertyChannels::saveContent(xmlNodePtr node)
{
    deChannels::const_iterator i;
    for (i = channels.begin(); i != channels.end() ; i++)
    {
        xmlNodePtr child = xmlNewChild(node, NULL, xmlCharStrdup("channel"), NULL);

        std::ostringstream oss; 
        oss << (*i);

        xmlNodeSetContent(child, xmlCharStrdup(oss.str().c_str()));
    }        
}

void dePropertyChannels::load(xmlNodePtr node)
{
    clear();

    xmlNodePtr child = node->xmlChildrenNode;

    while (child)
    {
        if ((!xmlStrcmp(child->name, xmlCharStrdup("channel")))) 
        {
            xmlChar* s = xmlNodeGetContent(child);            
            std::string valueStr = (char*)(s);
            xmlFree(s);

            std::istringstream iss(valueStr);
            int channel;
            iss >> channel;

            insert(channel);
        }            

        child = child->next;
    }
}
