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

#ifndef _DE_OPERATION_PROCESSOR_H
#define _DE_OPERATION_PROCESSOR_H

class deLayerProcessor;
class deBaseLayer;
class deProject;
#include <string>
#include <vector>
#include "color_space.h"

class deOperationProcessor
{
    private:
        deLayerProcessor& layerProcessor;
        deProject& project;

        void removeTopLayer();
        void addNewLayerOnTop(deBaseLayer* layer);

        bool tryExecuteBasicOperation(const std::string& operation);
        bool executeOperation(deColorSpace colorSpace, const std::string& operation);

    public:
        deOperationProcessor(deLayerProcessor& _layerProcessor, deProject& _project);
        virtual ~deOperationProcessor();

        void execute(const std::string& operation);

        void initProfile(const std::string& profile);

};

void getSupportedBasicOperations(std::vector<std::string>& actions);

#endif
