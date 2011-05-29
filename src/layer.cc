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

#include "layer.h"
#include "layer_stack.h"
#include "size.h"
#include "preview.h"
#include <iostream>
#include "action_frame.h"

deLayer::deLayer(const std::string& _name)
:name(_name)
{
    sourceLayer = -1;
    overlayLayer = -1;
    colorSpace = deColorSpaceInvalid;
    actionFrame = NULL;
}

deLayer::~deLayer()
{
    if (actionFrame)
    {
        actionFrame->Close();
    }
}


void deLayer::setName(const std::string& _name)
{
    name = _name;
}

void deLayer::closeActionFrame()
{
    actionFrame = NULL;
}

void deLayer::setActionFrame(deActionFrame* frame)
{
    assert(!actionFrame);
    actionFrame = frame;
}

deActionFrame* deLayer::getActionFrame()
{
    return actionFrame;
}
