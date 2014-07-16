#include "axApp.h"

axApp::axApp()
{
	_core = new axCoreX11(this);
	_core->Init(axSize(0, 0));
	// // axCORE = new axCoreWin32();
	// axCORE = new axCoreX11();
	// axCORE->Init(axSize(500, 500));
	// //axManager* win_manager = axCORE->GetWindowManager();
	// //win_manager->InitManager(axCORE->GetScreenRect().size);
}

axApp::axApp(const axSize& frame_size)
{
	_core = new axCoreX11(this);
	_core->Init(frame_size);
	// //axCORE = new axCoreWin32();
	// axCORE = new axCoreX11();
	// axCORE->Init(frame_size);
	// //axManager* win_manager = axCORE->GetWindowManager();
	// //win_manager->InitManager(axCORE->GetScreenRect().size);
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

axCore* axApp::GetCore()
{
	return _core;
}
