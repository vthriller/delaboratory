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

#include "blend_color_luminosity.h"
#include "conversion_cpu.h"

void blendColorRGB(const deValue* source0, const deValue* source1, const deValue* source2, const deValue* overlay0, const deValue* overlay1, const deValue* overlay2, deValue* destination0, deValue* destination1, deValue* destination2, int n, deValue o)
{
    deConversionCPU cpu(3);
    int i;
    for (i = 0; i < n; i++)
    {
        deValue rs = source0[i];
        deValue gs = source1[i];
        deValue bs = source2[i];

        cpu.input[0] = rs;
        cpu.input[1] = gs;
        cpu.input[2] = bs;

        rgb2lab(cpu);

        deValue L = cpu.output[0];

        deValue ro = overlay0[i];
        deValue go = overlay1[i];
        deValue bo = overlay2[i];

        cpu.input[0] = ro;
        cpu.input[1] = go;
        cpu.input[2] = bo;

        rgb2lab(cpu);

        deValue A = cpu.output[1];
        deValue B = cpu.output[2];

        cpu.input[0] = L;
        cpu.input[1] = A;
        cpu.input[2] = B;

        lab2rgb(cpu);

        deValue r = cpu.output[0];
        deValue g = cpu.output[1];
        deValue b = cpu.output[2];

        destination0[i] = (1 - o) * rs + o * r;
        destination1[i] = (1 - o) * gs + o * g;
        destination2[i] = (1 - o) * bs + o * b;
    }
}
