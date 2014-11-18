//
//  axCore.h
//  test
//
//  Created by Caroline Ross on 2014-04-24.
//  Copyright (c) 2014 alexarse. All rights reserved.
//

#ifndef test_axCore_h
#define test_axCore_h

#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>

#include "axCore.h"




class axCoreMac : public axCore
{
public:
    axCoreMac()
    {
        
    }
    virtual void MainLoop()
    {
        
    }
    
    virtual void Init(const axPoint& frame_size)
    {
        
    }

    virtual string GetAppDirectory()
    {
        return "";
    }
    
    virtual bool CreatePopupWindow(char* title, int width, int height)
    {
        return false;
    }

    virtual axRect GetScreenRect()
    {
        // NEED TO CHANGE THIS !!!.
        return axRect(0, GetYTestValue(), _size.x, _size.y);
    }
    
    virtual string OpenFileDialog();

    virtual void KillGLWindow()
    {
        
    }
    virtual void KillPopGLWindow()
    {
        
    }
    
    virtual bool CreateGLWindow(char* title, int width, int height, int bits)
    {
        return true;
    }
    
    virtual void UpdateAll();

};

#endif
