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

#ifndef _DE_CHANNELS_H
#define _DE_CHANNELS_H

// RGB
#define DE_CHANNEL_RED          0
#define DE_CHANNEL_GREEN        1
#define DE_CHANNEL_BLUE         2

// BW
#define DE_CHANNEL_GRAYSCALE    0

// LAB
#define DE_CHANNEL_L            0
#define DE_CHANNEL_A            1
#define DE_CHANNEL_B            2

// CMY/CMYK
#define DE_CHANNEL_CYAN         0
#define DE_CHANNEL_MAGENTA      1
#define DE_CHANNEL_YELLOW       2
#define DE_CHANNEL_KEY          3

// HSL/HSV
#define DE_CHANNEL_HUE          0
#define DE_CHANNEL_SATURATION   1
#define DE_CHANNEL_LIGHTNESS    2
#define DE_CHANNEL_VALUE        2

// XYZ
#define DE_CHANNEL_X            0
#define DE_CHANNEL_Y            1
#define DE_CHANNEL_Z            2

#endif
