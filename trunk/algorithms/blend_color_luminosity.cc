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

void blendLuminosityRGB(const deValue* source0, const deValue* source1, const deValue* source2, const deValue* overlay0, const deValue* overlay1, const deValue* overlay2, deValue* destination0, deValue* destination1, deValue* destination2, int n, deValue o)
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

        deValue A = cpu.output[1];
        deValue B = cpu.output[2];

        deValue ro = overlay0[i];
        deValue go = overlay1[i];
        deValue bo = overlay2[i];

        cpu.input[0] = ro;
        cpu.input[1] = go;
        cpu.input[2] = bo;

        rgb2lab(cpu);

        deValue L = cpu.output[0];

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

void blendColorProPhoto(const deValue* source0, const deValue* source1, const deValue* source2, const deValue* overlay0, const deValue* overlay1, const deValue* overlay2, deValue* destination0, deValue* destination1, deValue* destination2, int n, deValue o)
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

        prophoto2lab(cpu);

        deValue L = cpu.output[0];

        deValue ro = overlay0[i];
        deValue go = overlay1[i];
        deValue bo = overlay2[i];

        cpu.input[0] = ro;
        cpu.input[1] = go;
        cpu.input[2] = bo;

        prophoto2lab(cpu);

        deValue A = cpu.output[1];
        deValue B = cpu.output[2];

        cpu.input[0] = L;
        cpu.input[1] = A;
        cpu.input[2] = B;

        lab2prophoto(cpu);

        deValue r = cpu.output[0];
        deValue g = cpu.output[1];
        deValue b = cpu.output[2];

        destination0[i] = (1 - o) * rs + o * r;
        destination1[i] = (1 - o) * gs + o * g;
        destination2[i] = (1 - o) * bs + o * b;
    }
}

void blendLuminosityProPhoto(const deValue* source0, const deValue* source1, const deValue* source2, const deValue* overlay0, const deValue* overlay1, const deValue* overlay2, deValue* destination0, deValue* destination1, deValue* destination2, int n, deValue o)
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

        prophoto2lab(cpu);

        deValue A = cpu.output[1];
        deValue B = cpu.output[2];

        deValue ro = overlay0[i];
        deValue go = overlay1[i];
        deValue bo = overlay2[i];

        cpu.input[0] = ro;
        cpu.input[1] = go;
        cpu.input[2] = bo;

        prophoto2lab(cpu);

        deValue L = cpu.output[0];

        cpu.input[0] = L;
        cpu.input[1] = A;
        cpu.input[2] = B;

        lab2prophoto(cpu);

        deValue r = cpu.output[0];
        deValue g = cpu.output[1];
        deValue b = cpu.output[2];

        destination0[i] = (1 - o) * rs + o * r;
        destination1[i] = (1 - o) * gs + o * g;
        destination2[i] = (1 - o) * bs + o * b;
    }
}

void blendColorCMYK(const deValue* source0, const deValue* source1, const deValue* source2, const deValue* source3, const deValue* overlay0, const deValue* overlay1, const deValue* overlay2, const deValue* overlay3, deValue* destination0, deValue* destination1, deValue* destination2, deValue* destination3, int n, deValue o)
{
    deConversionCPU cpu(4);
    int i;
    for (i = 0; i < n; i++)
    {
        deValue cs = source0[i];
        deValue ms = source1[i];
        deValue ys = source2[i];
        deValue ks = source3[i];

        cpu.input[0] = cs;
        cpu.input[1] = ms;
        cpu.input[2] = ys;
        cpu.input[3] = ks;

        cmyk2lab(cpu);

        deValue L = cpu.output[0];

        deValue co = overlay0[i];
        deValue mo = overlay1[i];
        deValue yo = overlay2[i];
        deValue ko = overlay3[i];

        cpu.input[0] = co;
        cpu.input[1] = mo;
        cpu.input[2] = yo;
        cpu.input[3] = ko;

        cmyk2lab(cpu);

        deValue A = cpu.output[1];
        deValue B = cpu.output[2];

        cpu.input[0] = L;
        cpu.input[1] = A;
        cpu.input[2] = B;

        lab2cmyk(cpu);

        deValue c = cpu.output[0];
        deValue m = cpu.output[1];
        deValue y = cpu.output[2];
        deValue k = cpu.output[3];

        destination0[i] = (1 - o) * cs + o * c;
        destination1[i] = (1 - o) * ms + o * m;
        destination2[i] = (1 - o) * ys + o * y;
        destination3[i] = (1 - o) * ks + o * k;
    }
}

void blendLuminosityCMYK(const deValue* source0, const deValue* source1, const deValue* source2, const deValue* source3, const deValue* overlay0, const deValue* overlay1, const deValue* overlay2, const deValue* overlay3, deValue* destination0, deValue* destination1, deValue* destination2, deValue* destination3, int n, deValue o)
{
    deConversionCPU cpu(4);
    int i;
    for (i = 0; i < n; i++)
    {
        deValue cs = source0[i];
        deValue ms = source1[i];
        deValue ys = source2[i];
        deValue ks = source3[i];

        cpu.input[0] = cs;
        cpu.input[1] = ms;
        cpu.input[2] = ys;
        cpu.input[3] = ks;

        cmyk2lab(cpu);

        deValue A = cpu.output[1];
        deValue B = cpu.output[2];

        deValue co = overlay0[i];
        deValue mo = overlay1[i];
        deValue yo = overlay2[i];
        deValue ko = overlay3[i];

        cpu.input[0] = co;
        cpu.input[1] = mo;
        cpu.input[2] = yo;
        cpu.input[3] = ko;

        cmyk2lab(cpu);

        deValue L = cpu.output[0];

        cpu.input[0] = L;
        cpu.input[1] = A;
        cpu.input[2] = B;

        lab2cmyk(cpu);

        deValue c = cpu.output[0];
        deValue m = cpu.output[1];
        deValue y = cpu.output[2];
        deValue k = cpu.output[3];

        destination0[i] = (1 - o) * cs + o * c;
        destination1[i] = (1 - o) * ms + o * m;
        destination2[i] = (1 - o) * ys + o * y;
        destination3[i] = (1 - o) * ks + o * k;
    }
}
