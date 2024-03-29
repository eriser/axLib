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
	virtual int DrawGLPopScene();
	virtual int DrawGLScene();
	virtual bool CreatePopupWindow(char* title, int width, int height);
	virtual string GetAppDirectory();
	HWND GetMainWindowHandle();
	HWND GetPopupWindowHandle();

	virtual string OpenFileDialog();

	virtual void PushEventOnSystemQueue();

	axPoint GetScreenRelativePosition();

protected:
	virtual void KillGLWindow();
	virtual void KillPopGLWindow();
	virtual bool CreateGLWindow(char* title, int width, int height, int bits = 16);
	virtual void MainLoop();

private:
	HDC _hdc;

	// Permanent rendering context.
	HGLRC _hrc;	

	HDC _popHdc;
	HGLRC _popHrc;

	// Holds our window handle.
	HWND _hwnd, _popHwnd; 
	HINSTANCE _instance, _popInstance;

	static LRESULT CALLBACK WindowCallback(HWND _hwnd,
										   UINT uMsg,
										   WPARAM wParam,
										   LPARAM lParam);

	static LRESULT CALLBACK WindowPopupCallback(HWND _hwnd,
												UINT uMsg,
												WPARAM wParam,
												LPARAM lParam);
};

#endif //__AX_CORE_WIN32__