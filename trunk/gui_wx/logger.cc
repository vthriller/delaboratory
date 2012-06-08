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
#include <sstream>
#include <wx/wx.h>

#define LOCK_THRESHOLD 100
#define LOGGING 1

class deLoggerHelp
{
    private:
    public:
        wxThreadIdType main_id;
        wxStopWatch sw;
        deLoggerHelp()
        {
        }
        virtual ~deLoggerHelp()
        {
        }

};

deLogger& deLogger::getLogger()
{
    static deLogger logger;
    return logger;
}

deLogger::deLogger()
{
    help = new deLoggerHelp();
    f = NULL;
    started = false;
}

deLogger::~deLogger()
{
    mutex.lock();

    if (f)
    {
        f->close();
        delete f;
    }

    delete help;

    mutex.unlock();
}

void deLogger::setFile(const std::string& fileName)
{
#ifdef LOGGING    
    mutex.lock();

    if (f)
    {
        f->close();
        delete f;
    }

    f = new std::ofstream(fileName.c_str());
    if (!f)
    {
        logError("can't create logfile: " + fileName);
    }

    mutex.unlock();
#endif    
}

std::string deLogger::getThreadName()
{
    if (!started)
    {
        help->main_id = wxThread::GetCurrentId();
        started = true;
    }

    wxThreadIdType c_id = wxThread::GetCurrentId();

    std::string thr = "main";

    if (help->main_id != c_id)
    {
        std::ostringstream oss;
        oss.str("");
        oss << c_id;
        thr = oss.str();
    }            

    return thr;
}

void deLogger::log(const std::string& message)
{
    mutex.lock();

    if (f)
    {
        int t = help->sw.Time();
            
        (*f) << t << ": [" << getThreadName() << "] " << message << std::endl;
    }

    mutex.unlock();
}

void deLogger::logInfo(const std::string& message)
{
    log("INFO " + message);
}

int deLogger::getTime() const
{
    int t = help->sw.Time();
    return t;
}

void logInfo(const std::string& message)
{
#ifdef LOGGING    
    deLogger::getLogger().logInfo(message);
#endif    
}

void logError(const std::string& message)
{
    const std::string m = "ERROR " + message;
    static deMutex mutex;
    mutex.lock();
    std::cout << m << std::endl;
    mutex.unlock();
#ifdef LOGGING    
    deLogger::getLogger().log(m);
#endif    
}

