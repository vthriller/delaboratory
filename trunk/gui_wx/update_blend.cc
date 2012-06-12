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

#include "update_blend.h"
#include <wx/wx.h>
#include "color_space_utils.h"
#include "layer_with_blending.h"
#include "semaphore.h"

class deUpdateBlendThread:public wxThread
{
    private:
        virtual void *Entry()
        {
            layer.updateBlend(channel);
            semaphore.post();
            return NULL;
        }
        deLayerWithBlending& layer;
        int channel;
        deSemaphore& semaphore;
    public:    
        deUpdateBlendThread(deLayerWithBlending& _layer, int _channel, deSemaphore& _semaphore)
        :layer(_layer),
         channel(_channel),
         semaphore(_semaphore)
        {
        }
        virtual ~deUpdateBlendThread()
        {
        }
};

void updateBlendOnThread(deLayerWithBlending& layer)
{
    int n = getColorSpaceSize(layer.getColorSpace());
    int i;

    deSemaphore semaphore(0, n);

    for (i = 0; i < n; i++)
    {
        deUpdateBlendThread* thread = new deUpdateBlendThread(layer, i, semaphore);

        if ( thread->Create() != wxTHREAD_NO_ERROR )
        {
        }

        if ( thread->Run() != wxTHREAD_NO_ERROR )
        {
        }
    }

    for (i = 0; i < n; i++)
    {
        semaphore.wait();
    }
}

