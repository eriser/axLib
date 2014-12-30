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
#ifndef __AX_CORE__
#define __AX_CORE__

/// @defgroup Core
/// @{

#include "axC++.h"
#include "axManager.h"
#include <time.h>

class axCore
{
public:
	axCore();
	virtual void MainLoop() = 0;
	virtual void Init(const axPoint& frame_size) = 0;

	virtual void ResizeGLScene(const int& width, const int& height, double y = 0);
	virtual axManager* GetWindowManager();
	virtual axManager* GetPopupManager();
	virtual string GetAppDirectory() = 0;

    
	virtual bool CreatePopupWindow(const char* title, int width, int height) = 0;

	axSize GetGlobalSize() const;
    
    void SetGlobalSize(const axSize& size);
	virtual axRect GetScreenRect() = 0;
	virtual void UpdateAll();

	virtual string OpenFileDialog() = 0;

	void DeletePopWindow();
    
    virtual void PushEventOnSystemQueue() = 0;

    virtual void ResizeFrame(const axSize& size) = 0;
    
    virtual void HideMouse(){}
    virtual void ShowMouse(){}

	int InitGL();
	void ResizePopGLScene(const int& width, const int& height);
	virtual int DrawGLScene();
	virtual int DrawGLPopScene();
	virtual void KillGLWindow() = 0;
	virtual void KillPopGLWindow() = 0;
	virtual bool CreateGLWindow(const char* title, int width, int height, int bits) = 0;
    
    double GetYTestValue()
    {
        return _y_test;
    }

protected:
	axManager* _windowManager;
	axManager* _popupManager;

	bool _needToDraw, _popupNeedToDraw;
	axSize _size, _popSize;
    double _y_test;
    
    virtual void InitManagers();
};

/// @}
#endif //__AX_CORE__

