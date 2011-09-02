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

#include "add_layer_dialog.h"
#include "layer_list_panel.h"
#include "layer.h"
#include "project.h"
#include "exception.h"
#include "preview.h"
#include "layer_factory.h"

void deAddLayerFrame::click(wxCommandEvent &event)
{
    try
    {
        deLayerFactory& factory = project->getLayerFactory();

        Close();

        deLayerStack& stack = project->getLayerStack();

        int index = stack.getSize();

        int id = event.GetId();

        std::string type = "";
    
        std::map<int, std::string>::iterator i = buttons.find(id);
        if (i == buttons.end())
        {
            return;
        }

        type = i->second;

        std::string name = type;

        deLayer* layer = factory.createLayer(type, stack, index, name);

        if (layer)
        {
            layer->changeSourceLayer(project->getVisibleLayerID());
            project->addLayer(layer);

        }         

    }
    catch (deException& e)
    {
    }
}

deAddLayerFrame::deAddLayerFrame(deLayerListPanel* _panel, deProject* _project)
:wxFrame(_panel, wxID_ANY, _T("add layer"), wxDefaultPosition, wxDefaultSize), panel(_panel), project(_project)
{
    deLayer* source = project->getVisibleLayer();
    std::vector<deColorSpace> convertColorSpaces;
    if (source)
    {
        deColorSpace currentColorSpace = source->getColorSpace();
        getSupportedConversions(currentColorSpace, convertColorSpaces);
    }

    std::vector<deColorSpace> colorSpaces;
    getSupportedColorSpaces(colorSpaces);

    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    deLayerFactory& factory = project->getLayerFactory();
    std::list<std::string> layers;
    factory.getSupportedLayers(layers);

    std::list<std::string>::const_iterator i;
    for (i = layers.begin(); i != layers.end(); i++)
    {
        wxButton* button = new wxButton(this, wxID_ANY, wxString::FromAscii((*i).c_str()), wxDefaultPosition, wxSize(-1, 50));
        sizer->Add(button, 1, wxEXPAND);
        buttons[button->GetId()] = *i;
    }

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deAddLayerFrame::click));

    SetSizer(sizer);

    Fit();
}

deAddLayerFrame::~deAddLayerFrame()
{
}
