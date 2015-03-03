/// @defgroup Core 
/// @{

#ifndef __AX_CORE_X11__
#define __AX_CORE_X11__

#include "axCore.h"

class axApp;

class axCoreX11 : public axCore
{
public:
	axCoreX11(axApp* app);

	virtual void Init(const axSize& frame_size);
	virtual axRect GetScreenRect();

	Display* GetDisplay();
	Window GetWindow();
	Window GetRootWindow();

// protected:
	virtual void KillGLWindow();
	virtual bool CreateGLWindow(char* title, 
								int width, 
								int height, 
								int bits = 16);

	virtual string GetAppDirectory();
	virtual bool CreatePopupWindow(char* title, int width, int height);
	virtual string OpenFileDialog();
	virtual void KillPopGLWindow();

	virtual void MainLoop();

	Display* CreateDisplay();

	XVisualInfo* GetGLXVisualInfo(Display* dpy);

	Window CreateWindow(Display* dpy, const axRect& rect, Window& root_win, 
						Colormap& cmap, XVisualInfo* v_info);



private:
	Display* _display;

	Window _win, _rootWindow;

	XVisualInfo* _visualInfo;
	
	int _screen, _nScreen, _depth;
		
	Colormap _colormap;
	
	GLXContext _glxContext;

	axRect _winRect;

	


	axApp* _app;
};


#endif //__AX_CORE_X11__

/// @}