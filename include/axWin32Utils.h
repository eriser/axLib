#ifndef __AX_WIN32_UTILS__
#define __AX_WIN32_UTILS__

#include "axC++.h"

namespace axWin32
{
	RECT axRectToWin32Rect(const axRect& rect);

	// Windows Class Structure.
	WNDCLASS CreateDefaultWindowClassStructure(const char* label,
		HINSTANCE hinstance,
		WNDPROC WindowCallback);

	bool CreateHWNDWindow(HWND& win,
		HINSTANCE& hinstance,
		DWORD& style,
		DWORD& exStyle,
		RECT& win_rect,
		const char* title);

	void ErrorMessageDialog(const string& msg);

	PIXELFORMATDESCRIPTOR GetPixelFormatDescriptor(const int& bits);
}

#endif //__AX_WIN32_UTILS__