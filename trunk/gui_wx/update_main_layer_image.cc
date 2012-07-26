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

#include "update_main_layer_image.h"
#include <wx/wx.h>
#include "base_layer.h"
#include "semaphore.h"
#include "logger.h"
#include "color_space_utils.h"
#include "str.h"

class deUpdateActionThread:public wxThread
{
    private:
        virtual void *Entry()
        {
            bool result = layer.processMainImageSingleChannel(channel);
            if (!result)
            {
                logInfo("update action failed");
            }
            semaphore.post();
            return NULL;
        }
        deBaseLayer& layer;
        int channel;
        deSemaphore& semaphore;
    public:    
        deUpdateActionThread(deBaseLayer& _layer, int _channel, deSemaphore& _semaphore)
        :layer(_layer),
         channel(_channel),
         semaphore(_semaphore)
        {
        }
        virtual ~deUpdateActionThread()
        {
        }
};

void updateMainImageAllChannels(deBaseLayer& layer)
{
#ifdef DEBUG_LOG
    logInfo("update action all channels start");
#endif    

    int n = getColorSpaceSize(layer.getColorSpace());
    int i;

    deSemaphore semaphore(0, n);

    for (i = 0; i < n; i++)
    {
#ifdef DEBUG_LOG
        logInfo("creating update action thread for channel " + str(i));
#endif        
        deUpdateActionThread* thread = new deUpdateActionThread(layer, i, semaphore);

        if ( thread->Create() != wxTHREAD_NO_ERROR )
        {
        }

        if ( thread->Run() != wxTHREAD_NO_ERROR )
        {
        }
    }

    for (i = 0; i < n; i++)
    {
#ifdef DEBUG_LOG
        logInfo("waiting for update action thread for channel " + str(i));
#endif        
        semaphore.wait();
    }

#ifdef DEBUG_LOG
    logInfo("update action all channels DONE");
#endif    
}

