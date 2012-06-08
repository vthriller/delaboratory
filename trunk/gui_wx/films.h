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

#ifndef _DE_FILMS_H
#define _DE_FILMS_H

#include "value.h"
#include <string>
#include <vector>

class deFilm
{
    private:
        std::string name;
        deValue r;
        deValue g;
        deValue b;
    public:
        deFilm(const std::string& _name, deValue _r, deValue _g, deValue _b)
        :name(_name), r(_r), g(_g), b(_b)
        {
        }
        virtual ~deFilm()
        {
        }

        deValue getR() {return r / 100.0;};
        deValue getG() {return g / 100.0;};
        deValue getB() {return b / 100.0;};

        std::string getName() const {return name;};
};

void getFilms(std::vector<deFilm>& films);

#endif
