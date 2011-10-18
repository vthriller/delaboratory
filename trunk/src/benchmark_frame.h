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

#ifndef _DE_BENCHMARK_FRAME_H
#define _DE_BENCHMARK_FRAME_H

#include "help_frame.h"
#include "benchmarks.h"

class deBenchmarkFrame:public deHelpFrame
{
    private:
        wxSizer* sizer;
        std::vector<deBenchmark*> benchmarks;
        std::vector<wxStaticText*> results;

        wxStaticText* total;

        void addBenchmark(const std::string& s, std::string name, int value);
        void addBenchmarkBlur(deSize size, deValue radius, deBlurType type);
        void addBenchmarkBlurs( deSize size, deBlurType type);
        void addBenchmarkColor();
        void addBenchmarkColor(deColorSpace colorSpace);

    public:
        deBenchmarkFrame(wxWindow *parent, const std::string& type);
        virtual ~deBenchmarkFrame();

        void perform();

};

#endif
