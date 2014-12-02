//
//  axCore.h
//  test
//
//  Created by Caroline Ross on 2014-04-24.
//  Copyright (c) 2014 alexarse. All rights reserved.
//

#ifndef test_axCore_h
#define test_axCore_h

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>

#include "axCore.h"

class axCoreMac : public axCore
{
public:
    axCoreMac();
    
    virtual void MainLoop();
    
    virtual void Init(const axPoint& frame_size);

    virtual string GetAppDirectory();
    
    virtual bool CreatePopupWindow(char* title, int width, int height);

    virtual axRect GetScreenRect();
    
    virtual string OpenFileDialog();

    virtual void KillGLWindow();
    
    virtual void KillPopGLWindow();
    
    virtual bool CreateGLWindow(char* title, int width, int height, int bits);
    
    virtual void UpdateAll();
};

#endif // __APPLE__

#endif
