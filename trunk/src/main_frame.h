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

#ifndef _DE_MAIN_FRAME_H
#define _DE_MAIN_FRAME_H

#include "wx/wx.h"
class deProject;
class deImageAreaPanel;
class deLayerGridPanel;
class deControlPanel;
class deHistogramPanel;
class deLayerProcessor;

class deMainFrame: public wxFrame
{
private:
    deProject& project;
    wxPanel* hPanel;
    wxPanel* topPanel;
    deLayerGridPanel* layerGridPanel;
    deControlPanel* controlPanel;
    wxSizer* mainSizer;
    deImageAreaPanel* imageAreaPanel;
    deHistogramPanel* histogramPanel;
    bool full;

	void onTestImageSmall(wxCommandEvent& event);
	void onTestImageBig(wxCommandEvent& event);
	void onOpenImage(wxCommandEvent& event);
	void onQuit(wxCommandEvent& event);
	void onNewProject(wxCommandEvent& event);
	void onOpenProject(wxCommandEvent& event);
	void onSaveProject(wxCommandEvent& event);
	void onOpenLayerStack(wxCommandEvent& event);
	void onSaveLayerStack(wxCommandEvent& event);
	void onHelpColorSpaces(wxCommandEvent& event);
	void onHelpColorSpaces2(wxCommandEvent& event);
	void onHelpColorSpaces3(wxCommandEvent& event);
	void onHelpColorSpaces4(wxCommandEvent& event);
	void onHelpColorSpaces5(wxCommandEvent& event);
	void onLABColors1(wxCommandEvent& event);
	void onLABColors2(wxCommandEvent& event);
	void onLABColors5(wxCommandEvent& event);
	void onMemoryInfo(wxCommandEvent& event);
	void onBenchmarkBlur(wxCommandEvent& event);
	void onBenchmarkColor(wxCommandEvent& event);
	void onRepaintEvent(wxCommandEvent& event);

	void test(wxCommandEvent& event);

public:
	deMainFrame(const wxSize& size, deProject& _project, deLayerProcessor& _layerProcessor);

    void showPanels();
    void hidePanels();
    void rebuild();

    void onKey(int key);

    void repaintMainFrame(bool calcHistogram);

	DECLARE_EVENT_TABLE()

};


#endif
