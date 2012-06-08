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

#include "gradient_panel.h"
#include "conversion_processor.h"

deColorPanelOld::deColorPanelOld(wxWindow* parent, wxSize _size, int style)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, _size, style)
{
    Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(deColorPanelOld::click));
    Connect(wxEVT_MOTION, wxMouseEventHandler(deColorPanelOld::hover));
    Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(deColorPanelOld::enter));
    Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(deColorPanelOld::leave));
}

deColorPanelOld::~deColorPanelOld()
{
}

void deColorPanelOld::click(wxMouseEvent &event)
{

}

void deColorPanelOld::hover(wxMouseEvent &event)
{

}

void deColorPanelOld::enter(wxMouseEvent &event)
{
//    SetBackgroundColour(wxColour(255 * r, 255 * g, 255 * b));
}

void deColorPanelOld::leave(wxMouseEvent &event)
{
//    SetBackgroundColour(wxColour(100 * r, 100 * g, 100 * b));
}


void deColorPanelOld::setRGB(deValue rr, deValue gg, deValue bb)
{
    r = rr;
    g = gg;
    b = bb;

    SetBackgroundColour(wxColour(255 * rr, 255 * gg, 255 * bb));
}        

void deColorPanelOld::setColor(deColorSpace colorSpace, int channel, deValue value)
{
    deValue rr = 0;
    deValue gg = 0;
    deValue bb = 0;

    deValue v1 = 0;
    deValue v2 = 0;
    deValue v3 = 0;
    deValue v4 = 0;

    if (colorSpace == deColorSpaceHSV)
    {
        v2 = 0.8;
        v3 = 1.0;
    }

    if (colorSpace == deColorSpaceHSL)
    {
        v2 = 0.8;
        v3 = 0.5;
    }

    if (colorSpace == deColorSpaceLCH)
    {
        v1 = 0.7;
        v2 = 0.9;
    }

    if (colorSpace == deColorSpaceLAB)
    {
        v2 = 0.5;
        v3 = 0.5;
    }

    switch (channel)
    {
        case 0:
            v1 = value;
            break;
        case 1:
            v2 = value;
            break;
        case 2:
            v3 = value;
            break;
        case 3:
            v4 = value;
            break;
    }

    deConversionProcessor p;

    deValue z;
    p.convert(colorSpace, v1, v2, v3, v4, deColorSpaceRGB, rr, gg, bb, z);

    SetBackgroundColour(wxColour(255 * rr, 255 * gg, 255 * bb));
}

deGradientPanel::deGradientPanel(wxWindow* parent, wxSize _size, deColorSpace _colorSpace)
:wxPanel(parent, wxID_ANY, wxDefaultPosition, _size),
 colorSpace(_colorSpace)
{
    bitmap = NULL;
    Connect(wxEVT_PAINT, wxPaintEventHandler(deGradientPanel::paintEvent));
}

deGradientPanel::~deGradientPanel()
{
    if (bitmap)
    {
        delete bitmap;
    }
}

void deGradientPanel::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    dc.DrawBitmap(*bitmap, 0, 0, false);
}


deGradientPanel1::deGradientPanel1(wxWindow* parent, wxSize _size, deColorSpace _colorSpace, int _channel1, int _channel2, deValue _c1, deValue _c2, deValue _c3)
:deGradientPanel(parent, _size, _colorSpace), 
channel1(_channel1), channel2(_channel2), c1(_c1), c2(_c2), c3(_c3)
{
    bitmap = NULL;
    generateBitmap();
}

deGradientPanel1::~deGradientPanel1()
{
}

void deGradientPanel1::changeChannel(int _channel)
{
    channel1 = _channel;
    generateBitmap();
    Update();
    Refresh();
}

