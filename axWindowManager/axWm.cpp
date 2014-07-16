/* TinyWM is written by Nick Welch <nick@incise.org> in 2005 & 2011.
 *
 * This software is in the public domain
 * and is provided AS IS, with NO WARRANTY. */

#include <X11/Xlib.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include "axLib.h"
#include "axCoreX11.h"   
#include <deque>


// bool TEST = true;

using namespace std;  

#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Window CreateWindow(Display* dpy, 
//                                const axRect& rect, 
//                                Window& root_win, 
//                                Colormap& cmap, 
//                                XVisualInfo* v_info)
// {
//     XSetWindowAttributes attributes;
//     attributes.colormap = cmap;
//     attributes.override_redirect = false;

//     return XCreateWindow( dpy,
//                           root_win, // Parent.
//                           rect.position.x,
//                           rect.position.y,
//                           rect.size.x,
//                           rect.size.y,
//                           0,
//                           v_info->depth,
//                           InputOutput,
//                           v_info->visual,
//                           CWColormap | CWOverrideRedirect,
//                           &attributes);
// }

// XVisualInfo* GetGLXVisualInfo(Display* dpy)
// {
//     XVisualInfo* v_info = nullptr;

//     GLint att[] = { GLX_RGBA,
//                     GLX_DEPTH_SIZE,
//                     24,
//                     GLX_DOUBLEBUFFER, None };

//     v_info = glXChooseVisual(dpy, 0, att);

//     if(v_info == nullptr)
//     {
//         cerr << "ERROR : NO VISUAL FOUND !" << endl;
//         v_info =  nullptr;
//     }
//     else
//     {
//         cout << "GLX Visual." << endl;
//     }

//     return v_info;
// }

class axWindowManager
{
public:
    axWindowManager();
    void MainLoop();

private:
    Display* dpy;
    XWindowAttributes attr;
    XButtonEvent start;
    XEvent ev;
    Window win;
    deque<Window> windows;
};

axWindowManager::axWindowManager()
{
    if(!(dpy = XOpenDisplay(0x0))) 
    {
        exit(1);
    }

    win = XDefaultRootWindow(dpy);

    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), Mod1Mask,
             DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);

    XGrabButton(dpy, 1, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

    XGrabButton(dpy, 3, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

    start.subwindow = None;

    XSelectInput(dpy, win, KeyPressMask | SubstructureRedirectMask);
}

void axWindowManager::MainLoop()
    {
        while(1)
        {
            XNextEvent(dpy, &ev);
            

            if(ev.type == KeyPress && ev.xkey.subwindow != None)
            {
                XRaiseWindow(dpy, ev.xkey.subwindow);

                XEvent event_to_process;
                event_to_process.type = Expose;
                event_to_process.xexpose.window = ev.xkey.subwindow;
                XSendEvent(dpy, ev.xkey.subwindow, true, ExposureMask, &event_to_process);
            }

            else if(ev.type == ButtonPress && ev.xbutton.subwindow != None)
            {
                XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
                start = ev.xbutton;
            }

            else if(ev.type == MotionNotify && start.subwindow != None)
            {
                int xdiff = ev.xbutton.x_root - start.x_root;
                int ydiff = ev.xbutton.y_root - start.y_root;
                
                XMoveResizeWindow(dpy, start.subwindow,
                    attr.x + (start.button==1 ? xdiff : 0),
                    attr.y + (start.button==1 ? ydiff : 0),
                    MAX(1, attr.width + (start.button==3 ? xdiff : 0)),
                    MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
            }

            else if(ev.type == ButtonRelease)
            {
                start.subwindow = None;
            }

            else if (ev.type == MapRequest)
            {
                std::cout << "New Window Mapped" << std::endl;
                
                XWindowAttributes att;
                XGetWindowAttributes(dpy, ev.xmaprequest.window, &att);

                Window root_win, parent_before, *child;
                unsigned int nchild;
                XQueryTree(dpy, ev.xmaprequest.window, &root_win, &parent_before, &child, &nchild);

                bool found = false;
                for(Window& n : windows)
                {
                    if(n == ev.xmaprequest.window)
                    {
                        found = true;
                    }
                }

                if(found == false)
                {
                    pid_t pid = fork();

                    // Child.
                    if(pid == 0)
                    {
                        int d = ev.xmaprequest.window;
                        string arg = std::to_string(d);
                        string arg_w = std::to_string(att.width);
                        string arg_h = std::to_string(att.height);

                        execl("/home/alexarse/Desktop/axLib/axProjects/TitleBar/main", 
                              arg.c_str(), arg.c_str(), arg_w.c_str(), arg_h.c_str(), 0);

                        exit(0);
                    }
                    else
                    {
                        XMapWindow(dpy, ev.xmaprequest.window);

                        Window root_win, parent = parent_before, *child;
                        unsigned int nchild;

                        // Active wait for reparenting. 
                        while(parent == parent_before)
                        {
                            XQueryTree(dpy, ev.xmaprequest.window, &root_win, &parent, &child, &nchild);
                            XFree(child);
                        }
                        
                        windows.push_back(parent);
                        
                        XMapWindow(dpy, parent);
                        XMapWindow(dpy, ev.xmaprequest.window);
                    }
                }
            }

            else if(ev.type == Expose)
            {
                std::cout << "Expose : win = " << ev.xexpose.window << std::endl;  
            }
        }
    }

//------------------------
// Desktop app.
//------------------------
class Desktop: public axPanel
{
public:
    Desktop(axApp* app, axWindow* parent, const axRect& rect);

private:
    virtual void OnPaint();
    virtual void OnMouseLeftDown(const axPoint& pos);
    virtual void OnMouseMotion(const axPoint& pos);
};

// 
// Desktop::Desktop(axApp* app, axWindow* parent, const axRect& rect):
//             axPanel(app, parent, rect)
// {

// }

// void Desktop::OnMouseLeftDown(const axPoint& pos)
// {

// }

// void Desktop::OnMouseMotion(const axPoint& pos)
// {

// }


// void Desktop::OnPaint()
// {
//     // cout << "REDRAW" << endl;
//     axGC* gc = GetGC();
//     axRect rect(GetRect());
//     axRect rect0(axPoint(0, 0), rect.size);

//     gc->SetColor(axColor(1.0, 1.0, 1.0), 1.0);
//     gc->DrawRectangle(rect0);
// }

int main()
{
    axWindowManager manager;
    manager.MainLoop();
}

