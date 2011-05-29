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

#include "layer_list_panel.h"
#include "project.h"
#include "layer.h"
#include "add_layer_dialog.h"
#include <sstream>
#include "exception.h"

void deLayerListPanel::select(wxCommandEvent &event)
{
    wxListEvent& e = dynamic_cast<wxListEvent&>(event);
    buttonView->Enable();

    selected = listbox->GetItemCount() - e.GetIndex() - 1;

}

void deLayerListPanel::deselect(wxCommandEvent &event)
{
    buttonView->Disable();

}

void deLayerListPanel::activate(wxCommandEvent &event)
{
    wxListEvent& e = dynamic_cast<wxListEvent&>(event);
    int i = listbox->GetItemCount() - e.GetIndex() - 1;

    const deLayerStack& layerStack = project->getLayerStack();
    wxDialog* layerDialog = layerStack.getLayer(i)->createDialog(this, i, project);
    if (!layerDialog)
    {
        throw deException("failed creating layer dialog");
    }
    layerDialog->ShowModal();

}

void deLayerListPanel::right_click(wxCommandEvent &event)
{

}

void deLayerListPanel::clickAdd(wxCommandEvent &event)
{
    buttonView->Disable();

    try
    {

        wxFrame* addLayerFrame = new deAddLayerFrame(this, project);
        addLayerFrame->Show();
    }
    catch (deException e)
    {
    }
}

void deLayerListPanel::clickRemove(wxCommandEvent &event)
{
    buttonView->Disable();

    project->removeTopLayer();
    project->getGUI().refreshView();
}

void deLayerListPanel::clickView(wxCommandEvent &event)
{
    try
    {
        project->setView(selected);
    }
    catch (deException& e)
    {
    }
}

deLayerListPanel::deLayerListPanel(wxWindow* parent, deProject* _project)
:wxPanel(parent), project(_project)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    listbox = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(256,200), wxLC_REPORT);

    Connect(listbox->GetId(), wxEVT_COMMAND_LIST_ITEM_SELECTED, wxCommandEventHandler(deLayerListPanel::select));
    Connect(listbox->GetId(), wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxCommandEventHandler(deLayerListPanel::deselect));
    Connect(listbox->GetId(), wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxCommandEventHandler(deLayerListPanel::activate));
    Connect(listbox->GetId(), wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxCommandEventHandler(deLayerListPanel::right_click));

    sizer->Add(listbox, 1, wxEXPAND);

    wxSizer* buttonsSizer = new wxBoxSizer(wxHORIZONTAL);

    buttonAdd = new wxButton(this, wxID_ANY, _T("add layer"));
    Connect(buttonAdd->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deLayerListPanel::clickAdd));
    buttonsSizer->Add(buttonAdd, 1, wxEXPAND);

    buttonRemove = new wxButton(this, wxID_ANY, _T("remove layer"));
    Connect(buttonRemove->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deLayerListPanel::clickRemove));
    buttonsSizer->Add(buttonRemove, 1, wxEXPAND);
    buttonRemove->Disable();

    buttonView = new wxButton(this, wxID_ANY, _T("set view"));
    Connect(buttonView->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deLayerListPanel::clickView));
    buttonsSizer->Add(buttonView, 1, wxEXPAND);
    buttonView->Disable();

    sizer->Add(buttonsSizer, 0, wxEXPAND);

    SetSizer(sizer);

    project->getGUI().setLayerListPanel(this);


}

deLayerListPanel::~deLayerListPanel()
{
}

void deLayerListPanel::update()
{
    listbox->ClearAll();

    const deLayerStack& layerStack = project->getLayerStack();

    unsigned int n = layerStack.getSize();

    wxListItem column0;
    wxListItem column1;
    wxListItem column2;
    wxListItem column3;

    column0.SetText(_T("id"));
    column0.SetId(0);
    column0.SetWidth(30);

    column1.SetText(_T("name"));
    column1.SetId(1);
    column1.SetWidth(120);

    column2.SetText(_T("colorspace"));
    column2.SetId(2);
    column2.SetWidth(80);

    column3.SetText(_T("view"));
    column3.SetId(2);
    column3.SetWidth(40);
 
    listbox->InsertColumn(0, column0);
    listbox->InsertColumn(1, column1);
    listbox->InsertColumn(2, column2);
    listbox->InsertColumn(3, column3);

    unsigned int i;
    for (i = 0; i < n; i++)
    {
        int index = n - i - 1;

        const std::string layerName = layerStack.getLayer(index)->getName();
        const std::string colorSpaceName = getColorSpaceName(layerStack.getLayer(index)->getColorSpace());

        wxListItem item;
        item.SetId(i);

        listbox->InsertItem(item);

        std::ostringstream oss;
        oss << index;

        listbox->SetItem(i, 0, wxString::FromAscii(oss.str().c_str()));
        listbox->SetItem(i, 1, wxString::FromAscii(layerName.c_str()));
        listbox->SetItem(i, 2, wxString::FromAscii(colorSpaceName.c_str()));
        if (project->getVisibleLayerID() == index)
        {
            listbox->SetItem(i, 3, _T("*"));
        }
        else
        {
            listbox->SetItem(i, 3, _T(" "));
        }
    }

    if (n < 2)
    {
        buttonRemove->Disable();
    }
    else
    {
        buttonRemove->Enable();
    }
}
