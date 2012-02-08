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

#ifndef _DE_LOGGER_H
#define _DE_LOGGER_H

#include <string>
#include <fstream>
#include <wx/wx.h>

class deLogger
{
    private:
        std::ofstream* f;
        std::ofstream* fl;
        wxStopWatch sw;
        wxMutex mutex;
        wxThreadIdType main_id;
        bool started;

        std::string getThreadName();

        deLogger();

    public:
        static deLogger& getLogger();
        virtual ~deLogger();

        void setFile(const std::string& fileName);
        void setLocksFile(const std::string& fileName);

        void log(const std::string& message);
        void logLock(const std::string& message, int t);

        int getTime() const;
};

void logError(const std::string& message);
void logMessage(const std::string& message);
void lockWithLog(wxMutex& mutex, const std::string& message);

#endif
