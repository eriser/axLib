#include "axC++.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <X11/Xos.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <sys/utsname.h>

Display* CreateDisplay()
{
	Display* dpy = XOpenDisplay(NULL);
	
	if (dpy == NULL) 
	{
		cerr << "Cannot open X11 display." << endl;
		exit(1);
	}

	return dpy;
}

XVisualInfo* GetGLXVisualInfo(Display* dpy)
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

Window CreateWindow(Display* dpy, const axRect& rect, Window& root_win, 
					Colormap& cmap, XVisualInfo* v_info)
{
	XSetWindowAttributes attributes;
    attributes.colormap = cmap;

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
                          CWColormap/* | CWEventMask*/,
                          &attributes);
}
 
 void UpdateWindow(Display* dpy, Window& win, axRect& win_rect)
 {
	//glXMakeCurrent(dpy, win, glx_gc);
    glViewport(0, 0, win_rect.size.x, win_rect.size.y);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Draw rectangle.
    axFloatRect rect( win_rect.position.x - 1.0, win_rect.position.y - 1 ,
                      win_rect.size.x * 2.0, win_rect.size.y * 2.0 );

	glColor3f(0.2, 0.6, 0.8);

    glBegin(GL_QUADS);
        glVertex3f( rect.position.x, rect.position.y, 0 ); // Bottom left.

        glVertex3f( rect.position.x + rect.size.x,
                    rect.position.y, 0 ); // Bottom Right.

        glVertex3f( rect.position.x + rect.size.x,
                    rect.position.y + rect.size.y, 0 ); // Top Right.

        glVertex3f( rect.position.x,
                    rect.position.y + rect.size.y, 0 ); // Top Left
    glEnd();

    glXSwapBuffers(dpy, win);
 }


void MainLoop(Display* dpy, Window& win, axRect& win_rect)
{
	//---------------------------------------------------------------------
	Atom WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", False); 
	XSetWMProtocols(dpy, win, &WM_DELETE_WINDOW, 1);  

	bool uname_ok = false;
	struct utsname sname;  
	int ret = uname(&sname);

	if (ret != -1)
	{
	uname_ok = true;
	}
	//---------------------------------------------------------------------

	XEvent e;

	bool loop_on = true;
	while (loop_on) 
	{
		XNextEvent(dpy, &e);


		switch(e.type)
		{
			case Expose:
				UpdateWindow(dpy, win, win_rect);
			break;

			case MotionNotify:
			break;

			case ButtonPress:
			{
				switch(e.xbutton.button)
				{
					case 1: // Mouse left down.
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
					break;

					case 3: // Mouse right up.
					break;
				}
			}
			break;

			case FocusIn:
			break;

			case EnterNotify:
			break;

			case LeaveNotify:
			break;

			case KeyPress:
			{
				char buf[128] = {0};
				KeySym keysym;
				int len = XLookupString(&e.xkey, buf, sizeof buf, &keysym, NULL);
			
				if (keysym == XK_Escape)
				{
					loop_on = false;
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
	}
}

int main(int argc, char** argv)
{
	// Create display.
	Display* dpy = CreateDisplay();
	int screen = XDefaultScreen(dpy);
	int depth = XDefaultDepth(dpy, screen);
	Window root_win = XDefaultRootWindow(dpy);
	int numScreens = 1;
	Drawable screens[] = { root_win };
	axRect dpy_rect = axRect(0, 0, XDisplayWidth(dpy, screen),
                   			 XDisplayHeight(dpy, screen));

	// Setup GLX.
	XVisualInfo* v_info = GetGLXVisualInfo(dpy);
	Visual* visual = v_info->visual;

	Colormap colormap = XCreateColormap(dpy, root_win, visual, AllocNone);
	glEnable(GL_TEXTURE_2D);

	// Create window.
	axRect window_rect(0, 0, 500, 500);
	
	Window win = CreateWindow(dpy, window_rect, root_win, colormap, v_info);
	
	// Pixmap back_buffer = XCreatePixmap(dpy, win,
 //                                       window_rect.size.x, window_rect.size.y,
 //                                       screen);
	
	GLXContext glx_context = glXCreateContext(dpy, v_info, NULL, GL_TRUE);
	//GC gc;
	glXMakeCurrent(dpy, win, glx_context);
	//glXMakeCurrent( AX_GLOBAL_DISPLAY, win, gc->GetGLX_GC() );

	//Window win = XCreateSimpleWindow(dpy, RootWindow(dpy, s), 10, 10, 660, 200, 1,
	//                               BlackPixel(dpy, s), WhitePixel(dpy, s));

    XSelectInput(dpy, win,
             	 StructureNotifyMask |
             	 ExposureMask        |
             	 ButtonPressMask     |
             	 ButtonReleaseMask   |
             	 PointerMotionMask   |
             	 EnterWindowMask     |
             	 LeaveWindowMask     |
             	 FocusChangeMask );

    // Show window.
    XMapWindow(dpy, win);
    //XFlush(dpy);

	XStoreName(dpy, win, "axLib");


  //XSelectInput(dpy, win, ExposureMask | KeyPressMask);
  //XMapWindow(dpy, win);
 
// #if defined(__APPLE_CC__)  
//   XStoreName(dpy, win, "Geeks3D.com - X11 window under Mac OS X (Lion)");
// #else
//   XStoreName(dpy, win, "Geeks3D.com - X11 window under Linux (Mint 10)");
// #endif  
 
  
 
  MainLoop(dpy, win, window_rect);
 
  XDestroyWindow(dpy, win);
  XCloseDisplay(dpy);
  return 0;
}