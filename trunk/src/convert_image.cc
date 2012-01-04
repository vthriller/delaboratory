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

#include "convert_image.h"
#include "copy_image.h"
#include "image.h"
#include "conversion_functions.h"
#include "channel_manager.h"


void convertImage3x3(const deImage& sourceImage, deImage& image, deChannelManager& channelManager, deConversion3x3 conversion)
{
    int n = channelManager.getChannelSize().getN();

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

    deValue* d1 = channelManager.getChannel(image.getChannelIndex(0))->getPixels();
    deValue* d2 = channelManager.getChannel(image.getChannelIndex(1))->getPixels();
    deValue* d3 = channelManager.getChannel(image.getChannelIndex(2))->getPixels();

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
    int n = channelManager.getChannelSize().getN();

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
    int n = channelManager.getChannelSize().getN();

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
    int n = channelManager.getChannelSize().getN();

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

void convertImage3x1(const deImage& sourceImage, deImage& image, deChannelManager& channelManager, deConversion3x1 conversion)
{
    int n = channelManager.getChannelSize().getN();

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


void convertImage(const deImage& sourceImage, deImage& image, deChannelManager& channelManager)
{
    deColorSpace sourceColorSpace = sourceImage.getColorSpace();
    deColorSpace targetColorSpace = image.getColorSpace();

    if (sourceColorSpace == targetColorSpace)
    {
        copyImage(sourceImage, image, channelManager);
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
