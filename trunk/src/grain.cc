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

#include "grain.h"
#include "preview.h"
#include "channel.h"

void fillGrain(deBaseChannel* c)
{
    deChannel* d = dynamic_cast<deChannel*>(c);
    if (!d)
    {
        return;
    }

    int seed = 0;

    srand(seed);

    int n = d->getSize().getN();
    int i;
    for (i = 0; i < n; i++)
    {
        deValue v = (deValue) rand() / RAND_MAX;
        d->setValue(i, v);
    }
}

void fillGrain(dePreview* preview)
{

    deColorSpace c = preview->getColorSpace();

    int n = getColorSpaceSize(c);

    int i;

    for (i = 0; i < n; i++)
    {
        fillGrain(preview->getChannel(i));
    }
}
