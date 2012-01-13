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

#include "logger.h"
#include "str.h"

deLogger::deLogger()
{
    f = NULL;
}

deLogger::~deLogger()
{
    mutex.Lock();

    if (f)
    {
        f->close();
    }

    mutex.Unlock();
}

void deLogger::setFile(const std::string& fileName)
{
    mutex.Lock();

    if (f)
    {
        f->close();
    }

    f = new std::ofstream(fileName.c_str());

    mutex.Unlock();
}

void deLogger::log(const std::string message)
{
    mutex.Lock();

    if (f)
    {
        int t = sw.Time();

        (*f) << str(t) + ": " + message << std::endl;
    }

    mutex.Unlock();
}
