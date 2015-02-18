#include "axCoreWin32.h"
#include "axEventManager.h"
#include "axApp.h"
#include <Winuser.h>
#include <Windowsx.h>

bool axCoreWin32::coreWin32_keys[256];
bool axCoreWin32::active = true;
axCore* axCORE = nullptr;

axCoreWin32::axCoreWin32() : axCore()
{
	_popHwnd = nullptr;

}

void axCoreWin32::Init(const axSize& frame_size)
{
    InitManagers();
    
	_popHwnd = nullptr;
	


	char* test = "axCore";
	if (!CreateGLWindow(test, frame_size.x, frame_size.y))
	{
		glewInit();
		return;
	}

	glewInit();
}

string axCoreWin32::OpenFileDialog()
{
	string prompt = "axLib";
	const int BUFSIZE = 1024;
	char buffer[BUFSIZE] = { 0 };
	OPENFILENAME ofns = { 0 };
	ofns.lStructSize = sizeof(ofns);
	ofns.lpstrFile = buffer;
	ofns.nMaxFile = BUFSIZE;
	ofns.lpstrTitle = prompt.c_str();
	GetOpenFileName(&ofns);

	return buffer;
}

HWND axCoreWin32::GetMainWindowHandle()
{
	return _hwnd;
}

HWND axCoreWin32::GetPopupWindowHandle()
{
	return _popHwnd;
}

string axCoreWin32::GetAppDirectory()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(hModule, path, MAX_PATH);

	char str[MAX_PATH];
	wcstombs(str, path, MAX_PATH);

	std::string app_path(str);

	size_t slash_pos = app_path.find_last_of('\\');
	app_path = app_path.substr(0, slash_pos + 1);

	return app_path;
}

bool axCoreWin32::CreatePopupWindow(const char* title, int width, int height)
{

	//int bits = 16;
	//// Holds the results after searching for a match.
	//GLuint PixelFormat;

	//// Grab an instance for our window.
	//_popInstance = GetModuleHandle(NULL);

	//WNDCLASS wc = axWin32::CreateDefaultWindowClassStructure(
	//	"PopOpenGL",
	//	_popInstance,
	//	WindowPopupCallback);

	//// Attempt to register the window class.
	//if (!RegisterClass(&wc))
	//{
	//	axWin32::ErrorMessageDialog("Failed to register the window class.");
	//	return false;
	//}

	//// Window extended style.
	//DWORD dwExStyle = 0;//WS_EX_WINDOWEDGE;

	//// Windows style.
	//DWORD dwStyle = WS_POPUP;// WS_OVERLAPPEDWINDOW;

	//// Window size.
	//RECT window_rect = axWin32::axRectToWin32Rect(axRect(0, 0, width, height));

	//// Adjust window to true requested size.
	//AdjustWindowRectEx(&window_rect, dwStyle, false, dwExStyle);

	//// Create The Window

	//const char* tt = "Popup window";
	//if (!axWin32::CreateHWNDWindow(_popHwnd, _popInstance,
	//	dwStyle, dwExStyle, window_rect, tt, "PopOpenGL"))
	//{
	//	KillPopGLWindow();
	//	//int err = GetLastError();

	//	axWin32::ErrorMessageDialog("Window Creation Error.");
	//	return false;
	//}

	//// Get device context.
	//if (!(_popHdc = GetDC(_popHwnd)))
	//{
	//	KillPopGLWindow();
	//	axWin32::ErrorMessageDialog("Can't Create A GL Device Context.");
	//	return false;
	//}

	//PIXELFORMATDESCRIPTOR pfd = axWin32::GetPixelFormatDescriptor(bits);

	//// Find a matching pixel format.
	//if (!(PixelFormat = ChoosePixelFormat(_popHdc, &pfd)))
	//{
	//	KillPopGLWindow();
	//	axWin32::ErrorMessageDialog("Can't Find A Suitable PixelFormat.");
	//	return false;
	//}

	//// Set pixel format.
	//if (!SetPixelFormat(_popHdc, PixelFormat, &pfd))
	//{
	//	KillPopGLWindow();
	//	axWin32::ErrorMessageDialog("Can't Set The PixelFormat.");
	//	return false;
	//}

	//// Get a rendering context.
	//if (!(_popHrc = wglCreateContext(_popHdc)))
	//{
	//	KillPopGLWindow();
	//	axWin32::ErrorMessageDialog("Can't Create A GL Rendering Context.");
	//	return false;
	//}

	//// Activate the rendering context.
	//if (!wglMakeCurrent(_popHdc, _popHrc))
	//{
	//	KillPopGLWindow();
	//	axWin32::ErrorMessageDialog("Can't Activate The GL Rendering Context.");
	//	return false;
	//}

	//// Show window.
	//ShowWindow(_popHwnd, SW_SHOW);

	//// Slightly higher priority.
	//SetForegroundWindow(_popHwnd);

	//// Set keyboard focus.
	//SetFocus(_popHwnd);

	//// Set perspective GLScreen.
	//ResizePopGLScene(width, height);

	//// Init Gl Window.
	//if (!InitGL())
	//{
	//	KillPopGLWindow();
	//	axWin32::ErrorMessageDialog("Initialization Failed.");
	//	return false;
	//}

	return true;
}

