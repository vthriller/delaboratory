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

#ifndef _DE_ZOOM_MANAGER_H
#define _DE_ZOOM_MANAGER_H

#include "value.h"

class deZoomManager
{
    private:
        bool selectionMode;

        deValue s_x1;
        deValue s_y1;
        deValue s_x2;
        deValue s_y2;

        deValue z_x1;
        deValue z_y1;
        deValue z_x2;
        deValue z_y2;

    public:
        deZoomManager();
        virtual ~deZoomManager();

        bool isInSelectionMode() const;
        void enableSelectionMode();

        bool onClick(deValue x, deValue y);
        bool onMove(deValue x, deValue y);
        bool onRelease();

        void getSelection(deValue& _x1, deValue& _y1, deValue& _x2, deValue& _y2);
        void getZoom(deValue& _x1, deValue& _y1, deValue& _x2, deValue& _y2);

        void fullZoomOut();

        bool isZoomed() const;
};

#endif
