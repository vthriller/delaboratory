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

#include "dodge_burn_frame.h"
#include "dodge_burn_layer.h"
#include <iostream>
#include "property_value_slider.h"
#include "property_boolean_ui.h"
#include "layer_processor.h"

deDodgeBurnFrame::deDodgeBurnFrame(wxWindow *parent, deActionLayer& _layer, deLayerProcessor& _layerProcessor, deLayerFrameManager& _frameManager)
:deActionFrame(parent, _layer, _frameManager), layerProcessor(_layerProcessor)
{
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    SetSizer(sizer);

    deDodgeBurnLayer& dodgeBurnLayer = dynamic_cast<deDodgeBurnLayer&>(_layer);

    wxSizer* sizerTop = new wxStaticBoxSizer(wxVERTICAL, this, _T(""));
    sizer->Add(sizerTop);

    int range = 400;

    radius = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyRadius(), dodgeBurnLayer, layerProcessor);
    sizerTop->Add(radius);

    alternate = new dePropertyBooleanUI(this, dodgeBurnLayer.getPropertyAlternate(), dodgeBurnLayer, layerProcessor);
    sizerTop->Add(alternate);


    int w = 80;
    int h = 25;


    wxSizer* sizerDodge = new wxStaticBoxSizer(wxVERTICAL, this, _T("dodge"));
    sizer->Add(sizerDodge);

    dodgeAmount = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyDodgeAmount(), dodgeBurnLayer, layerProcessor);
    sizerDodge->Add(dodgeAmount);

    dodgeMin = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyDodgeMin(), dodgeBurnLayer, layerProcessor);
    sizerDodge->Add(dodgeMin);

    dodgeMax = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyDodgeMax(), dodgeBurnLayer, layerProcessor);
    sizerDodge->Add(dodgeMax);

    wxSizer* sizerDB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizerDodge->Add(sizerDB, 0);

    dodgeLow = new wxButton(this, wxID_ANY, _T("low"), wxDefaultPosition, wxSize(w,h));
    sizerDB->Add(dodgeLow, 0);

    dodgeMedium = new wxButton(this, wxID_ANY, _T("medium"), wxDefaultPosition, wxSize(w,h));
    sizerDB->Add(dodgeMedium, 0);

    dodgeHigh = new wxButton(this, wxID_ANY, _T("high"), wxDefaultPosition, wxSize(w,h));
    sizerDB->Add(dodgeHigh, 0);

    dodgeNarrow = new wxButton(this, wxID_ANY, _T("narrow"), wxDefaultPosition, wxSize(w,h));
    sizerDB->Add(dodgeNarrow, 0);

    dodgeAverage = new wxButton(this, wxID_ANY, _T("average"), wxDefaultPosition, wxSize(w,h));
    sizerDB->Add(dodgeAverage, 0);

    dodgeWide = new wxButton(this, wxID_ANY, _T("wide"), wxDefaultPosition, wxSize(w,h));
    sizerDB->Add(dodgeWide, 0);



    wxSizer* sizerBurn = new wxStaticBoxSizer(wxVERTICAL, this, _T("burn"));
    sizer->Add(sizerBurn);

    burnAmount = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyBurnAmount(), dodgeBurnLayer, layerProcessor);
    sizerBurn->Add(burnAmount);

    burnMin = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyBurnMin(), dodgeBurnLayer, layerProcessor);
    sizerBurn->Add(burnMin);

    burnMax = new dePropertyValueSlider(this, range, dodgeBurnLayer.getPropertyBurnMax(), dodgeBurnLayer, layerProcessor);
    sizerBurn->Add(burnMax);

    wxSizer* sizerBB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizerBurn->Add(sizerBB, 0);

    burnLow = new wxButton(this, wxID_ANY, _T("low"), wxDefaultPosition, wxSize(w,h));
    sizerBB->Add(burnLow, 0);

    burnMedium = new wxButton(this, wxID_ANY, _T("medium"), wxDefaultPosition, wxSize(w,h));
    sizerBB->Add(burnMedium, 0);

    burnHigh = new wxButton(this, wxID_ANY, _T("high"), wxDefaultPosition, wxSize(w,h));
    sizerBB->Add(burnHigh, 0);

    burnNarrow = new wxButton(this, wxID_ANY, _T("narrow"), wxDefaultPosition, wxSize(w,h));
    sizerBB->Add(burnNarrow, 0);

    burnAverage = new wxButton(this, wxID_ANY, _T("average"), wxDefaultPosition, wxSize(w,h));
    sizerBB->Add(burnAverage, 0);

    burnWide = new wxButton(this, wxID_ANY, _T("wide"), wxDefaultPosition, wxSize(w,h));
    sizerBB->Add(burnWide, 0);


    wxSizer* sizerB = new wxStaticBoxSizer(wxHORIZONTAL, this, _T(""));
    sizer->Add(sizerB, 0);

    reset = new wxButton(this, wxID_ANY, _T("reset"), wxDefaultPosition, wxSize(100,25));
    sizerB->Add(reset, 0);

    Fit();

    Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(deDodgeBurnFrame::click));
}

deDodgeBurnFrame::~deDodgeBurnFrame()
{
}

void deDodgeBurnFrame::click(wxCommandEvent &event)
{
    int id = event.GetId();
    deDodgeBurnLayer& dodgeBurnLayer = dynamic_cast<deDodgeBurnLayer&>(layer);

    if (reset->GetId() == id)
    {
        dodgeBurnLayer.reset();
    }      

    if (dodgeLow->GetId() == id)
    {
        dodgeBurnLayer.setDodge(0.3);
    }      

    if (dodgeMedium->GetId() == id)
    {
        dodgeBurnLayer.setDodge(0.6);
    }      

    if (dodgeHigh->GetId() == id)
    {
        dodgeBurnLayer.setDodge(0.9);
    }      

    if (dodgeNarrow->GetId() == id)
    {
        dodgeBurnLayer.setDodgeRange(0.7, 0.95);
    }      

    if (dodgeAverage->GetId() == id)
    {
        dodgeBurnLayer.setDodgeRange(0.5, 0.95);
    }      

    if (dodgeWide->GetId() == id)
    {
        dodgeBurnLayer.setDodgeRange(0.2, 0.95);
    }      

    if (burnLow->GetId() == id)
    {
        dodgeBurnLayer.setBurn(0.3);
    }      

    if (burnMedium->GetId() == id)
    {
        dodgeBurnLayer.setBurn(0.6);
    }      

    if (burnHigh->GetId() == id)
    {
        dodgeBurnLayer.setBurn(0.9);
    }      

    if (burnNarrow->GetId() == id)
    {
        dodgeBurnLayer.setBurnRange(0.05, 0.3);
    }      

    if (burnAverage->GetId() == id)
    {
        dodgeBurnLayer.setBurnRange(0.05, 0.5);
    }      

    if (burnWide->GetId() == id)
    {
        dodgeBurnLayer.setBurnRange(0.05, 0.8);
    }      


    radius->setFromProperty();
    alternate->setFromProperty();
    dodgeAmount->setFromProperty();
    dodgeMin->setFromProperty();
    dodgeMax->setFromProperty();
    burnAmount->setFromProperty();
    burnMin->setFromProperty();
    burnMax->setFromProperty();

    int index = layer.getIndex();
    layerProcessor.markUpdateAllChannels(index);
}   
