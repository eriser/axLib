/*******************************************************************************
 * Copyright (c) 2013 Alexandre Arsenault.
 *
 * This file is part of axLibrary.
 *
 * axLibrary is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axLibrary License.
 *
 * axLibrary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axLibrary. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axLibrary, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 ******************************************************************************/
#include "axVstCore.h"

#include "axMath.h"
#include  <unistd.h>

axVstCore::axVstCore() :
// Heritage.
axCore()
{
    _currentManagerIndex = 0;
}

void axVstCore::MainLoop()
{
    
}

void axVstCore::Init(const axPoint& frame_size)
{
    _size = frame_size;
    InitManagers();
    InitGL();
}

void axVstCore::InitManagers()
{
    axManager* winManager = new axManager();
    winManager->_managerName = std::string("WindowManager");
    
    axManager* winPopManager = new axManager();
    winPopManager->_managerName = std::string("PopupManager");
    
    axVstCoreData data;
    data.windowManager = winManager;
    data.popupManager = winPopManager;
    data.effect = nullptr;
    data.appDelegate = nullptr;
    _managersData.push_back(data);
}

axManager* axVstCore::GetWindowManager()
{
    if(_currentManagerIndex >= _managersData.size())
    {
        std::cerr << "SIZE ERROR" << std::endl;
    }
    return _managersData[_currentManagerIndex].windowManager;
}

axManager* axVstCore::GetPopupManager()
{
    if(_currentManagerIndex >= _managersData.size())
    {
        std::cerr << "SIZE ERROR" << std::endl;
    }
    return _managersData[_currentManagerIndex].popupManager;
}

void* axVstCore::GetCurrentAppDelegate()
{
    return _managersData[_currentManagerIndex].appDelegate;
}

void axVstCore::SetCurrentManagerIndex(const int& index)
{
    _currentManagerIndex = index;
}

axVstCoreData* axVstCore::GetVstCoreData()
{
    return &_managersData[_currentManagerIndex];
}

int axVstCore::GetCurrentManagerIndex() const
{
    return _currentManagerIndex;
}

int axVstCore::GetNumberOfManager() const
{
    return (int)_managersData.size();
}

string axVstCore::GetAppDirectory()
{
    return "";
}

bool axVstCore::CreatePopupWindow(const char* title, int width, int height)
{
    return false;
}

std::vector<axVstCoreData>* axVstCore::GetManagerVector()
{
    return &_managersData;
}

axRect axVstCore::GetScreenRect()
{
    // NEED TO CHANGE THIS !!!.
    return axRect(0, GetYTestValue(), _size.x, _size.y);
}

void axVstCore::ResizeGLScene(const int& width, const int& height, double y)
{
    int h = height;
    
    // Prevent a divide by zero by.
    if (h == 0)
    {
        h = 1;
    }
    
    _y_test = y;
    _size = axSize(width, h);
    
    // Reset the current viewport.
    //--------------------------------------------------
    //--------------------------------------------------
#ifdef __APPLE__
    glViewport(0, _y_test, width, h);
#else
    glViewport(0, 0, width, h);
#endif // __APPLE__
    //--------------------------------------------------
    //--------------------------------------------------
    
    glMatrixMode(GL_PROJECTION);
    
    axMatrix4 proj;
    axOrtho2D(proj.Identity().GetData(), _size);
    
    // Select the modelview matrix.
    glMatrixMode(GL_MODELVIEW);
    
    _needToDraw = true;
    _popupNeedToDraw = true;

    for(auto& n : _managersData)
    {
        n.windowManager->OnSize();
    }
}


void axVstCore::KillGLWindow()
{
    
}
void axVstCore::KillPopGLWindow()
{
    
}