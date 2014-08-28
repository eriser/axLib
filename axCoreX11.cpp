#include "axCoreX11.h"
#include "axApp.h"

#include <X11/cursorfont.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
// #include <X11/extensions/Xcomposite.h>
// #include <X11/extensions/Xrender.h>
#include <X11/extensions/Xfixes.h>
#include <X11/Xatom.h>
#include <X11/Xmd.h>

// #include <X11/Xutil.h>
// #include <X11/Xatom.h>
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

string axCoreX11::GetAppDirectory()
{
	return string("error");
}

bool axCoreX11::CreatePopupWindow(char* title, int width, int height)
{
	return false;
}

string axCoreX11::OpenFileDialog()
{
	return "";
}

void axCoreX11::KillPopGLWindow()
{
	
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

#define MAX_PROPERTY_VALUE_LEN 4096

char* GetPropriety(Display *disp, Window win, 
        			 Atom xa_prop_type, 
        			 string prop_name, unsigned long& size) 
{
    Atom xa_prop_name;
    Atom xa_ret_type;
    int ret_format;
    unsigned long ret_nitems;
    unsigned long ret_bytes_after;
    
    unsigned char *ret_prop;
    // gchar *ret;
    
    xa_prop_name = XInternAtom(disp, prop_name.c_str(), False);
    
     int result = XGetWindowProperty(
     	disp, // Specifies the connection to the X server.

     	win, // Specifies the window whose property you want to obtain.

     	xa_prop_name, // Specifies the property name.

     	0, // Specifies the offset in the specified property 
     	   // (in 32-bit quantities) where the data is to be retrieved.

     	1024, // Specifies the length in 32-bit multiples 
     		  // of the data to be retrieved.

     	False, // Specifies a Boolean value that determines whether the 
     		   // property is deleted.

        xa_prop_type, // Specifies the atom identifier associated with 
        			  // the property type or AnyPropertyType.
        
        &xa_ret_type, // Returns the atom identifier that defines the 
        			  // actual type of the property.

        &ret_format, // Returns the actual format of the property.

        &ret_nitems, // Returns the actual number of 8-bit, 16-bit, 
        			 // or 32-bit items stored in the prop_return data.

        &ret_bytes_after, // Returns the number of bytes remaining to be read 
        				  //in the property if a partial read was performed.

        &ret_prop // Returns the data in the specified format. 
        );	

    if(result != Success) 
    {
    	cerr << "Error with XGetWindowProperty." << endl;
        // p_verbose("Cannot get %s property.\n", prop_name);
        return "";
    }
  
    if(xa_ret_type != xa_prop_type) 
    {
        // p_verbose("Invalid type of %s property.\n", prop_name);
        XFree(ret_prop);
        return "";
    }

    /* null terminate the result to make string handling easier */
    unsigned long tmp_size = (ret_format / 8) * ret_nitems;

    // string ret(ret_prop);//ret = new 
    char* ret = new char[tmp_size + 1];
    memcpy(ret, ret_prop, tmp_size);
    ret[tmp_size] = '\0';
    size = tmp_size;
    
    XFree(ret_prop);
    return ret;
}

bool WindowManagerSupported(Display *disp, const string& prop) 
{
    unsigned long size;
    Atom *list = (Atom*)GetPropriety(disp, 
    								 DefaultRootWindow(disp), 
    								 XA_ATOM, 
    								 "_NET_SUPPORTED", 
    								 size);
    if(list == NULL) 
    {
        // p_verbose("Cannot get _NET_SUPPORTED property.\n");
        return false;
    }

    Atom xa_prop = XInternAtom(disp, prop.c_str(), False);

    for (int i = 0; i < size / sizeof(Atom); i++) 
    {
        if (list[i] == xa_prop) 
        {
            // g_free(list);

            return true;
        }
    }
    
    // g_free(list);

    return false;
}



void axCoreX11::MainLoop()
{
	// Not sure what's this for anymore.
	Atom _NET_WM_STATE = XInternAtom(_display, "_NET_WM_STATE", False); 
	XSetWMProtocols(_display, _win, &_NET_WM_STATE, 1);

	Atom del = XInternAtom(_display, "WM_DELETE_WINDOW", False); 
	XSetWMProtocols(_display, _win, &del, 1);  

	XEvent e;

	bool loop_on = true;

	while (loop_on) 
	{
		XNextEvent(_display, &e);
		axCore* core = _app->GetCore();
		axManager* windowManager = _app->GetCore()->GetWindowManager();

		switch(e.type)
		{
			case Expose:
				// cout << "Catch Expose : " << e.xexpose.window << endl; 
				UpdateAll();

			break;

			case ConfigureNotify:
			{
				XConfigureEvent conf = e.xconfigure;
				axSize size = GetGlobalSize();

				if (conf.width != size.x || conf.height != size.y) 
				{
	                core->ResizeGLScene(conf.width, conf.height);
					windowManager->OnSize();
            	}
            	else // Window move.
            	{
            		// cout << "Window move" << endl;
            	}
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
				switch(e.xbutton.button)
				{
					case 1: // Mouse left down.
					{
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
					/// @todo Implement mouse right up.
					case 3: // Mouse right up.
					break;
				}
			}
			break;

			/// @todo Implement FocusInApp.
			case FocusIn:
			// cout << "FOCUS IN" << endl;
			break;

			/// @todo Implement MouseEnterApp.
			case EnterNotify:
			// cout << "ENTER" << endl;
			break;

			/// @todo Implement MouseLeavingApp..
			case LeaveNotify:
			// cout << "LEAAVE" << endl;
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
				if((Atom)e.xclient.data.l[0] == del)
				{
					loop_on = false;
					// cout << "Delete Window" << endl;
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
        // cout << "GLX Visual." << endl;
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