void setValues(deValue& v1, deValue& v2, deValue& v3, deValue& v4, deColorSpace colorSpace, int channel1, int channel2, deValue u, deValue v, deValue c1, deValue c2, deValue c3)
{
    v1 = 0;
    v2 = 0;
    v3 = 0;
    v4 = 0;

    if (colorSpace == deColorSpaceLAB)
    {
        v1 = 0.9;
        v2 = 0.5;
        v3 = 0.5;
    }

    if (colorSpace == deColorSpaceLCH)
    {
        v1 = 0.8;
        v2 = 0.7;
        v3 = 0.0;
    }

    if (colorSpace == deColorSpaceHSL)
    {
        v1 = 0.0;
        v2 = 0.8;
        v3 = 0.6;
    }

    if (colorSpace == deColorSpaceHSV)
    {
        v1 = 0.0;
        v2 = 0.8;
        v3 = 0.8;
    }

    if (colorSpace == deColorSpaceXYZ)
    {
        v1 = 0.1;
        v2 = 0.1;
        v3 = 0.1;
    }

    switch (channel2)
    {
        case 0:
            v1 = u;
            break;
        case 1:
            v2 = u;
            break;
        case 2:
            v3 = u;
            break;
        case 3:
            v4 = u;
            break;
    };

    switch (channel1)
    {
        case 0:
            v1 = v;
            if ((colorSpace == deColorSpaceLCH) && (channel2 < 0))
            {
                v2 = 0;
            }
            break;
        case 1:
            v2 = v;
            break;
        case 2:
            v3 = v;
            break;
        case 3:
            v4 = v;
            break;
    };

    if (c1 >= 0)    
    {
        if ((colorSpace == deColorSpaceLAB) || (colorSpace == deColorSpaceLCH))
        {
            v1 = c1;
        }
        if ((colorSpace == deColorSpaceHSL) || (colorSpace == deColorSpaceHSV))
        {
            v3 = c1;
        }
    }

    if (c2 >= 0)    
    {
        if (colorSpace == deColorSpaceLCH)
        {
            v3 = c2;
        }
        if ((colorSpace == deColorSpaceHSL) || (colorSpace == deColorSpaceHSV))
        {
            v1 = c2;
        }
    }

    if (colorSpace == deColorSpaceCMYK)
    {
        if (c1 >= 0)
        {
            v1 = c1;
        }
        if (c2 >= 0)
        {
            v2 = c2;
        }
        if (c3 >= 0)
        {
            v3 = c3;
        }
    }
}

void deGradientPanel1::generateBitmap()
{
    if (bitmap)
    {
        delete bitmap;
    }

    deConversionProcessor cp;

    int w = GetSize().GetWidth();
    int h = GetSize().GetHeight();
    wxImage* image = new wxImage(w, h);
    unsigned char* data = image->GetData();

    int x;
    int y;
    for (x = 0; x < w; x++)
    {
        for (y = 0; y < h; y++)
        {
            float v = 0;
            float u = 0;
            if (h > w)
            {
                v = 1.0 - (float) y / h;                        
                u = (float) x / w;                        
            }
            else
            {
                v = (float) x / w;                        
                u = 1.0 - (float) y / h;                        
            }

            deValue v1 = v;
            deValue v2 = v;
            deValue v3 = v;
            deValue v4 = v;

            setValues(v1, v2, v3, v4, colorSpace, channel1, channel2, u, v, c1, c2, c3);

            deValue rr = 0;
            deValue gg = 0;
            deValue bb = 0;

            deValue z;
            cp.convert(colorSpace, v1, v2, v3, v4, deColorSpaceRGB, rr, gg, bb, z);

            unsigned char r = 255 * rr;
            unsigned char g = 255 * gg;
            unsigned char b = 255 * bb;

            data[3*(y*w+x) + 0] = r;
            data[3*(y*w+x) + 1] = g;
            data[3*(y*w+x) + 2] = b;

        }
    }
    bitmap = new wxBitmap(*image);
    delete image;
}

deGradientPanel2::deGradientPanel2(wxWindow* parent, wxSize _size, deColorSpace _colorSpace, deValue _c1, deValue _c2, deValue _c3, deValue _c4)
:deGradientPanel(parent, _size, _colorSpace), 
 c1(_c1), c2(_c2), c3(_c3), c4(_c4)
{
    bitmap = NULL;
    generateBitmap();
}

deGradientPanel2::~deGradientPanel2()
{
}

