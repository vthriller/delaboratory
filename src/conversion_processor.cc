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

#include "conversion_processor.h"

#include "copy_image.h"
#include "image.h"
#include "conversion_functions.h"
#include "channel_manager.h"
#include "color_space_utils.h"

#include "logger.h"

bool deConversionProcessor::renderImageToRGB(const deImage& image, unsigned char* data, deChannelManager& channelManager)
{
    deColorSpace colorSpace = image.getColorSpace();

    deConversion3x3 conversion3x3 = getConversion3x3(colorSpace, deColorSpaceRGB);
    deConversion4x3 conversion4x3 = NULL;
    deConversion1x3 conversion1x3 = NULL;
    if (!conversion3x3)
    {
        conversion4x3 = getConversion4x3(colorSpace, deColorSpaceRGB);
        if (!conversion4x3)
        {
            conversion1x3 = getConversion1x3(colorSpace, deColorSpaceRGB);
            if (!conversion1x3)
            {
                logError("no conversion found");
                return false;
            }            
        }            
    }

    const deSize& s = image.getChannelSize();

    deChannel* channel0 = channelManager.getChannel(image.getChannelIndex(0));
    deChannel* channel1 = channelManager.getChannel(image.getChannelIndex(1));
    deChannel* channel2 = channelManager.getChannel(image.getChannelIndex(2));
    deChannel* channel3 = channelManager.getChannel(image.getChannelIndex(3));

    if (conversion4x3)
    {
        if (!channel0)
        {
            return false;
        }
        if (!channel1)
        {
            return false;
        }
        if (!channel2)
        {
            return false;
        }
        if (!channel3)
        {
            return false;
        }
        channel0->lockRead();
        channel1->lockRead();
        channel2->lockRead();
        channel3->lockRead();
    }        
    else if (conversion1x3)
    {
        if (!channel0)
        {
            return false;
        }
        channel0->lockRead();
    }        
    else
    {
        if (!channel0)
        {
            logError("no channel 0");
            return false;
        }
        if (!channel1)
        {
            logError("no channel 1");
            return false;
        }
        if (!channel2)
        {
            logError("no channel 2");
            return false;
        }
        channel0->lockRead();
        channel1->lockRead();
        channel2->lockRead();
    }        

    deValue rr;
    deValue gg;
    deValue bb;

    int n = s.getN();
    int i;
    int pos = 0;

    if (conversion4x3)
    {
        const deValue* p0 = channel0->getPixels();
        const deValue* p1 = channel1->getPixels();
        const deValue* p2 = channel2->getPixels();
        const deValue* p3 = channel3->getPixels();
        for (i = 0; i < n; i++)
        {
            deValue s0 = p0[i];
            deValue s1 = p1[i];
            deValue s2 = p2[i];
            deValue s3 = p3[i];
            conversion4x3(s0, s1, s2, s3, rr, gg, bb);
            
            unsigned char r = 255 * rr;
            data[pos] = r;
            pos++;
            unsigned char g = 255 * gg;
            data[pos] = g;
            pos++;
            unsigned char b = 255 * bb;
            data[pos] = b;
            pos++;
        }

        channel0->unlockRead();
        channel1->unlockRead();
        channel2->unlockRead();
        channel3->unlockRead();

    }
    else if (conversion1x3)
    {
        const deValue* p0 = channel0->getPixels();
        for (i = 0; i < n; i++)
        {
            deValue s0 = p0[i];
            conversion1x3(s0, rr, gg, bb);

            unsigned char r = 255 * rr;
            data[pos] = r;
            pos++;
            unsigned char g = 255 * gg;
            data[pos] = g;
            pos++;
            unsigned char b = 255 * bb;
            data[pos] = b;
            pos++;
        }
        channel0->unlockRead();
    }
    else
    {
        const deValue* p0 = channel0->getPixels();
        const deValue* p1 = channel1->getPixels();
        const deValue* p2 = channel2->getPixels();
        for (i = 0; i < n; i++)
        {

            deValue s1 = p0[i];
            deValue s2 = p1[i];
            deValue s3 = p2[i];
            conversion3x3(s1, s2, s3, rr, gg, bb);

            unsigned char r = 255 * rr;
            data[pos] = r;
            pos++;
            unsigned char g = 255 * gg;
            data[pos] = g;
            pos++;
            unsigned char b = 255 * bb;
            data[pos] = b;
            pos++;

        }
        channel0->unlockRead();
        channel1->unlockRead();
        channel2->unlockRead();
    }

    return true;

}


