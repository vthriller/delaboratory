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
class deSamplersPanel;
class deSamplerManager;
class deZoomManager;
class deThreadsPanel;
class deOperationProcessor;
class deChannelManager;
class deWarningPanel;
#include "size.h"
#include "main_frame_events.h"
#include "gui.h"

class deMainFrame: public wxFrame
{
private:
    deProject& project;
    wxPanel* hPanel;
    wxPanel* topPanel;
    deLayerGridPanel* layerGridPanel;
    deSamplersPanel* samplersPanel;
    deControlPanel* controlPanel;
    wxSizer* mainSizer;
    deImageAreaPanel* imageAreaPanel;
    deHistogramPanel* histogramPanel;
    deThreadsPanel* threadsPanel;
    deWarningPanel* warningPanel;
    deGUI& gui;

    std::string imageName;

    deLayerProcessor& layerProcessor;

    deSize imageSize;

    std::string openDirectory;
    std::string saveDirectory;

    bool full;

	void onTestImageSmall(wxCommandEvent& event);
	void onTestImageBig(wxCommandEvent& event);
	void onOpenImage(wxCommandEvent& event);
	void onOpenRawImageProPhoto(wxCommandEvent& event);
	void onOpenRawImageRGB(wxCommandEvent& event);
	void onOpenRawImageProPhotoAB(wxCommandEvent& event);
	void onOpenRawImageRGBAB(wxCommandEvent& event);
	void onQuit(wxCommandEvent& event);
	void onNewProject(wxCommandEvent& event);
	void onHelpColorSpaces(wxCommandEvent& event);
	void onHelpColorSpaces2(wxCommandEvent& event);
	void onHelpColorSpaces3(wxCommandEvent& event);
	void onHelpColorSpaces4(wxCommandEvent& event);
	void onHelpColorSpaces5(wxCommandEvent& event);
	void onLABColors1(wxCommandEvent& event);
	void onLABColors2(wxCommandEvent& event);
	void onLABColors5(wxCommandEvent& event);
	void onColorMatrix1(wxCommandEvent& event);
	void onColorMatrix2(wxCommandEvent& event);
	void onColorMatrix3(wxCommandEvent& event);
	void onColorMatrix4(wxCommandEvent& event);
	void onColorMatrix5(wxCommandEvent& event);
	void onRepaintEvent(wxCommandEvent& event);
	void onImageLoadEvent(wxCommandEvent& event);
	void onInfoEvent(wxCommandEvent& event);
	void onWarningEvent(wxCommandEvent& event);
    void onHistogramEvent(wxCommandEvent& event);
    void onTimerEvent(wxTimerEvent& event);
    void onExportGIMP(wxCommandEvent& event);
    void onExportTIFF(wxCommandEvent& event);
    void onExportJPG(wxCommandEvent& event);
    void onExportAll(wxCommandEvent& event);

	void onCloseEvent(wxCloseEvent& event);

    void repaintMainFrame(bool calcHistogram);

    void check(wxCommandEvent &event);

    void updateTitle();

    void paintHistogram();

    bool generateFinalImage(const std::string& app, const std::string& type, const std::string& name, bool saveAll, const std::string& dir);

public:
	deMainFrame(const wxSize& size, deProject& _project, deLayerProcessor& _layerProcessor, deSamplerManager& _samplerManager, deZoomManager& _zoomManager, const std::string& dcrawVersion, deOperationProcessor& _operationProcessor, deChannelManager& channelManager, deGUI& _gui);
	~deMainFrame();

    void rebuild();

    void onKey(int key);

    void generateHistogram();
    void updateWarning();

    void setImageName(const std::string& _imageName, const deSize& _size);
    void forceUpdateSize();

	DECLARE_EVENT_TABLE()

};


#endif
