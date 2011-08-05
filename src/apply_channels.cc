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

#include "apply_channels.h"
#include "converter.h"
#include "preview.h"
#include "channel.h"

void applyChannels(const dePreview* sourcePreview, const dePreview* overlayPreview, dePreview* preview, deColorSpace colorSpace, bool a[4])
{
    deConverter converter;

    dePreview* tmpSource = new dePreview(colorSpace, preview->getRawSize());
    converter.setSource(sourcePreview);
    converter.setDestination(tmpSource);
    converter.convert();

    dePreview* tmpOverlay = new dePreview(colorSpace, preview->getRawSize());
    converter.setSource(overlayPreview);
    converter.setDestination(tmpOverlay);
    converter.convert();

    dePreview* tmpFinal = new dePreview(colorSpace, preview->getRawSize());

    int i;
    for (i = 0; i < getColorSpaceSize(colorSpace); i++)
    {
        if (a[i])
        {
            tmpFinal->getChannel(i)->copy(tmpOverlay->getChannel(i));
        }
        else
        {
            tmpFinal->getChannel(i)->copy(tmpSource->getChannel(i));
        }
    }

    converter.setSource(tmpFinal);
    converter.setDestination(preview);
    converter.convert();

    delete tmpFinal;
    delete tmpSource;
    delete tmpOverlay;

}
