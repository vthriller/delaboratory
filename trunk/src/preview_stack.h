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

#ifndef _DE_PREVIEW_STACK_H
#define _DE_PREVIEW_STACK_H

#include <vector>
#include <set>
#include <string>

class dePreview;
class deProject;

class dePreviewStack
{
    private:
        std::vector<dePreview*> previews;
        std::set<int> dirty;
        deProject* project;
        bool nowUpdating;
#ifdef DE_PROFILER    
        int previewsCalculated;
#endif        

        std::string error;

        void realUpdatePreviews();
        
    public:
        dePreviewStack();
        virtual ~dePreviewStack();

        dePreview* getPreview(int id);
        void addPreview();
        void removeTopPreview();

        int getSize() const;

        void updatePreviews(int n);

        void setProject(deProject* _project);
        deProject* getProject() {return project;};

        void refreshView();

        void setError(const std::string& _error);
        std::string getError();
};

#endif