bool axCoreWin32::CreateGLWindow(const char* title, int width, int height, int bits)
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
		axWin32::ErrorMessageDialog("Failed to register the window class.");
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
		dwStyle, dwExStyle, window_rect, tt, "OpenGL"))
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
	ResizeGLScene(axSize(width, height));

	// Init Gl Window.
	if (!InitGL())
	{
		KillGLWindow();
		axWin32::ErrorMessageDialog("Initialization Failed.");
		return false;
	}

	return true;
}

std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return "No error message has been recorded";

	LPSTR messageBuffer = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

void axCoreWin32::KillGLWindow()
{
	if (_hrc)
	{
		// Release DC and RC.
		if (!wglMakeCurrent(NULL, NULL))
		{
			//axWin32::ErrorMessageDialog(GetLastErrorAsString());
			//axWin32::ErrorMessageDialog("Release Of DC And RC Failed.");
			cerr << "Release Of DC And RC Failed : " << GetLastErrorAsString() << endl;
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
	// If the function succeeds, the return value is nonzero.
	if (UnregisterClass("OpenGL", _instance) == 0)
	{
		axWin32::ErrorMessageDialog(GetLastErrorAsString());
		axWin32::ErrorMessageDialog("Could Not Unregister Class.");
		//_instance = NULL;
	}
}

void axCoreWin32::KillPopGLWindow()
{
	//wglMakeCurrent(_hdc, _hrc);

	if (_popHrc)
	{
		// Release DC and RC.
		if (!wglMakeCurrent(NULL, NULL))
		{
			//axWin32::ErrorMessageDialog("Popup Window : Release Of DC And RC Failed.");
			//axWin32::ErrorMessageDialog(GetLastErrorAsString());
			cerr << GetLastErrorAsString() << endl;
		}

		// Delete RC.
		if (!wglDeleteContext(_popHrc))
		{

			cerr << "Popup Window : Release Rendering Context Failed : " << GetLastErrorAsString() << endl;
			//axWin32::ErrorMessageDialog(GetLastErrorAsString());
			//axWin32::ErrorMessageDialog("Popup Window : Release Rendering Context Failed.");
		}
		_popHrc = NULL;
	}

	// Release DC.
	if (_popHdc && !ReleaseDC(_popHwnd, _popHdc))
	{
		//axWin32::ErrorMessageDialog(GetLastErrorAsString());
		//axWin32::ErrorMessageDialog("Popup Window : Release Device Context Failed.");
		_popHdc = NULL;
	}

	// Destroy window.
	if (_popHwnd && !DestroyWindow(_popHwnd))
	{
		//axWin32::ErrorMessageDialog(GetLastErrorAsString());
		//axWin32::ErrorMessageDialog("Popup Window : Could Not Release _hwnd.");
		_popHwnd = NULL;
	}

	//// Unregister class.
	if (!UnregisterClass("PopOpenGL", _popInstance))
	{
		//axWin32::ErrorMessageDialog("Popup Window : Could Not Unregister Class.");
		_popInstance = nullptr;
	}

	wglMakeCurrent(_hdc, _hrc);
	//SetFocus(_hwnd);

}


void axCoreWin32::MainLoop()
{
	MSG	msg;
	BOOL done = false;

	//while (!done)
	while(GetMessage(&msg, NULL, 0, 0) != 0)
	//while (!done)
	{
		/*axEventManager::GetInstance()->CallNext();*/

		//if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		//if (GetMessage(&msg, NULL, 0, 0) != 0)
		//{
			if (msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		//}
		//else
		//{
			if (DrawGLScene())
			{
				SwapBuffers(_hdc);
			}

			if (DrawGLPopScene())
			{
				SwapBuffers(_popHdc);
			}
		//}
	}

	// Shutdown
	KillPopGLWindow();
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

axPoint axCoreWin32::GetScreenRelativePosition()
{
	RECT rect;
	GetWindowRect(_hwnd, &rect);
	return axPoint(rect.left, rect.top);
}

LRESULT CALLBACK axCoreWin32::WindowCallback(HWND hWnd,	// Handle For This Window
	UINT	uMsg, // Message For This Window.
	WPARAM	wParam,	// Additional Message Information
	LPARAM	lParam)	// Additional Message Information
{
	axManager* windowManager = nullptr;

	if (hWnd == static_cast<axCoreWin32*>(axCORE)->GetMainWindowHandle())
	{
		windowManager = axCORE->GetWindowManager();
	}

	if (windowManager != nullptr)
	{
		axPoint mouse_position;
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
			for (int i = 0; i < 5; i++)
				axEventManager::GetInstance()->CallNext();

			switch (wParam)
			{
			case SC_SCREENSAVE:
			case SC_MONITORPOWER:
				return 0;
			}
			break;
		}

		case WM_SETFOCUS:
		{
			cout << "Main got focus" << endl;
			return 0;
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
			axCORE->ResizeGLScene(axSize(LOWORD(lParam), HIWORD(lParam)));
			windowManager->OnSize();

			return 0;
		}

		case WM_MOUSEMOVE:
			mouse_position = axPoint(GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam));

			axApp::MainInstance->GetPopupManager()->OnMouseMotion(mouse_position);
			if (axApp::MainInstance->GetPopupManager()->IsEventReachWindow() == false)
			{
				axApp::MainInstance->GetWindowManager()->OnMouseMotion(mouse_position);
			}
			return 0;

		case WM_LBUTTONDOWN:
			mouse_position = axPoint(GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam));

			axApp::MainInstance->GetPopupManager()->OnMouseLeftDown(mouse_position);

			if (axApp::MainInstance->GetPopupManager()->IsEventReachWindow() == false)
			{
				axApp::MainInstance->GetWindowManager()->OnMouseLeftDown(mouse_position);
			}
			return 0;

		case WM_LBUTTONUP:
			mouse_position = axPoint(GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam));

			axApp::MainInstance->GetPopupManager()->OnMouseLeftUp(mouse_position);
			axApp::MainInstance->GetWindowManager()->OnMouseLeftUp(mouse_position);
			return 0;

		case WM_RBUTTONDOWN:
			mouse_position = axPoint(GET_X_LPARAM(lParam),
				GET_Y_LPARAM(lParam));

			windowManager->OnMouseRightDown(mouse_position);
			return 0;

		case WM_RBUTTONUP:
			windowManager->OnMouseRightUp();
			return 0;
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


	LRESULT CALLBACK axCoreWin32::WindowPopupCallback(HWND hWnd,	// Handle For This Window
													  UINT	uMsg, // Message For This Window.
													  WPARAM	wParam,	// Additional Message Information
													  LPARAM	lParam)	// Additional Message Information
	{
		axManager* windowManager = nullptr;

		//if (hWnd == static_cast<axCoreWin32*>(axCORE)->GetMainWindowHandle())
		//{
		//	windowManager = axCORE->GetWindowManager();
		//	//SetFocus(hWnd);
		//}
		if (hWnd == static_cast<axCoreWin32*>(axCORE)->GetPopupWindowHandle())
		{
			windowManager = axCORE->GetPopupManager();
			//SetFocus(hWnd);
		}

		if (windowManager != nullptr)
		{
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

			case WM_QUIT:
			{
							//axCORE->KillPopGLWindow();
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
							axCORE->ResizeGLScene(axSize(LOWORD(lParam),
								HIWORD(lParam)));

							windowManager->OnSize();
							return 0;
			}

			case WM_MOUSEMOVE:
				windowManager->OnMouseMotion(
					axPoint(GET_X_LPARAM(lParam),
					GET_Y_LPARAM(lParam)));
				return 0;

			case WM_LBUTTONDOWN:
				windowManager->OnMouseLeftDown(
					axPoint(GET_X_LPARAM(lParam),
					GET_Y_LPARAM(lParam)));
				return 0;

			case WM_LBUTTONUP:
				windowManager->OnMouseLeftUp(
					axPoint(GET_X_LPARAM(lParam),
					GET_Y_LPARAM(lParam)));
				return 0;

			case WM_RBUTTONDOWN:
				//windowManager->OnMouseRightDown(mouse_position);
				return 0;

			case WM_RBUTTONUP:
				windowManager->OnMouseRightUp();
				return 0;
			}
		}

		// Pass All Unhandled Messages To DefWindowProc
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}


int axCoreWin32::DrawGLPopScene()
{
	wglMakeCurrent(_popHdc,	_popHrc);
	return 0;
	//return axCore::DrawGLPopScene();
}

int axCoreWin32::DrawGLScene()
{
	//cout << "GL" << endl;
	wglMakeCurrent(_hdc, _hrc);
	return axCore::DrawGLScene();
}

void axCoreWin32::PushEventOnSystemQueue()
{
	BOOL err = PostMessage(_hwnd, WM_SYSCOMMAND, 1, 1);
}