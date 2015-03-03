
#include "axConfig.h"
#if _axWxWidgetsCore_ == 1
#include "GL/glew.h"

#include "wx/glcanvas.h"

#include "axEventManager.h"
//#include "axApp.h"
//#include <Winuser.h>
//#include <Windowsx.h>
#include "wx/wx.h"
#include "axCoreWxWidgets.h"
axCoreWxWidgets::axCoreWxWidgets() : axCore()
{

}

void axCoreWxWidgets::Init(const axSize& frame_size)
{
	(frame_size);
	InitManagers();
	InitGL();
}

void axCoreWxWidgets::UpdateAll()
{
	if (_wxGLCanvas != nullptr)
	{
		_wxGLCanvas->Refresh();
	}
}

void axCoreWxWidgets::PushEventOnSystemQueue()
{
	wxCommandEvent btnEvent(wxEVT_COMMAND_BUTTON_CLICKED, 1000);
	wxPostEvent(_wxGLCanvas, btnEvent);
}

#endif //_axWxWidgetsCore_.