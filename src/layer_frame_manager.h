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

#ifndef _DE_LAYER_FRAME_MANAGER_H
#define _DE_LAYER_FRAME_MANAGER_H

#include <list>

#include "value.h"
#include "action_frame.h"

class deLayerFrameOld;
class deLayerFrame;
class deFrameOld;

class deLayerFrameManager
{
    private:
        std::map<int, deLayerFrame*> layerFrames;
        std::list<deLayerFrameOld*> actionFrames;
        std::list<deLayerFrameOld*> blendFrames;
        deLayerFrameManager(const deLayerFrameManager& );
        deLayerFrameManager& operator =(const deLayerFrameManager& );

    public:
        deLayerFrameManager();
        virtual ~deLayerFrameManager();

        void addActionFrame(deFrameOld* frame);
        void removeActionFrame(deFrameOld* frame);
        bool checkActionFrame(int index);

        void addBlendFrame(deFrameOld* frame);
        void removeBlendFrame(deFrameOld* frame);
        bool checkBlendFrame(int index);

        void onDestroyLayer(int index);

        void destroyActionFrame(int index);
        void destroyBlendFrame(int index);

        bool onImageClick(deValue x, deValue y);
        void onKey(int key);

        void destroyAllFrames();

        void onUpdateProperties();

        bool addLayerFrame(int index, deLayerFrame* frame);
        bool removeLayerFrame(int index);
        bool destroyLayerFrame(int index);

};

#endif
