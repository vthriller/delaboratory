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
#include "layer_frame.h"
#include "size.h"
#include "preview.h"
#include <iostream>
#include "action_frame.h"
#include "layer_dialog.h"
#include "project.h"
#include <sstream>

deLayer::deLayer(deLayerStack& _stack, int _index, const std::string& _name)
:stack(_stack), index(_index), name(*this), sourceLayer(*this, "source"), overlayLayer(*this, "overlay"), colorSpace(*this)
{
    name.setName(_name);
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
    name.setName(_name);
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

wxDialog* deLayer::createDialog(wxWindow* parent, int layerNumber, deProject* project)
{
    deLayerDialog* dialog = new deLayerDialog(parent, *this, layerNumber, project, "options");
    if (canChangeColorSpace())
    {
        dialog->addColorSpaceChoice();
    }        
    if (canChangeSourceLayer())
    {
        dialog->addSourceChoice();
    }        
    if (canChangeOverlayLayer())
    {
        dialog->addOverlayChoice();
    }
    return dialog;
}

void deLayer::changeSourceLayer(int id)
{
    sourceLayer.setIndex(id);
    overlayLayer.setIndex(id);
    updateColorSpace();
    onChangeSourceLayer();
}

void deLayer::changeOverlayLayer(int id)
{
    overlayLayer.setIndex(id);
    onChangeOverlayLayer();
}

void deLayer::changeColorSpace(deColorSpace _colorSpace)
{
    colorSpace.setColorSpace(_colorSpace);
    onChangeColorSpace();
}

deLayer* deLayer::getSourceLayer()
{
    return stack.getLayer(sourceLayer.getIndex());
}

void deLayer::updateColorSpace()
{
    if ((colorSpace.getColorSpace() != deColorSpaceInvalid) && (canChangeColorSpace()))
    {
        return;
    }
    deLayer* source = stack.getLayer(sourceLayer.getIndex());
    if (source)
    {
        deColorSpace c = source->getColorSpace();
        if (c != colorSpace.getColorSpace())
        {
            changeColorSpace(c);
        }
    }        
}   

void deLayer::registerProperty(deProperty& property)
{
    properties.push_back(&property);
}

deActionFrame* deLayer::createLayerFrame(wxWindow* parent, int layerNumber, deProject* project)
{
    deLayerFrame* frame = new deLayerFrame(parent, *this, project->getPreviewStack());
    deProperties::iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        frame->addProperty(*i);
    }

    frame->Fit();

    return frame;
}

void deLayer::notifyPropertiesOnColorSpaceChange()
{
    deProperties::iterator i;
    for (i = properties.begin(); i != properties.end(); i++)
    {
        (*i)->onColorSpaceChange();
    }
}

void deLayer::save(xmlNodePtr node)
{
    xmlNodePtr child = xmlNewChild(node, NULL, xmlCharStrdup("type"), NULL);
    xmlNodeSetContent(child, xmlCharStrdup(getType().c_str()));

    name.save(node, "name");
    colorSpace.save(node, "color_space");
    sourceLayer.save(node, "source_layer");
    overlayLayer.save(node, "overlay_layer");

    saveSpecific(node);
}

void deLayer::load(xmlNodePtr node)
{
    xmlNodePtr child = node->xmlChildrenNode;

    std::string colorSpaceString = "";
    std::string sourceLayerString = "";

    while (child)
    {
        if ((!xmlStrcmp(child->name, xmlCharStrdup("color_space")))) 
        {
            xmlChar* s = xmlNodeGetContent(child);            
            colorSpaceString = (char*)(s);
            xmlFree(s);
        }

        if ((!xmlStrcmp(child->name, xmlCharStrdup("source_layer")))) 
        {
            xmlChar* s = xmlNodeGetContent(child);            
            sourceLayerString = (char*)(s);
            xmlFree(s);
        }

        child = child->next;
    }

    changeColorSpace(colorSpaceFromString(colorSpaceString));

    std::istringstream iss(sourceLayerString);
    int s;
    iss >> s;

    changeSourceLayer(s);

    loadSpecific(node);
}
