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
#include "axMath.h"
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
//    std::cout << "Init axVstCoreMac." << std::endl;
    InitManagers();
    InitGL();
}

void axVstCoreMac::InitManagers()
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

axManager* axVstCoreMac::GetWindowManager()
{
    if(_currentManagerIndex >= _managersData.size())
    {
        std::cerr << "SIZE ERROR" << std::endl;
    }
    return _managersData[_currentManagerIndex].windowManager;
}

axManager* axVstCoreMac::GetPopupManager()
{
    if(_currentManagerIndex >= _managersData.size())
    {
        std::cerr << "SIZE ERROR" << std::endl;
    }
    return _managersData[_currentManagerIndex].popupManager;
}

void axVstCoreMac::SetAppDelegateHandle(const int& index, void* handle)
{
  
}

void* axVstCoreMac::GetCurrentAppDelegate()
{
    return _managersData[_currentManagerIndex].appDelegate;
}

void axVstCoreMac::SetCurrentManagerIndex(const int& index)
{
    _currentManagerIndex = index;
}

axVstCoreData* axVstCoreMac::GetVstCoreData()
{
    return &_managersData[_currentManagerIndex];
}

int axVstCoreMac::GetCurrentManagerIndex() const
{
    return _currentManagerIndex;
}

int axVstCoreMac::GetNumberOfManager() const
{
    return (int)_managersData.size();
}

void axVstCoreMac::ReInitApp()
{
    axReInitApp(_managersData[_currentManagerIndex].appDelegate);
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

int axVstCoreMac::DrawGLScene()
{
    if (_needToDraw == true)
    {
        _needToDraw = false;
        
        // Clear screen and depth buffer.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Set projection matrix.
        glMatrixMode(GL_PROJECTION);
        
        axMatrix4 proj;
        axOrtho2D(proj.Identity().GetData(), _size);
        
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // Draw black rectangle.
        glColor4d(0.0, 0.0, 0.0, 1.0);
        
        // Draw black rectangle.
        axFloatRect rect(-1.0, -1.0, _size.x * 2.0, _size.y * 2.0);
        axRectFloatPoints points = rect.GetPoints();
        GLubyte indices[] = {0, 1,2, 2,3};
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_DOUBLE, 0, &points);
        glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_BYTE, indices);
        glDisableClientState(GL_VERTEX_ARRAY);
        
        // Draw all windows.
        //		_windowManager->OnPaint();
        //        _popupManager->OnPaint();
        

//        for(auto& n : _managersData)
//        {
//            n.windowManager->OnPaint();
//            n.popupManager->OnPaint();
//            glFlush();
//        }
        GetWindowManager()->OnPaint();
        GetPopupManager()->OnPaint();
        
        
        return true;
    }
    return false;
}

void axVstCoreMac::ResizeGLScene(const int& width, const int& height, double y)
{
    int h = height;
    
    // Prevent a divide by zero by.
    if (h == 0)
    {
        h = 1;
    }
    
    //    std::cout << "Y test : " << y << std::endl;
    
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
    
    //_windowManager->OnSize();
//    GetWindowManager()->OnSize();
    for(auto& n : _managersData)
    {
        n.windowManager->OnSize();
    }
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
