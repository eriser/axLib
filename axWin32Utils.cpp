#include "axWin32Utils.h"

RECT axWin32::axRectToWin32Rect(const axRect& rect)
{
	RECT win_rect;
	win_rect.left = rect.position.x;
	win_rect.right = rect.position.x + rect.size.x;
	win_rect.top = rect.position.y;
	win_rect.bottom = rect.position.y + rect.size.y;
	return win_rect;
}

WNDCLASS axWin32::CreateDefaultWindowClassStructure(const char* label, 
										   HINSTANCE hinstance,
										   WNDPROC callbackFctPointer)
{
	// Windows Class Structure
	WNDCLASS wc;

	// Redraw on size, and own dc for window.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	
	
	// Wndproc handles messages
	wc.lpfnWndProc = (WNDPROC)callbackFctPointer;

	// No extra window data.
	wc.cbClsExtra = 0;									
	wc.cbWndExtra = 0;	

	// Windows hinstance.
	wc.hInstance = hinstance;							
	
	// Load the default icon.
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			
	
	// Arrow pointer.
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			
	
	// No background needed for OpenGl.
	wc.hbrBackground = NULL;								
	
	// Windows menu.
	wc.lpszMenuName = NULL;									

	// Set the class name.
	wc.lpszClassName = label;

	return wc;
}


bool axWin32::CreateHWNDWindow(HWND& win,
							   HINSTANCE& hinstance,
							   DWORD& style,
							   DWORD& exStyle,
							   RECT& win_rect,
							   const char* title,
							   const char* winclassname)
{
	win = CreateWindowEx(
		exStyle,	// Extended style. 
		winclassname,	// Class name.
		title,		// Title.
		style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, // Window style.
		10, //win_rect.left,
		10, //win_rect.top,
		win_rect.right - win_rect.left,
		win_rect.bottom - win_rect.top,
		nullptr, // Parent window.
		nullptr, // Window menu.
		hinstance,
		nullptr // Don't pass anything to WM_CREATE.
		);

	return (win != NULL);
}

PIXELFORMATDESCRIPTOR axWin32::GetPixelFormatDescriptor(const int& bits)
{
	PIXELFORMATDESCRIPTOR pfd =				
	{
		sizeof(PIXELFORMATDESCRIPTOR),	// Size of this pixel format descriptor.
		1,								// Version number.
		PFD_DRAW_TO_WINDOW |			// Format must support window.
		PFD_SUPPORT_OPENGL |			// Format must support opengl.
		PFD_DOUBLEBUFFER,				// Must support double buffering.
		PFD_TYPE_RGBA,					// Request an rgba format.
		bits,							// Select our color depth.
		0, 0, 0, 0, 0, 0,				// Color bits ignored.
		0,								// No alpha buffer.
		0,								// Shift bit ignored.
		0,								// No accumulation buffer.
		0, 0, 0, 0,						// Accumulation bits ignored.
		16,								// 16bit z-buffer (depth buffer)  .
		0,								// No stencil buffer.
		0,								// No auxiliary buffer.
		PFD_MAIN_PLANE,					// Main drawing layer.
		0,								// Reserved.
		0, 0, 0							// Layer masks ignored.
	};

	return pfd;
}

void axWin32::ErrorMessageDialog(const string& msg)
{
	MessageBox(NULL, msg.c_str(), "ERROR", MB_OK | MB_ICONEXCLAMATION);
}