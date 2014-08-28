/// @defgroup Core 
/// @{

#ifndef __AX_APP__
#define __AX_APP__

#ifdef __linux__
#include "axCoreX11.h"
#endif //__linux__

#ifdef _MSC_VER
#include "axCoreWin32.h"
#endif //_MSC_VER

#include "axC++.h"

// Is use as an adapter to global axCore class (axCORE).
class axApp
{
public:
	axApp();
	// ~axApp();

	axApp(const axSize& frame_size);

	void MainLoop();

	void CreatePopupWindow(const axSize& size);

	axManager* GetWindowManager();

	void AddWindow(axWindow* win);
	void AddPopWindow(axWindow* win);

	void UpdateAll();

	axCore* GetCore();

	string OpenFileDialog();

	bool CreatePopupWindow(char*, int, int);

	string GetCurrentAppDirectory();

	string GetAppDirectory();

private:
	axCore* _core;
};

//extern axCore* axCORE;

#endif //__AX_APP__

/// @}
