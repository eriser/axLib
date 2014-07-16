/// @defgroup Core 
/// @{

#ifndef __AX_APP__
#define __AX_APP__

#include "axCoreX11.h"
#include "axC++.h"

// Is use as an adapter to global axCore class (axCORE).
class axApp
{
public:
	axApp();

	axApp(const axSize& frame_size);

	void MainLoop();

	axManager* GetWindowManager();

	void AddWindow(axWindow* win);

	void UpdateAll();

	axCore* GetCore();

private:
	axCore* _core;
};

//extern axCore* axCORE;

#endif //__AX_APP__

/// @}
