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
#include "axCoreMac.h"

/// @todo Put in axLib core lib.
#include "axCocoaInterfaceMac.h"

#include  <unistd.h>

axCoreMac::axCoreMac()
{
    
}

void axCoreMac::MainLoop()
{
    
}

void axCoreMac::Init(const axPoint& frame_size)
{
    std::cout << "Init axCoreMac." << std::endl;
    InitManagers();
    InitGL();
}

axSize axCoreMac::GetScreenSize()
{
    return axCocoaGetScreenSize();
}

string axCoreMac::GetAppDirectory()
{
    
    //    char buf[1024];
//    getcwd(buf, size_t(1024));
////    readlink("/proc/self/exe", buf, sizeof(buf)-1);
//    string path(buf);
////    //path = path.substr(0, path.find_last_of("/"));
////    //path.push_back('/');
//    std::cout << "Path : " << path << std::endl;

//    std::cout << std::string(CocoaGetAppDirectory() + std::string("/")) << std::endl;
    return std::string(CocoaGetAppDirectory() + std::string("/"));
}

bool axCoreMac::CreatePopupWindow(const char* title, int width, int height)
{
    return false;
}

axRect axCoreMac::GetScreenRect()
{
    // NEED TO CHANGE THIS !!!.
//    return axRect(0, GetYTestValue(), _size.x, _size.y);
    return axCocoaGetScreenSize();
}


void axCoreMac::KillGLWindow()
{
    
}
void axCoreMac::KillPopGLWindow()
{
    
}

bool axCoreMac::CreateGLWindow(const char* title, int width, int height, int bits)
{
    return true;
}

void axCoreMac::UpdateAll()
{
    axCore::UpdateAll();
    TestFunctionInterface();
}

void axCoreMac::ResizeFrame(const axSize& size)
{
//    ResizeGLScene(size.x, size.y);
    axCocoaResizeFrame(size);
    ResizeGLScene(size.x, size.y);
    
}

std::string axCoreMac::OpenFileDialog()
{
    return axOpenFileDialog();
}

void axCoreMac::PushEventOnSystemQueue()
{
    AddEventToDispatchQueue();
}

void axCoreMac::HideMouse()
{
    axCocoaHideMouse();
}

void axCoreMac::ShowMouse()
{
    axCocoaShowMouse();
}
