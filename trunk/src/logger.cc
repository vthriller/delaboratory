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

#define LOCK_THRESHOLD 100

deLogger& deLogger::getLogger()
{
    static deLogger logger;
    return logger;
}

deLogger::deLogger()
{
    f = NULL;
    fl = NULL;
    started = false;
}

deLogger::~deLogger()
{
    mutex.Lock();

    if (f)
    {
        f->close();
    }

    if (fl)
    {
        fl->close();
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

void deLogger::setLocksFile(const std::string& fileName)
{
    mutex.Lock();

    if (fl)
    {
        fl->close();
    }

    fl = new std::ofstream(fileName.c_str());

    mutex.Unlock();
}

std::string deLogger::getThreadName()
{
    if (!started)
    {
        main_id = wxThread::GetCurrentId();
        started = true;
    }

    wxThreadIdType c_id = wxThread::GetCurrentId();

    std::string thr = "main";

    if (main_id != c_id)
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
    mutex.Lock();

    if (f)
    {
        int t = sw.Time();
            
        (*f) << t << ": [" << getThreadName() << "] " << message << std::endl;
    }

    mutex.Unlock();
}

void deLogger::logLock(const std::string& message, int dt)
{
    if (dt < LOCK_THRESHOLD)
    {
        return;
    }

    mutex.Lock();

    if (f)
    {
        int t = sw.Time();
            
        (*fl) << t << ": [" << getThreadName() << "] [" << dt << "ms] " << message << std::endl;
    }

    mutex.Unlock();
}

int deLogger::getTime() const
{
    int t = sw.Time();
    return t;
}

void logMessage(const std::string& message)
{
    deLogger::getLogger().log(message);
}

void lockWithLog(wxMutex& mutex, const std::string& message)
{
    deLogger& logger = deLogger::getLogger();
    int t1 = logger.getTime();
    logger.log(message + " lock");
    mutex.Lock();
    int t2 = logger.getTime();
    logger.logLock(message, t2-t1);
}    
