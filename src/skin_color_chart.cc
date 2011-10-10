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

#include "skin_color_chart.h"

void getFelixVonLuschan(std::vector<deSkinRGB>& skins)
{
    skins.push_back(deSkinRGB(244, 242, 245));
    skins.push_back(deSkinRGB(236, 235, 233));
    skins.push_back(deSkinRGB(250, 249, 247));
    skins.push_back(deSkinRGB(253, 251, 230));
    skins.push_back(deSkinRGB(253, 246, 230));

    skins.push_back(deSkinRGB(254, 247, 229));
    skins.push_back(deSkinRGB(249, 241, 239));
    skins.push_back(deSkinRGB(243, 234, 229));
    skins.push_back(deSkinRGB(244, 241, 234));
    skins.push_back(deSkinRGB(251, 252, 244));

    skins.push_back(deSkinRGB(251, 247, 235));
    skins.push_back(deSkinRGB(254, 246, 225));
    skins.push_back(deSkinRGB(255, 249, 225));
    skins.push_back(deSkinRGB(255, 249, 224));
    skins.push_back(deSkinRGB(241, 231, 195));

    skins.push_back(deSkinRGB(239, 226, 173));
    skins.push_back(deSkinRGB(224, 210, 147));
    skins.push_back(deSkinRGB(242, 226, 151));
    skins.push_back(deSkinRGB(235, 214, 159));
    skins.push_back(deSkinRGB(235, 217, 133));
    skins.push_back(deSkinRGB(227, 196, 103));

    skins.push_back(deSkinRGB(225, 193, 106));
    skins.push_back(deSkinRGB(223, 193, 123));
    skins.push_back(deSkinRGB(222, 184, 119));
    skins.push_back(deSkinRGB(199, 164, 100));
    skins.push_back(deSkinRGB(188, 151, 98));
    skins.push_back(deSkinRGB(155, 107, 67));
    skins.push_back(deSkinRGB(142, 88, 62));

    skins.push_back(deSkinRGB(121, 77, 48));
    skins.push_back(deSkinRGB(100, 49, 22));
    skins.push_back(deSkinRGB(101, 48, 32));
    skins.push_back(deSkinRGB(96, 49, 33));
    skins.push_back(deSkinRGB(87, 50, 41));
    skins.push_back(deSkinRGB(64, 32, 21));
    skins.push_back(deSkinRGB(49, 37, 41));
    skins.push_back(deSkinRGB(27, 28, 27));

}

void getSkinRanges(std::vector<deSkinRange>& ranges)
{
    deSkinRange t1;
    t1.first = 1;
    t1.last = 5;
    t1.description = "Very light or \"white\" type";
    ranges.push_back(t1);

    deSkinRange t2;
    t2.first = 6;
    t2.last = 10;
    t2.description = "Light";
    ranges.push_back(t2);

    deSkinRange t3;
    t3.first = 11;
    t3.last = 15;
    t3.description = "Light intermediate";
    ranges.push_back(t3);

    deSkinRange t4;
    t4.first = 16;
    t4.last = 21;
    t4.description = "Dark intermediate";
    ranges.push_back(t4);

    deSkinRange t5;
    t5.first = 22;
    t5.last = 28;
    t5.description = "Dark or \"brown\" type";
    ranges.push_back(t5);

    deSkinRange t6;
    t6.first = 29;
    t6.last = 36;
    t6.description = "Very dark or \"black\" type";
    ranges.push_back(t6);

}
