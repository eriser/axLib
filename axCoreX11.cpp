#include "axCoreX11.h"
#include "axApp.h"

#include <X11/cursorfont.h>
// #include <X11/MwmUtil.h>

axCoreX11::axCoreX11(axApp* app) : axCore()
{
	_app = app;
}

void axCoreX11::Init(const axSize& frame_size)
{
	char win_name[] = {"axCore"};
	if (!CreateGLWindow(win_name, frame_size.x, frame_size.y))
	{
		/// @todo Deal with error.
		return;
	}
}

axRect axCoreX11::GetScreenRect()
{
	return axRect(0, 0, 
				  XDisplayWidth(_display, _screen),
                  XDisplayHeight(_display, _screen));
}

//-----------------------------------------------------------------------------
// PROTECTED
//-----------------------------------------------------------------------------

void axCoreX11::KillGLWindow()
{
	//XDestroyWindow(_display, _win);
  	XCloseDisplay(_display);
}

bool axCoreX11::CreateGLWindow(char* title, int width, int height, int bits)
{
	_display = CreateDisplay();

	_screen = XDefaultScreen(_display);

	_depth = XDefaultDepth(_display, _screen);

	_rootWindow = XDefaultRootWindow(_display);

	_nScreen = 1;

	// Setup GLX.
	_visualInfo = GetGLXVisualInfo(_display);

	Visual* visual = _visualInfo->visual;

	_colormap = XCreateColormap(_display, _rootWindow, visual, AllocNone);
	
	glEnable(GL_TEXTURE_2D);

	// Create window.
	if( width == 0 && height == 0)
	{
		_winRect = GetScreenRect();
	
	}
	else
	{
		_winRect = axRect(0, 0, width, height);
	}
	
	
	_win = CreateWindow(_display, 
						_winRect, 
						_rootWindow, 
						_colormap, 
						_visualInfo);
	
	_glxContext = glXCreateContext(_display, _visualInfo, NULL, GL_TRUE);
	
	glXMakeCurrent(_display, _win, _glxContext);

    XSelectInput(_display, _win,
             	 StructureNotifyMask |
             	 ExposureMask        |
             	 ButtonPressMask     |
             	 ButtonReleaseMask   |
             	 PointerMotionMask   |
             	 EnterWindowMask     |
             	 LeaveWindowMask     |
             	 FocusChangeMask);
             	 //SubstructureRedirectMask);

    // Show window.
    XMapWindow(_display, _win);
    //XFlush(dpy);

	XStoreName(_display, _win, "axLib");

	Cursor cursor = XCreateFontCursor(_display, XC_left_ptr);
    XDefineCursor(_display, _win, cursor);


	ResizeGLScene(_winRect.size.x, _winRect.size.y);

	if (!InitGL())
	{
		std::cerr << "Initialization Failed." << endl;
		return false;
	}

	// void axWindow::SetPosition( const axPoint& pos )
// {
    // m_rect.position = pos;
    //-----------------------------------------------------------------------
    //////////////////////////////////////////////XMoveWindow( _display, _win, 0, 0);
    //-----------------------------------------------------------------------
// }

	//----------------------------------------------------------------
	// typedef struct Hints
	// {
	//     unsigned long   flags;
	//     unsigned long   functions;
	//     unsigned long   decorations;
	//     long            inputMode;
	//     unsigned long   status;
	// } Hints;

	// //code to remove decoration
	// Hints hints;

	// Atom property;

	// hints.flags = 2;
	// hints.functions = (1L << 5);

	// hints.decorations = (1L << 3);//;(1L << 1);

	// property = XInternAtom(_display, "_MOTIF_WM_HINTS", true);

	// XChangeProperty(_display, _win, 
	// 				property, property, 32, 
	// 				PropModeReplace,(unsigned char *)&hints,5);

	// XMapWindow(_display, _win);
	//------------------------------------------------------------------

	return true;
}

Display* axCoreX11::GetDisplay()
{
	return _display;
}

Window axCoreX11::GetWindow()
{
	return _win;
}

Window axCoreX11::GetRootWindow()
{
	return _rootWindow;
}

