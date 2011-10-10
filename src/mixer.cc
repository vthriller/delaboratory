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
#include "channel.h"

deMixer::deMixer(int _size)
:size(_size)
{
    weights = new deValue [size];
}

deMixer::~deMixer()
{
    delete [] weights;
}

void deMixer::reset(int index)
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (i == index)
        {
            weights[i] = 1.0;
        }
        else
        {
            weights[i] = 0.0;
        }
    }
}

deValue deMixer::getValue(int c) const
{
    if ((c < 0) || (c >= size))
    {
        return 0.0;
    }
    return weights[c];
}

void deMixer::setValue(int c, deValue value)
{
    if ((c < 0) || (c >= size))
    {
        return;
    }
    weights[c] = value;
}

void deMixer::process(const deChannel* s1, const deChannel* s2, const deChannel* s3, const deChannel* s4, deChannel& destination, int n)
{
    if (size < 1)
    {
        return;
    }

    int i;

    if (!s1)
    {
        return;
    }
    const deValue *p1 = s1->getPixels();
    if (size == 1)
    {
        for (i = 0; i < n; i++)
        {
            deValue result = weights[0] * p1[i];
            destination.setValueClip(i, result);
        }
        return;
    }        

    if (!s2)
    {
        return;
    }
    if (!s3)
    {
        return;
    }
    const deValue *p2 = s2->getPixels();
    const deValue *p3 = s3->getPixels();
    if (size == 3)
    {
        for (i = 0; i < n; i++)
        {
            deValue result = weights[0] * p1[i];
            result += weights[1] * p2[i];
            result += weights[2] * p3[i];
            destination.setValueClip(i, result);
        }
        return;
    }        

    if (!s4)
    {
        return;
    }
    const deValue *p4 = s4->getPixels();
    for (i = 0; i < n; i++)
    {
        deValue result = weights[0] * p1[i];
        result += weights[1] * p2[i];
        result += weights[2] * p3[i];
        result += weights[3] * p4[i];
        destination.setValueClip(i, result);
    }
}

bool deMixer::isNeutral(int index) const
{
    int i;
    for (i = 0; i < size; i++)
    {
        if (i == index)
        {
            if (weights[i] != 1.0)
            {
                return false;
            }
        }
        else
        {
            if (weights[i] != 0.0)
            {
                return false;
            }
        }
    }
    return true;
}

        /*
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
*/
