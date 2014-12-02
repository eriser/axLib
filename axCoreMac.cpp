//
//  axCore.cpp
//  test
//
//  Created by Caroline Ross on 2014-04-24.
//  Copyright (c) 2014 alexarse. All rights reserved.
//

#include "axCoreMac.h"

/// @todo Put in axLib core lib.
#include "axCocoaInterfaceMac.h"

axCoreMac::axCoreMac()
{
    
}

void axCoreMac::MainLoop()
{
    
}

void axCoreMac::Init(const axPoint& frame_size)
{
    std::cout << "Init axCoreMac." << std::endl;
    InitGL();
}

string axCoreMac::GetAppDirectory()
{
    return std::string(CocoaGetAppDirectory() + std::string("/"));
}

bool axCoreMac::CreatePopupWindow(char* title, int width, int height)
{
    return false;
}

axRect axCoreMac::GetScreenRect()
{
    // NEED TO CHANGE THIS !!!.
    return axRect(0, GetYTestValue(), _size.x, _size.y);
}


void axCoreMac::KillGLWindow()
{
    
}
void axCoreMac::KillPopGLWindow()
{
    
}

bool axCoreMac::CreateGLWindow(char* title, int width, int height, int bits)
{
    return true;
}

void axCoreMac::UpdateAll()
{
    axCore::UpdateAll();
    TestFunctionInterface();
}

std::string axCoreMac::OpenFileDialog()
{
    return axOpenFileDialog();
}