void axCoreX11::MainLoop()
{
	//---------------------------------------------------------------------
	Atom WM_DELETE_WINDOW = XInternAtom(_display, "WM_DELETE_WINDOW", False); 
	XSetWMProtocols(_display, _win, &WM_DELETE_WINDOW, 1);  

	// bool uname_ok = false;
	// struct utsname sname;  
	// int ret = uname(&sname);

	// if (ret != -1)
	// {
	// 	uname_ok = true;
	// }
	//---------------------------------------------------------------------

	XEvent e;

	bool loop_on = true;
	while (loop_on) 
	{
		XNextEvent(_display, &e);

		axCore* core = _app->GetCore();
		axManager* windowManager = _app->GetCore()->GetWindowManager();

		//axManager* windowManager = axCORE->GetWindowManager();
		switch(e.type)
		{
			case Expose:
				cout << "Catch Expose : " << e.xexpose.window << endl; 
				UpdateAll();

			break;

			case ConfigureNotify:
			{
				XConfigureEvent conf = e.xconfigure;
				//axCORE->ResizeGLScene(conf.width, conf.height);
				core->ResizeGLScene(conf.width, conf.height);
				windowManager->OnSize();
			}
			break;

			case MotionNotify:
			{
				axPoint m_pos(e.xmotion.x, e.xmotion.y);
				windowManager->OnMouseMotion(m_pos);

				
			}
			break;

			case ButtonPress:
			{
				cout << "ButtonPress" << endl; 

				switch(e.xbutton.button)
				{
					case 1: // Mouse left down.
					{
						std::cout << "Btn : " << e.xexpose.window << std::endl; 
						axPoint m_pos(e.xbutton.x, e.xbutton.y);
						windowManager->OnMouseLeftDown(m_pos);
					}
					break;

					case 3: // Mouse right down.
					break;
				}
			}
			break;

			case ButtonRelease:
			{
				switch(e.xbutton.button)
				{
					case 1: // Mouse left up.
					{
						axPoint m_pos(e.xbutton.x, e.xbutton.y);
						windowManager->OnMouseLeftUp(m_pos);
					}
					break;

					case 3: // Mouse right up.
					break;
				}
			}
			break;

			case FocusIn:
			cout << "FOCUS IN" << endl;
			break;

			case EnterNotify:
			cout << "ENTER" << endl;
			break;

			case LeaveNotify:
			cout << "LEAAVE" << endl;
			break;

			case KeyPress:
			{
				char buf[128] = {0};
				KeySym keysym;
				int len = XLookupString(&e.xkey, buf, sizeof buf, &keysym, NULL);
			
				if (keysym == XK_Escape)
				{
					loop_on = false;
					exit(1);
				}
			}
			break;

			case ClientMessage:
			{
				if(static_cast<unsigned int>(e.xclient.data.l[0]) == WM_DELETE_WINDOW)
				{
					loop_on = false;
				}
			}
			break;
		}

		if (DrawGLScene())
		{
			glXSwapBuffers(_display, _win);
		}
	}

	KillGLWindow();
}

//-----------------------------------------------------------------------------
// PRIVATE
//-----------------------------------------------------------------------------

Display* axCoreX11::CreateDisplay()
{
	Display* dpy = XOpenDisplay(NULL);
	
	if (dpy == NULL) 
	{
		cerr << "Cannot open X11 display." << endl;
		exit(1);
	}

	return dpy;
}

XVisualInfo* axCoreX11::GetGLXVisualInfo(Display* dpy)
{
    XVisualInfo* v_info = nullptr;

    GLint att[] = { GLX_RGBA,
                    GLX_DEPTH_SIZE,
                    24,
                    GLX_DOUBLEBUFFER, None };

    v_info = glXChooseVisual(dpy, 0, att);

    if(v_info == nullptr)
    {
        cerr << "ERROR : NO VISUAL FOUND !" << endl;
        v_info =  nullptr;
    }
    else
    {
        cout << "GLX Visual." << endl;
    }

    return v_info;
}

Window axCoreX11::CreateWindow(Display* dpy, 
							   const axRect& rect, 
							   Window& root_win, 
						       Colormap& cmap, 
						       XVisualInfo* v_info)
{
	XSetWindowAttributes attributes;
    attributes.colormap = cmap;
    attributes.override_redirect = false;

    return XCreateWindow( dpy,
                          root_win, // Parent.
                          rect.position.x,
                          rect.position.y,
                          rect.size.x,
                          rect.size.y,
                          0,
                          v_info->depth,
                          InputOutput,
                          v_info->visual,
                          CWColormap | CWOverrideRedirect,
                          &attributes);
}