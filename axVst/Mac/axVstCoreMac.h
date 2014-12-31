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

#include "axCore.h"
#include "audioeffectx.h"

struct axVstCoreData
{
    axManager* windowManager;
    axManager* popupManager;
    void* appDelegate;
    AudioEffect* effect;
};

class axVstCoreMac : public axCore
{
public:
    axVstCoreMac();
    
    virtual void MainLoop();
    
    virtual void InitManagers();
    
    void ReInitApp();
    
    virtual axManager* GetWindowManager();
    
    virtual axManager* GetPopupManager();
    
    virtual void Init(const axPoint& frame_size);

    virtual string GetAppDirectory();
    
    virtual bool CreatePopupWindow(const char* title, int width, int height);

    virtual axRect GetScreenRect();
    
    virtual string OpenFileDialog();
    
    virtual void PushEventOnSystemQueue();

    virtual void KillGLWindow();
    
    virtual void KillPopGLWindow();
    
    virtual bool CreateGLWindow(const char* title,
                                int width, int height, int bits);
    
    void ResizeGLScene(const int& width, const int& height, double y = 0);
    
    virtual void UpdateAll();
    
    virtual void ResizeFrame(const axSize& size);
    
    virtual void HideMouse();
    
    virtual void ShowMouse();
    
    virtual int DrawGLScene();
    
    int GetCurrentManagerIndex() const;
    void SetCurrentManagerIndex(const int& index);
    
//    int GetCurrentManagerIndex() const;
    
    void SetAppDelegateHandle(const int& index, void* handle);
    
    axVstCoreData* GetVstCoreData();
    
    int GetNumberOfManager() const;
    
    void* GetCurrentAppDelegate();
    
    std::vector<axVstCoreData>* GetManagerVector()
    {
        return &_managersData;
    }
    
private:
    int _currentManagerIndex;

    std::vector<axVstCoreData> _managersData;
};

#endif // __APPLE__

#endif // __AX_VST_CORE_MAC__
