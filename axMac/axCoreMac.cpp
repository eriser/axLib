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
    axCallNSApplicationMain();
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
    return std::string(CocoaGetAppDirectory() + std::string("/"));
}

//bool axCoreMac::CreatePopupWindow(const char* title, int width, int height)
//{
//    return false;
//}

axRect axCoreMac::GetScreenRect()
{
    return axCocoaGetScreenSize();
}


void axCoreMac::KillGLWindow()
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
    axCocoaResizeFrame(size);
    ResizeGLScene(size);
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
