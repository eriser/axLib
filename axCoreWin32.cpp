#include "axCoreWin32.h"
#include <Winuser.h>
#include <Windowsx.h>

bool axCoreWin32::coreWin32_keys[256];
bool axCoreWin32::active = true;
axCore* axCORE = nullptr;

axCoreWin32::axCoreWin32() : axCore()
{

}

void axCoreWin32::Init(const axSize& frame_size)
{
	char* test = "axCore";
	if (!CreateGLWindow(test, frame_size.x, frame_size.y))
	{
		return;
	}
}

bool axCoreWin32::CreateGLWindow(char* title, int width, int height, int bits)
{

	// Holds the results after searching for a match.
	GLuint PixelFormat;		
				
	// Grab an instance for our window.
	_instance = GetModuleHandle(NULL);				
	
	WNDCLASS wc = axWin32::CreateDefaultWindowClassStructure(
		"OpenGL", 
		_instance, 
		WindowCallback);

	// Attempt to register the window class.
	if (!RegisterClass(&wc))									
	{
		axWin32::ErrorMessageDialog("Failed To Register The Window Class.");
		return false;
	}

	// Window extended style.
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	// Windows style.
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;		
	//WS_OVERLAPPEDWINDOW  
	//DWORD dwStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);

	//DWORD dwStyle = (WS_OVERLAPPEDWINDOW);

	// Window size.
	RECT window_rect = axWin32::axRectToWin32Rect(axRect(0, 0, width, height));

	// Adjust window to true requested size.
	AdjustWindowRectEx(&window_rect, dwStyle, false, dwExStyle);		

	// Create The Window
	
	const char* tt = "bkabla";
	if (!axWin32::CreateHWNDWindow(_hwnd, _instance, 
		dwStyle, dwExStyle, window_rect, tt))
	{
		KillGLWindow();
		//int err = GetLastError();

		axWin32::ErrorMessageDialog("Window Creation Error.");
		return false;
	}
	
	// Get device context.
	if (!(_hdc = GetDC(_hwnd)))							
	{
		KillGLWindow();
		axWin32::ErrorMessageDialog("Can't Create A GL Device Context.");
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd = axWin32::GetPixelFormatDescriptor(bits);

	// Find a matching pixel format.
	if (!(PixelFormat = ChoosePixelFormat(_hdc, &pfd)))	
	{
		KillGLWindow();
		axWin32::ErrorMessageDialog("Can't Find A Suitable PixelFormat.");
		return false;
	}

	// Set pixel format.
	if (!SetPixelFormat(_hdc, PixelFormat, &pfd))
	{
		KillGLWindow();
		axWin32::ErrorMessageDialog("Can't Set The PixelFormat.");
		return false;
	}

	// Get a rendering context.
	if (!(_hrc = wglCreateContext(_hdc)))
	{
		KillGLWindow();
		axWin32::ErrorMessageDialog("Can't Create A GL Rendering Context.");
		return false;
	}

	// Activate the rendering context.
	if (!wglMakeCurrent(_hdc, _hrc))
	{
		KillGLWindow();
		axWin32::ErrorMessageDialog("Can't Activate The GL Rendering Context.");
		return false;
	}

	// Show window.
	ShowWindow(_hwnd, SW_SHOW);

	// Slightly higher priority.
	SetForegroundWindow(_hwnd);						
	
	// Set keyboard focus.
	SetFocus(_hwnd);

	// Set perspective GLScreen.
	ResizeGLScene(width, height);

	// Init Gl Window.
	if (!InitGL())
	{
		KillGLWindow();
		axWin32::ErrorMessageDialog("Initialization Failed.");
		return false;
	}

	return true;
}

void axCoreWin32::KillGLWindow()
{
	if (_hrc)
	{
		// Release DC and RC.
		if (!wglMakeCurrent(NULL, NULL))
		{
			axWin32::ErrorMessageDialog("Release Of DC And RC Failed.");
		}

		// Delete RC.
		if (!wglDeleteContext(_hrc))
		{
			axWin32::ErrorMessageDialog("Release Rendering Context Failed.");
		}
		_hrc = NULL;
	}

	// Release DC.
	if (_hdc && !ReleaseDC(_hwnd, _hdc))
	{
		axWin32::ErrorMessageDialog("Release Device Context Failed.");
		_hdc = NULL;
	}

	// Destroy window.
	if (_hwnd && !DestroyWindow(_hwnd))
	{
		axWin32::ErrorMessageDialog("Could Not Release _hwnd.");
		_hwnd = NULL;
	}

	// Unregister class.
	if (!UnregisterClass("OpenGL", _instance))
	{
		axWin32::ErrorMessageDialog("Could Not Unregister Class.");
		_instance = NULL;
	}
}

void axCoreWin32::MainLoop()
{
	MSG	msg;
	BOOL done = false;

	//while (!done)
	//while(GetMessage(&msg, NULL, 0, 0) != 0)
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Comment.
		{
			if (msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else // Comment.
		{
			if (DrawGLScene())
			{
				SwapBuffers(_hdc);
			}
		}
	}

	// Shutdown
	KillGLWindow();
}

axRect axCoreWin32::GetScreenRect()
{
	RECT wrect;
	GetWindowRect(GetDesktopWindow(), &wrect);
	
	return axRect(wrect.left, wrect.top, 
				  wrect.right - wrect.left,
				  wrect.bottom - wrect.top);
}

LRESULT CALLBACK axCoreWin32::WindowCallback(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{

	//cout << "CALLBACK FUNCTION" << endl;

	/*if (axCORE != nullptr)
	{
		cout << "axCORE OK" << endl;
	}
	else if (axCORE == nullptr)
	{
		cerr << "axCORE ERROR" << endl;
	}*/
	//axCore* core = axCoreWin32::GetInstance();
	//axCore* core = axCORE->GetInstance();
	axManager* windowManager = axCORE->GetWindowManager();



	// Windows messages.
	switch (uMsg)
	{
	case WM_ACTIVATE:
	{
		// Check Minimization State
		if (!HIWORD(wParam))
		{
			active = true;
		}
		else
		{
			active = false;
		}

		return 0;
	}

	case WM_SYSCOMMAND:
	{
		switch (wParam)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
			return 0;
		}
		break;
	}

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_KEYDOWN:
	{
			
		coreWin32_keys[wParam] = true;
		return 0;
	}

	case WM_KEYUP:
	{
			
		coreWin32_keys[wParam] = false;
		return 0;
	}

	// Resize The OpenGL Window
	case WM_SIZE:
	{
		// LoWord=Width, HiWord=Height
		axCORE->ResizeGLScene(LOWORD(lParam), HIWORD(lParam));

		windowManager->OnSize();
		return 0;
	}

	case WM_MOUSEMOVE:
		windowManager->OnMouseMotion(
			axPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		return 0;
		
	case WM_LBUTTONDOWN:
		windowManager->OnMouseLeftDown(
			axPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		return 0;
	
	case WM_LBUTTONUP:
		windowManager->OnMouseLeftUp(
			axPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		return 0;

	case WM_RBUTTONDOWN:
		windowManager->OnMouseRightDown();
		return 0;

	case WM_RBUTTONUP:
		windowManager->OnMouseRightUp();
		return 0;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