void convertImage3x3(const deImage& sourceImage, deImage& image, deChannelManager& channelManager, deConversion3x3 conversion)
{
    int n = sourceImage.getChannelSize().getN();

    deChannel* sc1 = channelManager.getChannel(sourceImage.getChannelIndex(0));
    if (!sc1)
    {
        return;
    }
    deChannel* sc2 = channelManager.getChannel(sourceImage.getChannelIndex(1));
    if (!sc2)
    {
        return;
    }
    deChannel* sc3 = channelManager.getChannel(sourceImage.getChannelIndex(2));
    if (!sc3)
    {
        return;
    }

    sc1->lockRead();
    sc2->lockRead();
    sc3->lockRead();

    deValue* s1 = sc1->getPixels();
    deValue* s2 = sc2->getPixels();
    deValue* s3 = sc3->getPixels();

    deChannel* dc1 = channelManager.getChannel(image.getChannelIndex(0));
    deChannel* dc2 = channelManager.getChannel(image.getChannelIndex(1));
    deChannel* dc3 = channelManager.getChannel(image.getChannelIndex(2));

    dc1->lockWrite();
    dc2->lockWrite();
    dc3->lockWrite();

    deValue* d1 = dc1->getPixels();
    deValue* d2 = dc2->getPixels();
    deValue* d3 = dc3->getPixels();

    int i;

    for (i = 0; i < n; i++)
    {
        conversion(s1[i], s2[i], s3[i], d1[i], d2[i], d3[i]);
    }

    dc1->unlockWrite();
    dc2->unlockWrite();
    dc3->unlockWrite();

    sc1->unlockRead();
    sc2->unlockRead();
    sc3->unlockRead();

}

void convertImage3x4(const deImage& sourceImage, deImage& image, deChannelManager& channelManager, deConversion3x4 conversion)
{
    int n = sourceImage.getChannelSize().getN();

    deChannel* sc1 = channelManager.getChannel(sourceImage.getChannelIndex(0));
    if (!sc1)
    {
        return;
    }
    deChannel* sc2 = channelManager.getChannel(sourceImage.getChannelIndex(1));
    if (!sc2)
    {
        return;
    }
    deChannel* sc3 = channelManager.getChannel(sourceImage.getChannelIndex(2));
    if (!sc3)
    {
        return;
    }

    deChannel* dc1 = channelManager.getChannel(image.getChannelIndex(0));
    deChannel* dc2 = channelManager.getChannel(image.getChannelIndex(1));
    deChannel* dc3 = channelManager.getChannel(image.getChannelIndex(2));
    deChannel* dc4 = channelManager.getChannel(image.getChannelIndex(3));

    dc1->lockWrite();
    dc2->lockWrite();
    dc3->lockWrite();
    dc4->lockWrite();

    sc1->lockRead();
    sc2->lockRead();
    sc3->lockRead();

    deValue* d1 = channelManager.getChannel(image.getChannelIndex(0))->getPixels();
    deValue* d2 = channelManager.getChannel(image.getChannelIndex(1))->getPixels();
    deValue* d3 = channelManager.getChannel(image.getChannelIndex(2))->getPixels();
    deValue* d4 = channelManager.getChannel(image.getChannelIndex(3))->getPixels();

    deValue* s1 = sc1->getPixels();
    deValue* s2 = sc2->getPixels();
    deValue* s3 = sc3->getPixels();

    int i;

    for (i = 0; i < n; i++)
    {
        conversion(s1[i], s2[i], s3[i], d1[i], d2[i], d3[i], d4[i]);
    }

    dc1->unlockWrite();
    dc2->unlockWrite();
    dc3->unlockWrite();
    dc4->unlockWrite();

    sc1->unlockRead();
    sc2->unlockRead();
    sc3->unlockRead();

}

