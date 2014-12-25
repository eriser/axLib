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
#include "axApp.h"

axApp* axApp::MainInstance = nullptr;

axApp::axApp()
{
#ifdef __linux__
	_core = new axCoreX11(this);
	_core->Init(axSize(0, 0));
#endif //__linux__


#ifdef _MSC_VER
	_core = new axCoreWin32();
	axCORE = _core;
	_core->Init(axSize(0, 0));
#endif //_MSC_VER
    
    
#ifdef __APPLE__
    
#ifdef _AX_VST_APP_
    _core = new axVstCoreMac();
    _core->Init(axSize(1000, 717));
#else
    _core = new axCoreMac();
    _core->Init(axSize(1000, 717));
#endif // _AX_VST_APP_
    
#endif // __APPLE__
    
}

axApp::axApp(const axSize& frame_size)
{
#ifdef __linux__
	_core = new axCoreX11(this);
	_core->Init(frame_size);
#endif //__linux__

#ifdef _MSC_VER
	_core = new axCoreWin32();
	axCORE = _core;
	_core->Init(frame_size);
#endif // _MSC_VER
    
#ifdef __APPLE__
    
#ifdef _AX_VST_APP_
    _core = new axVstCoreMac();
    _core->Init(frame_size);
#else
    _core = new axCoreMac();
    _core->Init(frame_size);
#endif // _AX_VST_APP_
    
#endif // __APPLE__
}

void axApp::CreatePopupWindow(const axSize& size)
{
#ifdef __linux__
	axCore* c = new axCoreX11(this);
	c->Init(size);
#endif //__linux__
}

string axApp::OpenFileDialog()
{
	return _core->OpenFileDialog();
}

bool axApp::CreatePopupWindow(const char* title, int x, int y)
{
	return _core->CreatePopupWindow("Popup", x, y);
}

string axApp::GetAppDirectory()
{
	return _core->GetAppDirectory();
}

string axApp::GetCurrentAppDirectory()
{
#ifdef __linux__
	char buf[1024];
	readlink("/proc/self/exe", buf, sizeof(buf)-1);
	string path(buf);
	path = path.substr(0, path.find_last_of("/"));
	path.push_back('/');
	return path;
#endif //__linux__

#ifdef _MSC_VER
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(hModule, path, MAX_PATH);

	char str[MAX_PATH];
	wcstombs(str, path, MAX_PATH);

	return string(str);

#endif //_MSC_VER
    
#ifdef __APPLE__
    return "";
#endif // __APPLE__
}

void axApp::MainLoop()
{
	_core->MainLoop();
}

void axApp::UpdateAll()
{
	_core->UpdateAll();
}

axManager* axApp::GetWindowManager()
{
	return _core->GetWindowManager();
}

axManager* axApp::GetPopupManager()
{
    return _core->GetPopupManager();
}

void axApp::AddWindow(axWindow* win)
{
	GetWindowManager()->Add(win);
}

void axApp::AddPopWindow(axWindow* win)
{
    //std::cout << "axApp::AddPopWindow" << std::endl;
	_core->GetPopupManager()->Add(win);
}


axCore* axApp::GetCore()
{
	return _core;
}
