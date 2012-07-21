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

#include "mutex.h"
#include <wx/wx.h>
#include "logger.h"

class deMutexImpl
{
    private:
        wxMutex mutex;
    public:
        deMutexImpl(wxMutexType flags)
        :mutex(flags)
        {
        }

        ~deMutexImpl()
        {
        }

        void lock()
        {
            mutex.Lock();
        }

        void unlock()
        {
            mutex.Unlock();
        }

};

deMutex::deMutex(bool recursive)
{
    wxMutexType flags = wxMUTEX_DEFAULT;
    if (recursive)
    {
#ifdef DEBUG_LOG    
        logInfo("create recursive mutex");
#endif        
        flags = wxMUTEX_RECURSIVE;
    }        
    else
    {
#ifdef DEBUG_LOG    
        logInfo("create normal mutex");
#endif        
    }

    impl = new deMutexImpl(flags);
}

deMutex::~deMutex()
{
#ifdef DEBUG_LOG    
    logInfo("destroy mutex");
#endif        
    delete impl;
}

void deMutex::lock()
{
    impl->lock();
}

void deMutex::unlock()
{
    impl->unlock();
}