void convertImage4x3(const deImage& sourceImage, deImage& image, deChannelManager& channelManager, deConversion4x3 conversion)
{
    int n = sourceImage.getChannelSize().getN();

    deChannel* sc1 = channelManager.getChannel(sourceImage.getChannelIndex(0));
    if (!sc1)
    {
        return;
    }
    deChannel* sc2 = channelManager.getChannel(sourceImage.getChannelIndex(1));
    if (!sc2)
    {
        return;
    }
    deChannel* sc3 = channelManager.getChannel(sourceImage.getChannelIndex(2));
    if (!sc3)
    {
        return;
    }
    deChannel* sc4 = channelManager.getChannel(sourceImage.getChannelIndex(3));
    if (!sc4)
    {
        return;
    }

    sc1->lockRead();
    sc2->lockRead();
    sc3->lockRead();
    sc4->lockRead();

    deValue* s1 = sc1->getPixels();
    deValue* s2 = sc2->getPixels();
    deValue* s3 = sc3->getPixels();
    deValue* s4 = sc4->getPixels();

    deChannel* dc1 = channelManager.getChannel(image.getChannelIndex(0));
    deChannel* dc2 = channelManager.getChannel(image.getChannelIndex(1));
    deChannel* dc3 = channelManager.getChannel(image.getChannelIndex(2));

    dc1->lockWrite();
    dc2->lockWrite();
    dc3->lockWrite();

    deValue* d1 = channelManager.getChannel(image.getChannelIndex(0))->getPixels();
    deValue* d2 = channelManager.getChannel(image.getChannelIndex(1))->getPixels();
    deValue* d3 = channelManager.getChannel(image.getChannelIndex(2))->getPixels();

    int i;

    for (i = 0; i < n; i++)
    {
        conversion(s1[i], s2[i], s3[i], s4[i], d1[i], d2[i], d3[i]);
    }

    sc1->unlockRead();
    sc2->unlockRead();
    sc3->unlockRead();
    sc4->unlockRead();

    dc1->unlockWrite();
    dc2->unlockWrite();
    dc3->unlockWrite();

}

void convertImage1x3(const deImage& sourceImage, deImage& image, deChannelManager& channelManager, deConversion1x3 conversion)
{
    int n = sourceImage.getChannelSize().getN();

    deChannel* sc1 = channelManager.getChannel(sourceImage.getChannelIndex(0));
    if (!sc1)
    {
        return;
    }

    sc1->lockRead();

    deValue* s1 = sc1->getPixels();

    deChannel* dc1 = channelManager.getChannel(image.getChannelIndex(0));
    deChannel* dc2 = channelManager.getChannel(image.getChannelIndex(1));
    deChannel* dc3 = channelManager.getChannel(image.getChannelIndex(2));

    dc1->lockWrite();
    dc2->lockWrite();
    dc3->lockWrite();

    deValue* d1 = channelManager.getChannel(image.getChannelIndex(0))->getPixels();
    deValue* d2 = channelManager.getChannel(image.getChannelIndex(1))->getPixels();
    deValue* d3 = channelManager.getChannel(image.getChannelIndex(2))->getPixels();

    int i;

    for (i = 0; i < n; i++)
    {
        conversion(s1[i], d1[i], d2[i], d3[i]);
    }

    dc1->unlockWrite();
    dc2->unlockWrite();
    dc3->unlockWrite();

    sc1->unlockRead();

}

void convertImage1x4(const deImage& sourceImage, deImage& image, deChannelManager& channelManager, deConversion1x4 conversion)
{
    int n = sourceImage.getChannelSize().getN();

    deChannel* sc1 = channelManager.getChannel(sourceImage.getChannelIndex(0));
    if (!sc1)
    {
        return;
    }

    sc1->lockRead();

    deValue* s1 = sc1->getPixels();

    deChannel* dc1 = channelManager.getChannel(image.getChannelIndex(0));
    deChannel* dc2 = channelManager.getChannel(image.getChannelIndex(1));
    deChannel* dc3 = channelManager.getChannel(image.getChannelIndex(2));
    deChannel* dc4 = channelManager.getChannel(image.getChannelIndex(3));

    dc1->lockWrite();
    dc2->lockWrite();
    dc3->lockWrite();
    dc4->lockWrite();

    deValue* d1 = channelManager.getChannel(image.getChannelIndex(0))->getPixels();
    deValue* d2 = channelManager.getChannel(image.getChannelIndex(1))->getPixels();
    deValue* d3 = channelManager.getChannel(image.getChannelIndex(2))->getPixels();
    deValue* d4 = channelManager.getChannel(image.getChannelIndex(3))->getPixels();

    int i;

    for (i = 0; i < n; i++)
    {
        conversion(s1[i], d1[i], d2[i], d3[i], d4[i]);
    }

    dc1->unlockWrite();
    dc2->unlockWrite();
    dc3->unlockWrite();
    dc4->unlockWrite();

    sc1->unlockRead();

}

