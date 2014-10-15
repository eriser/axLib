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
    _core = new axCoreMac();
    _core->Init(axSize(1000, 717));
#endif
    
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
    _core = new axCoreMac();
    _core->Init(frame_size);
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

bool axApp::CreatePopupWindow(char* title, int x, int y)
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
	//axCORE->MainLoop();
	_core->MainLoop();
}

void axApp::UpdateAll()
{
	// axCORE->UpdateAll();
	_core->UpdateAll();
}

axManager* axApp::GetWindowManager()
{
	return _core->GetWindowManager();
	//return axCORE->GetWindowManager();
}

void axApp::AddWindow(axWindow* win)
{
	GetWindowManager()->Add(win);
}

void axApp::AddPopWindow(axWindow* win)
{
	cerr << "POP WINDOW ERROR" << endl;
	// _core->GetPopupManager()->Add(win);
}


axCore* axApp::GetCore()
{
	return _core;
}
