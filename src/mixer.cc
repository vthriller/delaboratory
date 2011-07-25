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

#include "mixer.h"
#include "preview.h"
#include "channel.h"
#include <iostream>
#include <sstream>

deMixer::deMixer(deColorSpace _sourceColorSpace, deColorSpace _destinationColorSpace)
:sourceColorSpace(_sourceColorSpace), destinationColorSpace(_destinationColorSpace)
{

    int sourceChannels = getColorSpaceSize(sourceColorSpace);
    int destinationChannels = getColorSpaceSize(destinationColorSpace);

    range = 3;

    values = new deValue [sourceChannels * destinationChannels];

    reset();
}

deMixer::~deMixer()
{
    delete [] values;
}

void deMixer::reset()
{
    int sourceChannels = getColorSpaceSize(sourceColorSpace);
    int destinationChannels = getColorSpaceSize(destinationColorSpace);

    deValue a = 1;
    deValue b = 0;

    if (sourceChannels == 1)
    {
        a = 1;
        b = 1;
    }

    if (destinationChannels == 1)
    {
        a = 1.0 / sourceChannels;
        b = 1.0 / sourceChannels;
    }

    int i;
    for (i = 0; i < sourceChannels; i++)
    {
        int j;
        for (j = 0; j < destinationChannels; j++)
        {
            int n = j * sourceChannels + i;

            if (i == j)
            {
                values[n] = a;
            }
            else
            {
                values[n] = b;
            }

        }
    }

}

void deMixer::calc(const dePreview& source, dePreview& destination) const
{
    int sourceChannels = getColorSpaceSize(sourceColorSpace);
    int destinationChannels = getColorSpaceSize(destinationColorSpace);

    int s = source.getChannel(0)->getSize().getN();

    int p;
    for (p = 0; p < s; p++)
    {
        int j;
        for (j = 0; j < destinationChannels; j++)
        {
            deBaseChannel* destinationChannel = destination.getChannel(j);

            deValue result = 0;

            int i;
            for (i = 0; i < sourceChannels; i++)
            {
                const deBaseChannel* sourceChannel = source.getChannel(i);

                int n = j * sourceChannels + i;

                result += sourceChannel->getValue(p) * values[n];
        
            }

            destinationChannel->setValue(p, result);
        }
    }
}


deValue deMixer::getValue(int s, int d) const
{
    int sourceChannels = getColorSpaceSize(sourceColorSpace);

    int n = d * sourceChannels + s;
    return values[n];
}

void deMixer::setValue(int s, int d, deValue value)
{
    int sourceChannels = getColorSpaceSize(sourceColorSpace);
    int destinationChannels = getColorSpaceSize(destinationColorSpace);

    if ((s < 0) || (s >= sourceChannels))
    {
        return;
    }

    if ((d < 0) || (d >= destinationChannels))
    {
        return;
    }

    int n = d * sourceChannels + s;
    values[n] = value;
}

deValue deMixer::getRangeMin() const
{
    return -range;
}

deValue deMixer::getRangeMax() const
{
    return range;
}


deColorSpace deMixer::getSourceColorSpace() const
{
    return sourceColorSpace;
}

deColorSpace deMixer::getDestinationColorSpace() const
{
    return destinationColorSpace;
}

void deMixer::save(xmlNodePtr node)
{
    int sourceChannels = getColorSpaceSize(sourceColorSpace);
    int destinationChannels = getColorSpaceSize(destinationColorSpace);
    int n = sourceChannels * destinationChannels;

    int i;
    for (i = 0; i < n; i++)
    {
        xmlNodePtr child = xmlNewChild(node, NULL, xmlCharStrdup("value"), NULL);

        std::ostringstream oss; 
        oss << values[i];

        xmlNodeSetContent(child, xmlCharStrdup(oss.str().c_str()));
    }        
}

void deMixer::load(xmlNodePtr node)
{
    int sourceChannels = getColorSpaceSize(sourceColorSpace);
    int destinationChannels = getColorSpaceSize(destinationColorSpace);
    int n = sourceChannels * destinationChannels;

    int i = 0;

    xmlNodePtr child = node->xmlChildrenNode;

    while (child)
    {
        if ((!xmlStrcmp(child->name, xmlCharStrdup("value")))) 
        {
            if (i < n)
            {
                xmlChar* s = xmlNodeGetContent(child);            
                std::string valueStr = (char*)(s);
                xmlFree(s);

                std::istringstream iss(valueStr);
                iss >> values[i];
            }            
            i++;
        }            

        child = child->next;
    }
}
