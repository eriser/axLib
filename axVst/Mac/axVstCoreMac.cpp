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
#include "axVstCoreMac.h"

/// @todo Put in axLib core lib.
#include "axCocoaInterfaceMac.h"
#include  <unistd.h>

axVstCoreMac::axVstCoreMac() :
// Heritage.
axCore()
{
    _currentManagerIndex = 0;
}

void axVstCoreMac::MainLoop()
{
    
}

void axVstCoreMac::Init(const axPoint& frame_size)
{
    std::cout << "Init axVstCoreMac." << std::endl;
    InitManagers();
    
//    _windowManager = new axManager();
//    _windowManager->_managerName = std::string("WindowManager");
//    _popupManager = new axManager();
//    _popupManager->_managerName = std::string("PopupManager");
    
    InitGL();
}

void axVstCoreMac::InitManagers()
{
    std::cout << "axVstCoreMac::InitManagers()" << std::endl;
//    _windowManager = new axManager();
//    _windowManager->_managerName = std::string("WindowManager");
//    _popupManager = new axManager();
//    _popupManager->_managerName = std::string("PopupManager");
    
    axManager* winManager = new axManager();
    winManager->_managerName = std::string("WindowManager");
    
    axManager* winPopManager = new axManager();
    winPopManager->_managerName = std::string("PopupManager");
    
    _windowManagers.push_back(winManager);
    _popupManagers.push_back(winPopManager);
}

axManager* axVstCoreMac::GetWindowManager()
{
    return _windowManagers[_currentManagerIndex];
}

axManager* axVstCoreMac::GetPopupManager()
{
    return _popupManagers[_currentManagerIndex];
}

void axVstCoreMac::SetAppDelegateHandle(const int& index, void* handle)
{
    _appDelegateHandles.push_back(handle);
}

void* axVstCoreMac::GetCurrentAppDelegate()
{
    if(_appDelegateHandles.size())
    {
        return _appDelegateHandles[0];
    }
    else
    {
        return nullptr;
    }
    
}

int axVstCoreMac::GetCurrentManagerIndex() const
{
    return _currentManagerIndex;
}

string axVstCoreMac::GetAppDirectory()
{
    std::cout << std::string(CocoaGetAppDirectory() + std::string("/")) << std::endl;
    return std::string(CocoaGetAppDirectory() + std::string("/"));
}

bool axVstCoreMac::CreatePopupWindow(const char* title, int width, int height)
{
    return false;
}

axRect axVstCoreMac::GetScreenRect()
{
    // NEED TO CHANGE THIS !!!.
    return axRect(0, GetYTestValue(), _size.x, _size.y);
}


void axVstCoreMac::KillGLWindow()
{
    
}
void axVstCoreMac::KillPopGLWindow()
{
    
}

bool axVstCoreMac::CreateGLWindow(const char* title, int width, int height, int bits)
{
    return true;
}

void axVstCoreMac::UpdateAll()
{
    axCore::UpdateAll();
    TestFunctionInterface();
}

void axVstCoreMac::ResizeFrame(const axSize& size)
{
    axCocoaResizeFrame(size);
    ResizeGLScene(size.x, size.y);
}

std::string axVstCoreMac::OpenFileDialog()
{
    return axOpenFileDialog();
}

void axVstCoreMac::PushEventOnSystemQueue()
{
    AddEventToDispatchQueue();
}

void axVstCoreMac::HideMouse()
{
    axCocoaHideMouse();
}

void axVstCoreMac::ShowMouse()
{
    axCocoaShowMouse();
}
