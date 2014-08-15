#ifndef __AX_CORE_WIN32__
#define __AX_CORE_WIN32__

#include "axCore.h"
#include "axWin32Utils.h"

extern axCore* axCORE;

class axCoreWin32 : public axCore
{
public:
	axCoreWin32();

	static bool active;
	static bool coreWin32_keys[256];

	virtual void Init(const axSize& frame_size);
	virtual axRect GetScreenRect();

protected:
	virtual void KillGLWindow();
	virtual bool CreateGLWindow(char* title, int width, int height, int bits = 16);
	virtual void MainLoop();

private:
	HDC _hdc;

	// Permanent rendering context.
	HGLRC _hrc;	

	// Holds our window handle.
	HWND _hwnd; 
	HINSTANCE _instance;

	static LRESULT CALLBACK WindowCallback(HWND _hwnd,
										   UINT uMsg,
										   WPARAM wParam,
										   LPARAM lParam);
};

#endif //__AX_CORE_WIN32__