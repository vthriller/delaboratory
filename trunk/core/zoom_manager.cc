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

#include "zoom_manager.h"
#include "str.h"
#include "logger.h"

deZoomManager::deZoomManager()
{
    selectionMode = false;
    fullZoomOut();
}

deZoomManager::~deZoomManager()
{
}

void deZoomManager::fullZoomOut()
{
    z_x1 = 0;
    z_y1 = 0;
    z_x2 = 1;
    z_y2 = 1;
}

bool deZoomManager::isInSelectionMode() const
{
    return selectionMode;
}

void deZoomManager::enableSelectionMode()
{
    selectionMode = true;
    s_x1 = 0;
    s_x2 = 0;
    s_y1 = 0;
    s_y2 = 0;
}

bool deZoomManager::onClick(deValue x, deValue y)
{
    s_x1 = x;
    s_x2 = x;
    s_y1 = y;
    s_y2 = y;
    return true;
}

bool deZoomManager::onMove(deValue x, deValue y)
{
    if (x > 1)
    {
        x = 1;
    }
    if (y > 1)
    {
        y = 1;
    }
    if (x < s_x1)
    {
        x = s_x1;
    }
    if (y < s_y1)
    {
        y = s_y1;
    }

    s_x2 = x;
    s_y2 = y;
    return true;
}

bool deZoomManager::onRelease()
{
    if ((s_x1 < s_x2) && (s_y1 < s_y2))
    {
        deValue x1 = z_x1 + s_x1 * (z_x2 - z_x1);
        deValue x2 = z_x1 + s_x2 * (z_x2 - z_x1);
        deValue y1 = z_y1 + s_y1 * (z_y2 - z_y1);
        deValue y2 = z_y1 + s_y2 * (z_y2 - z_y1);
        z_x1 = x1;
        z_x2 = x2;
        z_y1 = y1;
        z_y2 = y2;
        logInfo("set zoom x1: " + str(z_x1) + " x2: " + str(z_x2) + " y1: " + str(z_y1) + " y2: " + str(z_y2));
    }
    else
    {
        fullZoomOut();
    }

    selectionMode = false;
    return true;
}

void deZoomManager::getSelection(deValue& _x1, deValue& _y1, deValue& _x2, deValue& _y2)
{
    _x1 = s_x1;
    _y1 = s_y1;
    _x2 = s_x2;
    _y2 = s_y2;
}

void deZoomManager::getZoom(deValue& _x1, deValue& _y1, deValue& _x2, deValue& _y2)
{
    _x1 = z_x1;
    _y1 = z_y1;
    _x2 = z_x2;
    _y2 = z_y2;
}

bool deZoomManager::isZoomed() const
{
    if (z_x1 > 0.0)
    {
        return true;
    }

    if (z_y1 > 0.0)
    {
        return true;
    }

    if (z_x2 < 1.0)
    {
        return true;
    }

    if (z_y2 < 1.0)
    {
        return true;
    }

    return false;
}