void convertImage3x1(const deImage& sourceImage, deImage& image, deChannelManager& channelManager, deConversion3x1 conversion)
{
    int n = sourceImage.getChannelSize().getN();

    deChannel* sc1 = channelManager.getChannel(sourceImage.getChannelIndex(0));
    if (!sc1)
    {
        return;
    }
    deChannel* sc2 = channelManager.getChannel(sourceImage.getChannelIndex(1));
    if (!sc2)
    {
        return;
    }
    deChannel* sc3 = channelManager.getChannel(sourceImage.getChannelIndex(2));
    if (!sc3)
    {
        return;
    }

    deValue* s1 = sc1->getPixels();
    deValue* s2 = sc2->getPixels();
    deValue* s3 = sc3->getPixels();

    deValue* d1 = channelManager.getChannel(image.getChannelIndex(0))->getPixels();

    int i;

    for (i = 0; i < n; i++)
    {
        conversion(s1[i], s2[i], s3[i], d1[i]);
    }

}


deConversionProcessor::deConversionProcessor()
{
}

deConversionProcessor::~deConversionProcessor()
{
}

void deConversionProcessor::convertImage(const deImage& sourceImage, deImage& image, deChannelManager& channelManager)
{
    deColorSpace sourceColorSpace = sourceImage.getColorSpace();
    deColorSpace targetColorSpace = image.getColorSpace();

    if (sourceColorSpace == targetColorSpace)
    {
        copyImage(sourceImage, image);
        return;
    }

    int sn = getColorSpaceSize(sourceColorSpace);
    int tn = getColorSpaceSize(targetColorSpace);

    if ((sn == 3) && (tn == 3))
    {
        deConversion3x3 conversion = getConversion3x3(sourceColorSpace, targetColorSpace);
        if (conversion)
        {
            convertImage3x3(sourceImage, image, channelManager, conversion);
            return;
        }            
    }        

    if ((sn == 3) && (tn == 4))
    {
        deConversion3x4 conversion = getConversion3x4(sourceColorSpace, targetColorSpace);
        if (conversion)
        {
            convertImage3x4(sourceImage, image, channelManager, conversion);
            return;
        }            
    }        

    if ((sn == 4) && (tn == 3))
    {
        deConversion4x3 conversion = getConversion4x3(sourceColorSpace, targetColorSpace);
        if (conversion)
        {
            convertImage4x3(sourceImage, image, channelManager, conversion);
            return;
        }            
    }        

    if ((sn == 1) && (tn == 3))
    {
        deConversion1x3 conversion = getConversion1x3(sourceColorSpace, targetColorSpace);
        if (conversion)
        {
            convertImage1x3(sourceImage, image, channelManager, conversion);
            return;
        }            
    }        

    if ((sn == 1) && (tn == 4))
    {
        deConversion1x4 conversion = getConversion1x4(sourceColorSpace, targetColorSpace);
        if (conversion)
        {
            convertImage1x4(sourceImage, image, channelManager, conversion);
            return;
        }            
    }        

    if ((sn == 3) && (tn == 1))
    {
        deConversion3x1 conversion = getConversion3x1(sourceColorSpace, targetColorSpace);
        if (conversion)
        {
            convertImage3x1(sourceImage, image, channelManager, conversion);
            return;
        }            
    }        

}