void setValues2(deValue& v1, deValue& v2, deValue& v3, deValue& v4, deColorSpace colorSpace, deValue c1, deValue c2, deValue c3, deValue c4, deValue u, deValue v)
{
    if (colorSpace == deColorSpaceCMYK)
    {
        v1 = c1;
        v2 = c2;
        v3 = c3;
        v4 = c4 + v * 0.2;
    }
    if (colorSpace == deColorSpaceLAB)
    {
        deValue uu = u + 0.2;
        deValue vv = v + 0.2;
        deValue s = sqrt(vv * vv + uu * uu) - 0.2;
        if (s < 0)
        {
            s = 0;
        }
        v1 = 1.0 - 0.2 * s;
        v2 = c1;
        v3 = c2;
        v4 = 0;
    }
}

void deGradientPanel2::generateBitmap()
{
    if (bitmap)
    {
        delete bitmap;
    }

    deConversionProcessor cp;

    int w = GetSize().GetWidth();
    int h = GetSize().GetHeight();
    wxImage* image = new wxImage(w, h);
    unsigned char* data = image->GetData();

    int x;
    int y;

    for (x = 0; x < w; x++)
    {
        for (y = 0; y < h; y++)
        {
            float v = 0;
            float u = 0;

            deValue d = 2;

            deValue xx = fabs(x - w / d) / (w / d);
            deValue yy = fabs(y - h / d) / (h / d);

            if (h > w)
            {
                v = yy;
                u = xx;
            }
            else
            {
                v = xx;
                u = yy;
            }

            deValue v1 = v;
            deValue v2 = v;
            deValue v3 = v;
            deValue v4 = v;
    
            setValues2(v1, v2, v3, v4, colorSpace, c1, c2, c3, c4, u, v);

            deValue rr = 0;
            deValue gg = 0;
            deValue bb = 0;

            deValue z;
            cp.convert(colorSpace, v1, v2, v3, v4, deColorSpaceRGB, rr, gg, bb, z);

            unsigned char r = 255 * rr;
            unsigned char g = 255 * gg;
            unsigned char b = 255 * bb;

            data[3*(y*w+x) + 0] = r;
            data[3*(y*w+x) + 1] = g;
            data[3*(y*w+x) + 2] = b;

        }
    }
    bitmap = new wxBitmap(*image);
    delete image;
}


deGradientPanel0::deGradientPanel0(wxWindow* parent, wxSize _size, deColorSpace _colorSpace, int _channel, int _margin)
:deGradientPanel(parent, _size, _colorSpace), 
channel(_channel), margin(_margin) 
{
    bitmap = NULL;
    generateBitmap();
}

deGradientPanel0::~deGradientPanel0()
{
}

void deGradientPanel0::changeChannel(int _channel)
{
    channel = _channel;
    generateBitmap();
    Update();
    Refresh();
}

void deGradientPanel0::generateBitmap()
{
    if (bitmap)
    {
        delete bitmap;
    }

    deConversionProcessor cp;

    int w = GetSize().GetWidth();
    int h = GetSize().GetHeight();

    unsigned char gray = 128;

    int ww = w - 2 * margin;
    wxImage* image = new wxImage(w, h);
    unsigned char* data = image->GetData();

    int xx;
    int y;
    for (xx = 0; xx < w; xx++)
    {
        int x = xx - margin;
        if (x < 0)
        {
            x = -1;
        }
        if (x >= ww)
        {
            x = -1;
        }
        for (y = 0; y < h; y++)
        {
            unsigned char r = gray;
            unsigned char g = gray;
            unsigned char b = gray;

            if (x >= 0)
            {
                float v = 0;
                float u = 0;
                if (h > ww)
                {
                    v = 1.0 - (float) y / h;                        
                    u = (float) x / ww;                        
                }
                else
                {
                    v = (float) x / ww;                        
                    u = 1.0 - (float) y / h;                        
                }

                deValue v1 = v;
                deValue v2 = v;
                deValue v3 = v;
                deValue v4 = v;

                setValues(v1, v2, v3, v4, colorSpace, channel, -1, u, v, -1, -1, -1);

                deValue rr = 0;
                deValue gg = 0;
                deValue bb = 0;

                deValue z;
                cp.convert(colorSpace, v1, v2, v3, v4, deColorSpaceRGB, rr, gg, bb, z);

                r = 255 * rr;
                g = 255 * gg;
                b = 255 * bb;
            }

            data[3*(y*w+xx) + 0] = r;
            data[3*(y*w+xx) + 1] = g;
            data[3*(y*w+xx) + 2] = b;

        }
    }
    bitmap = new wxBitmap(*image);
    delete image;
}
