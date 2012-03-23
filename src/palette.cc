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

#include "palette.h"

dePalette3::dePalette3(deColorSpace _colorSpace)
:colorSpace(_colorSpace)
{
}

dePalette3::~dePalette3()
{
}

void dePalette3::addColor(const deColor3& color)
{
    colors.push_back(color);
}

void dePalette3::optimize(const dePalette3& source, int n, deValue margin)
{
    int ss = source.colors.size();
    int sss = ss * ss;

    deValue** distances = new deValue * [ss];

    int i;
    int j;
    for (i = 0; i < ss; i++)
    {
        distances[i] = new deValue [ss];
        for (j = 0; j < ss; j++)
        {
            deValue d = 0;
            if (i != j)
            {
                d = source.colors[i].calcDistance(source.colors[j]);
            }   
            distances[i][j] = d;
        }

    }

    bool no_more = false;

    while ((colors.size() < n) && (!no_more))
    {
        int winner = -1;
        int max = 0;

        for (i = 0; i < ss; i++)
        {
            int counter = 0;
            for (j = 0; j < ss; j++)
            {
                deValue d = distances[i][j];
                if (d != 0)
                {
                    if ( d < margin )
                    {
                        counter++;
                    }
                }
            }
            if (counter > max)
            {
                winner = i;
                max = counter;
            }
        }

        if (winner >= 0)
        {
            colors.push_back(source.colors[winner]);
            for (i = 0; i < ss; i++)
            {
                deValue d = distances[winner][i];
                if (d != 0)
                {
                    if ( d < margin )
                    {
                        for (j = 0; j < ss; j++)
                        {
                            distances[i][j] = 0;
                            distances[j][i] = 0;
                        }
                    }
                }
            }
        }
        else
        {
            no_more = true;
        }
    }

    for (i = 0; i < ss; i++)
    {
        delete [] distances[i];
    }        
    delete [] distances;
}

deColor3 dePalette3::getColor(int index)
{
    return colors[index];
}