bool deConversionProcessor::convertPixel(const deImage& image, int p, deColorSpace colorSpace, deValue &v1, deValue& v2, deValue& v3, deValue& v4)
{
    deColorSpace sourceColorSpace = image.getColorSpace();
    int sn = getColorSpaceSize(sourceColorSpace);
    int tn = getColorSpaceSize(colorSpace);

    if ((sn == 3) && (tn == 3))
    {
        deConversion3x3 conversion = getConversion3x3(sourceColorSpace, colorSpace);

        if (conversion)
        {
            const deValue* values1 = image.getValues(0);
            if (!values1)
            {
                return false;
            }
            const deValue* values2 = image.getValues(1);
            if (!values2)
            {
                return false;
            }
            const deValue* values3 = image.getValues(2);
            if (!values3)
            {
                return false;
            }
            deValue s1 = values1[p];
            deValue s2 = values2[p];
            deValue s3 = values3[p];
            conversion(s1, s2, s3, v1, v2, v3);
            v4 = -1;
            return true;
        } 
    }        

    if ((sn == 3) && (tn == 1))
    {
        deConversion3x1 conversion = getConversion3x1(sourceColorSpace, colorSpace);

        if (conversion)
        {
            const deValue* values1 = image.getValues(0);
            if (!values1)
            {
                return false;
            }
            const deValue* values2 = image.getValues(1);
            if (!values2)
            {
                return false;
            }
            const deValue* values3 = image.getValues(2);
            if (!values3)
            {
                return false;
            }
            deValue s1 = values1[p];
            deValue s2 = values2[p];
            deValue s3 = values3[p];
            conversion(s1, s2, s3, v1);
            return true;
        } 
    }        

    if ((sn == 3) && (tn == 4))
    {
        deConversion3x4 conversion = getConversion3x4(sourceColorSpace, colorSpace);

        if (conversion)
        {
            const deValue* values1 = image.getValues(0);
            if (!values1)
            {
                return false;
            }
            const deValue* values2 = image.getValues(1);
            if (!values2)
            {
                return false;
            }
            const deValue* values3 = image.getValues(2);
            if (!values3)
            {
                return false;
            }
            deValue s1 = values1[p];
            deValue s2 = values2[p];
            deValue s3 = values3[p];
            conversion(s1, s2, s3, v1, v2, v3, v4);
            return true;
        } 
    }        

    if ((sn == 4) && (tn == 4))
    {
        const deValue* values1 = image.getValues(0);
        if (!values1)
        {
            return false;
        }
        const deValue* values2 = image.getValues(1);
        if (!values2)
        {
            return false;
        }
        const deValue* values3 = image.getValues(2);
        if (!values3)
        {
            return false;
        }
        const deValue* values4 = image.getValues(3);
        if (!values4)
        {
            return false;
        }
        deValue s1 = values1[p];
        deValue s2 = values2[p];
        deValue s3 = values3[p];
        deValue s4 = values4[p];
        v1 = s1;
        v2 = s2;
        v3 = s3;
        v4 = s4;
        return true;
    }        

    if ((sn == 4) && (tn == 3))
    {
        deConversion4x3 conversion = getConversion4x3(sourceColorSpace, colorSpace);

        if (conversion)
        {
            const deValue* values1 = image.getValues(0);
            if (!values1)
            {
                return false;
            }
            const deValue* values2 = image.getValues(1);
            if (!values2)
            {
                return false;
            }
            const deValue* values3 = image.getValues(2);
            if (!values3)
            {
                return false;
            }
            const deValue* values4 = image.getValues(3);
            if (!values4)
            {
                return false;
            }
            deValue s1 = values1[p];
            deValue s2 = values2[p];
            deValue s3 = values3[p];
            deValue s4 = values4[p];
            conversion(s1, s2, s3, s4, v1, v2, v3);
            return true;
        } 
    }        

    if ((sn == 1) && (tn == 3))
    {
        deConversion1x3 conversion = getConversion1x3(sourceColorSpace, colorSpace);

        if (conversion)
        {
            const deValue* values1 = image.getValues(0);
            if (!values1)
            {
                return false;
            }
            deValue s1 = values1[p];
            conversion(s1, v1, v2, v3);
            return true;
        } 
    }        

    if ((sn == 1) && (tn == 4))
    {
        deConversion1x4 conversion = getConversion1x4(sourceColorSpace, colorSpace);

        if (conversion)
        {
            const deValue* values1 = image.getValues(0);
            if (!values1)
            {
                return false;
            }
            deValue s1 = values1[p];
            conversion(s1, v1, v2, v3, v4);
            return true;
        } 
    }        

    if ((sn == 1) && (tn == 1))
    {
        const deValue* values1 = image.getValues(0);
        if (!values1)
        {
            return false;
        }
        deValue s1 = values1[p];
        v1 = s1;
        return true;
    }        

    return false;
}

