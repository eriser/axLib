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
#ifndef __AX_APP__
#define __AX_APP__

/// @defgroup Core
/// @{

#ifdef __linux__
#include "axCoreX11.h"
#endif //__linux__

#ifdef _MSC_VER
	#ifdef _axWxWidgetsCore_
		#include "axCoreWin32.h"
	#else
		#include "axCoreWxWidgets.h"
	#endif // _axWxWidgetsCore_.
#endif //_MSC_VER

#ifdef __APPLE__
    #ifdef _AX_VST_APP_
        #include "axVstCoreMac.h"
    #else
        #include "axCoreMac.h"
    #endif // _AX_VST_APP_
#endif // __APPLE__


#include "axC++.h"
#include "axResourceManager.h"

// Is use as an adapter to global axCore class (axCORE).
class axApp
{
public:
    #pragma message("WARNING: Should use GetInstance.")
	static axApp* MainInstance;
	axApp();

	inline static axApp* GetInstance()
	{
		return MainInstance;
	}

	inline static axApp* CreateApp()
	{
		return MainInstance == nullptr ?
               MainInstance = new axApp() : MainInstance;
	}

	inline static axApp* CreateApp(const axSize& frame_size)
	{
		return MainInstance == nullptr ?
               MainInstance = new axApp(frame_size) : MainInstance;
	}
    
    inline static std::string GetAppPath()
    {
        return GetInstance()->GetCore()->GetAppDirectory();
    }

	axApp(const axSize& frame_size);

	void MainLoop();

    
    
//	void CreatePopupWindow(const axSize& size);

	axManager* GetWindowManager();
    axManager* GetPopupManager();

	void AddWindow(axWindow* win);
    
	void AddPopWindow(axWindow* win);

	void UpdateAll();

	axCore* GetCore();

	string OpenFileDialog();

	bool CreatePopupWindow(const char*, int, int);
    
    #pragma message("WARNING: Deprecate.")
	string GetCurrentAppDirectory();

    std::string GetAppDirectory();
    
    axResourceManager* GetResourceManager() const;
    
    void ActivateDebugEditor(const bool& active);
    bool IsDebugEditorActive() const;
    
    void CallMainEntryFunction();
    void AddMainEntry(std::function<void()> fct);
    
    void AddAfterGUILoadFunction(std::function<void()> fct);
    void CallAfterGUILoadFunction();
    
private:
	axCore* _core;
    
    std::function<void()> _mainEntryFunction, _afterGuiLoadFunction;
    static axResourceManager* _resourceManager;
    
    bool _debugEditorActive;
    
    
    axEVENT_ACCESSOR(axMsg, OnDebugEditor);
    void OnDebugEditor(const axMsg& msg);
};

/// @}
#endif //__AX_APP__