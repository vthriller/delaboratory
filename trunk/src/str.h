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

#ifndef _DE_STR_H
#define _DE_STR_H

#include <string>

/* returns str before ".", so for "abcde.jpg" returns "abcde" */
std::string getBaseName(const std::string& s);

/* returns str after ".", so for "abcde.jpg" returns "jpg" */
std::string getExtension(const std::string& s);

#endif
