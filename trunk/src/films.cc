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

#include "films.h"

void getFilms(std::vector<deFilm>& films)
{
        // http://www.markushartel.com/blog/learn-from-markus/channel-mixer-settings
        // http://www.darktable.org/usermanual/ch03s04s36.html.php
        // http://www.pbase.com/digitalrebel/bw_film_types
        // http://www.flickr.com/groups/blackwhite/discuss/72157594354861228/
        // http://oxle.com/topic.asp?archive=true&tid=5243
        // http://keith_towers.webs.com/channelmixersettings.htm
        films.push_back(deFilm("AGFA 200X", 18, 41, 41));
        films.push_back(deFilm("Agfapan 25", 25, 39, 36));
        films.push_back(deFilm("Agfapan 100", 21, 40, 39));
        films.push_back(deFilm("Agfapan 400", 20, 41, 39));
        films.push_back(deFilm("Ilford Delta 100", 21, 42, 37));
        films.push_back(deFilm("Ilford Delta 400", 22, 42, 36));
        films.push_back(deFilm("Ilford Delta 3200", 31, 36, 33));
        films.push_back(deFilm("Ilford FP4", 28, 41, 31));
        films.push_back(deFilm("Ilford HP5", 23, 37, 40));
        films.push_back(deFilm("Ilford Pan F", 33, 36, 31));
        films.push_back(deFilm("Ilford SFX", 36, 31, 33));
        films.push_back(deFilm("Ilford XP2 Super", 21, 42, 37));
        films.push_back(deFilm("Kodak T-Max 100", 24, 37, 39));
        films.push_back(deFilm("Kodak T-Max 400", 27, 36, 37));
        films.push_back(deFilm("Kodak Tri-X 400", 25, 35, 40));
}

