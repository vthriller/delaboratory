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

#include "property.h"
#include "layer.h"
#include "preview_stack.h"
#include "gui.h"
#include "project.h"

deProperty::deProperty(deLayer& _parent)
:parent(_parent)
{
    stack = NULL;
    parent.registerProperty(*this);
}

deProperty::~deProperty()
{
}

wxPanel* deProperty::getPanel(wxWindow* parent, dePreviewStack& _stack)
{
    stack = &_stack;

    wxPanel* panel = new wxPanel(parent);
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    addPanelContent(panel, sizer);

    panel->SetSizer(sizer);

    return panel;
}

/*
void deProperty::addPanelContent(wxPanel* panel, wxSizer* sizer, dePreviewStack& _stack)
{
    wxStaticText* label = new wxStaticText(panel, wxID_ANY, _T("unsupported property") );
    sizer->Add(label, 0);
}
*/

void deProperty::onUpdate()
{
    if (stack)
    {
        stack->updatePreviews(parent.getIndex());
        stack->refreshView();
    }
}

void deProperty::onListUpdate()
{
    deColorSpace colorSpace = parent.getColorSpace();
    deGUI* gui = &(stack->getProject()->getGUI());
    if (gui)
    {
        gui->updateAfterSetView(colorSpace);
    } 
    parent.notifyPropertiesOnColorSpaceChange();
    onUpdate();
}

void deProperty::setStack(dePreviewStack& _stack)
{
    stack = &_stack;
}

void deProperty::onColorSpaceChange()
{
}
