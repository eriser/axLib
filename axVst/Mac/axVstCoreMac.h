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
#ifndef __AX_VST_CORE_MAC__
#define __AX_VST_CORE_MAC__

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/glu.h>

#include "axVstCore.h"




//virtual axRect GetScreenRect() = 0;
//
//virtual axSize GetScreenSize() = 0;
//
//virtual string OpenFileDialog() = 0;
//
//virtual void PushEventOnSystemQueue() = 0;
//
//virtual void ResizeFrame(const axSize& size) = 0;
//
//virtual void KillGLWindow() = 0;
class axVstCoreMac : public axVstCore
{
public:
    axVstCoreMac();
    
    virtual void MainLoop();
    
    virtual void ReInitApp();

    virtual string GetAppDirectory();
    
    virtual bool CreatePopupWindow(const char* title, int width, int height);

    virtual axRect GetScreenRect();
    
    virtual axSize GetScreenSize()
    {
        return axSize(100, 100);
    }
    
    virtual string OpenFileDialog();
    
    virtual void PushEventOnSystemQueue();

    virtual void KillGLWindow();
    
    virtual void KillPopGLWindow();
    
    virtual bool CreateGLWindow(const char* title,
                                int width, int height, int bits);
    
    virtual void UpdateAll();
    
    virtual void ResizeFrame(const axSize& size);
    
    virtual void HideMouse();
    
    virtual void ShowMouse();
    
    virtual void SetAppDelegateHandle(const int& index, void* handle);
};

#endif // __APPLE__

#endif // __AX_VST_CORE_MAC__
