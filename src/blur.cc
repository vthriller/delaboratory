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

#include "blur.h"
#include "preview.h"
#include "channel.h"

void blur(const deBaseChannel* source, deBaseChannel* destination)
{
    deChannel* d = dynamic_cast<deChannel*>(destination);
    if (!d)
    {
        return;
    }
    
    const deChannel* s = dynamic_cast<const deChannel*>(source);
    if (!s)
    {
        return;
    }

    d->copy(s);
}


void blur(const dePreview& sourcePreview, dePreview& destinationPreview)
{
    deColorSpace sc = sourcePreview.getColorSpace();
    deColorSpace dc = destinationPreview.getColorSpace();

    int sn = getColorSpaceSize(sc);
    int dn = getColorSpaceSize(dc);

    int n = sn;
    if (dn < n)
    {
        n = dn;
    }

    int i;
    for (i = 0; i < n; i++)
    {
        blur(sourcePreview.getChannel(i), destinationPreview.getChannel(i));
    }
}
