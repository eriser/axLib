#ifndef __AX_WX_PANEL__
#define __AX_WX_PANEL__

#include "axConfig.h"

#if _axWxWidgetsCore_ == 1
#include "GL/glew.h"
#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include <vector>
#include "wx/glcanvas.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>

#include "axLib.h"

class axWxPanel : public wxGLCanvas, public axObject
{
public:
	/***************************************************************************
	* BasicGLPane::BasicGLPane.
	**************************************************************************/
	axWxPanel(wxPanel* parent,
		const wxPoint& pos,
		const wxSize& size,
		int* args);

	virtual ~axWxPanel();

	void resized(wxSizeEvent& evt);

	int getWidth();
	int getHeight();

	void render(wxPaintEvent& evt);

	// events
	void mouseMoved(wxMouseEvent& event);
	void mouseDown(wxMouseEvent& event);
	void mouseWheelMoved(wxMouseEvent& event);
	void mouseReleased(wxMouseEvent& event);
	void rightClick(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);

	void OnEvent(wxCommandEvent& event);

	DECLARE_EVENT_TABLE()

private:
	wxGLContext* m_context;
};
#endif // _axWxWidgetsCore_
#endif // __AX_WX_PANEL__