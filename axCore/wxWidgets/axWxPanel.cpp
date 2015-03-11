#include "axWxPanel.h"

#if _axWxWidgetsCore_ == 1
#include "axCoreWxWidgets.h"

BEGIN_EVENT_TABLE(axWxPanel, wxGLCanvas)
EVT_MOTION(axWxPanel::mouseMoved)
EVT_LEFT_DOWN(axWxPanel::mouseDown)
EVT_LEFT_UP(axWxPanel::mouseReleased)
EVT_RIGHT_DOWN(axWxPanel::rightClick)
EVT_LEAVE_WINDOW(axWxPanel::mouseLeftWindow)
EVT_SIZE(axWxPanel::resized)
EVT_KEY_DOWN(axWxPanel::keyPressed)
EVT_KEY_UP(axWxPanel::keyReleased)
EVT_MOUSEWHEEL(axWxPanel::mouseWheelMoved)
EVT_PAINT(axWxPanel::render)
EVT_BUTTON(1000, axWxPanel::OnEvent)
END_EVENT_TABLE()


// some useful events to use
void axWxPanel::mouseMoved(wxMouseEvent& event)
{
	axPoint pos(event.GetPosition().x, event.GetPosition().y);

	//std::cout << "Mouse pos : " << pos.x << " " << pos.y << std::endl;
	axApp::GetInstance()->GetWindowManager()->OnMouseMotion(pos);
}

void axWxPanel::mouseDown(wxMouseEvent& event)
{
	axPoint pos(event.GetPosition().x, event.GetPosition().y);

	if (event.LeftDown())
	{
		axApp::GetInstance()->GetWindowManager()->OnMouseLeftDown(pos);
	}
}

void axWxPanel::mouseWheelMoved(wxMouseEvent& event)
{
	(event);
}

void axWxPanel::mouseReleased(wxMouseEvent& event)
{
	axPoint pos(event.GetPosition().x, event.GetPosition().y);

	if (event.LeftUp())
	{
		axApp::GetInstance()->GetWindowManager()->OnMouseLeftUp(pos);
	}
}

void axWxPanel::rightClick(wxMouseEvent& event)
{
	(event);
}

void axWxPanel::mouseLeftWindow(wxMouseEvent& event)
{
	(event);
}

void axWxPanel::keyPressed(wxKeyEvent& event)
{
	(event);
}

void axWxPanel::keyReleased(wxKeyEvent& event)
{
	(event);
}

void axWxPanel::OnEvent(wxCommandEvent& event)
{
	(event);
	std::cout << "Event" << std::endl;
	axEventManager::GetInstance()->CallNext();
}


axWxPanel::axWxPanel(wxPanel* parent,
						 const wxPoint& pos, 
						 const wxSize& size, 
						 int* args) :
wxGLCanvas(parent, wxID_ANY, args, pos, size, wxFULL_REPAINT_ON_RESIZE)
{
	m_context = new wxGLContext(this);
	wxGLCanvas::SetCurrent(*m_context);

	//Initialize GLEW
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	}

	//Make sure OpenGL 2.1 is supported
	if (!GLEW_VERSION_2_1)
	{
		printf("OpenGL 2.1 not supported!\n");
	}
	
	axEventManager::GetInstance();
	axApp* app = axApp::CreateApp(axSize(size.x, size.y));
	static_cast<axCoreWxWidgets*>(app->GetCore())->SetWxGlCanvas(this);

	// To avoid flashing on MSW
	SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

axWxPanel::~axWxPanel()
{
	delete m_context;
}

void axWxPanel::resized(wxSizeEvent& evt)
{
	(evt);
	Refresh();
}

int axWxPanel::getWidth()
{
	return GetSize().x;
}

int axWxPanel::getHeight()
{
	return GetSize().y;
}

void axWxPanel::render(wxPaintEvent& evt)
{
	(evt);
	if (!IsShown())
	{
		return;
	}

	wxGLCanvas::SetCurrent(*m_context);
	wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event

	axCore* core = axApp::GetInstance()->GetCore();

	wxSize size(GetSize());
	core->ResizeGLScene(axSize(size.x, size.y));
	
	core->DrawGLScene();

	glFlush();
	SwapBuffers();
}

#endif // _axWxWidgetsCore_.