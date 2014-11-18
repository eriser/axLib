#include "axManager.h"

#include "axPanel.h"

axManager::axManager() :
	// Members
	_mouseCaptureWindow(nullptr),
	_pastWindow(nullptr),
	_currentWindow(nullptr)
{
	//DEBUG

	//_mouse = new axImage("C:/axGL/ressources/axImages/test.png");
}

axManager::~axManager()
{
	//_windows.clear();
}

void axManager::InitManager(const axSize& size)
{

}

void axManager::Add(axWindow* win)
{
	_windows.insert(axWindowPair(win->GetId(), win));
	_windowTree.AddWindow(win);
}

void axManager::OnPaint()
{
	/*for (auto& x : _windows)
	{
		axWindow* win = x.second;
		if (win->IsShown())
		{
			win->OnPaint();
		}
	}*/

//    cout << "Win manager paint. : " << _windows.size() << endl;
    
	for (auto& x : _windows)
	{
		axWindow* win = x.second;
		if (win->IsShown())
		{
			deque<axWindow*> parents = _windowTree.GetWindowParents(x.second);
			bool need_drawing = true;

			// If one of the parent is hidden then we don't draw the window.
			for (auto& c : parents)
			{
				if (c->IsShown() == false)
				{
					need_drawing = false;
					break;
				}
			}

			if (need_drawing)
			{
				x.second->OnPaint();
			}
		}
	}
}
void axManager::OnMouseLeftDragging(const axPoint& pos)
{
    if(_mouseCaptureWindow != nullptr)
    {
        _mouseCaptureWindow->OnMouseLeftDragging(pos);
    }
}

void axManager::OnMouseMotion(const axPoint& pos)
{
	_mousePosition = pos;

	if_not_null(_mouseCaptureWindow)
	{
		_mouseCaptureWindow->OnMouseLeftDragging(pos);
	}
	else
	{
		axWindow* win = _windowTree.FindMousePosition(pos);
		_currentWindow = win;

		if_not_null(win)
		{
			win->OnMouseMotion(pos);
		}

		// Generate MouseEnter and MouseLeave Event.
		if (_pastWindow != win)
		{
			if_not_null(win)
			{
				win->OnMouseEnter();
			}
			
			if_not_null(_pastWindow)
			{
				_pastWindow->OnMouseLeave();
			}

			_pastWindow = win;
		}
	}
}

void axManager::OnMouseLeftDown(const axPoint& pos)
{
	_mousePosition = pos;

	if_not_null(_mouseCaptureWindow)
	{
		_mouseCaptureWindow->OnMouseLeftDown(pos);
	}
	else
	{
		axWindow* win = _windowTree.FindMousePosition(pos);
		_currentWindow = win;

		if_not_null(win)
		{
			win->OnMouseLeftDown(pos);
		}

		if (_pastWindow != win)
		{
			if_not_null(win)
			{
				win->OnMouseEnter();
			}

			if_not_null(_pastWindow)
			{
				_pastWindow->OnMouseLeave();
			}

			_pastWindow = win;
		}
	}
}

void axManager::OnMouseLeftUp(const axPoint& pos)
{
	_mousePosition = pos;

	if_not_null(_mouseCaptureWindow)
	{
		_currentWindow = _windowTree.FindMousePosition(pos);
		_mouseCaptureWindow->OnMouseLeftUp(pos);
	}
	else
	{
		axWindow* win = _windowTree.FindMousePosition(pos);
		_currentWindow = win;

		if_not_null(win)
		{
			win->OnMouseLeftUp(pos);
		}

		if (_pastWindow != win)
		{
			if_not_null(win)
			{
				win->OnMouseEnter();
			}

			if_not_null(_pastWindow)
			{
				_pastWindow->OnMouseLeave();
			}

			_pastWindow = win;
		}
	}
}

void axManager::OnMouseRightDown()
{
	//DEBUG
}

void axManager::OnMouseRightUp()
{
	//DEBUG
}

void axManager::OnFocusIn()
{
	//DEBUG
}

void axManager::OnUpdate()
{
	//DEBUG
}

void axManager::OnSize()
{
    for (auto& x : _windows)
    {
        axWindow* win = x.second;
        win->OnResize();
    }
}

void axManager::GrabMouse(axWindow* win)
{
	_mouseCaptureWindow = win;
}

void axManager::UnGrabMouse()
{
	_mouseCaptureWindow = nullptr;
}

bool axManager::IsGrab()
{
	return (_mouseCaptureWindow != nullptr);
}

bool axManager::IsMouseHoverWindow(axWindow* win)
{
	return (_currentWindow == win);
}

//void axManager::DrawMouse(axImage* img, const axPoint& position)
//{
//	axPoint pos = position;;
//
//	glColor4f(1.0, 1.0, 1.0, 1.0);
//
//	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	glBindTexture(GL_TEXTURE_2D, img->GetTexture());
//	glDepthMask(GL_TRUE);
//	axSize img_size = img->GetSize();
//
//	// OpenGL stores texture upside down so glTexCoord2d must be flipped.
//	glBegin(GL_QUADS);
//	// Buttom left.
//	glTexCoord2d(0.0, 1.0);
//	glVertex2d(pos.x, pos.y);
//
//	// Top left.
//	glTexCoord2d(0.0, 0.0);
//	glVertex2d(pos.x, pos.y + img_size.y);
//
//	// Top right.
//	glTexCoord2d(1.0, 0.0);
//	glVertex2d(pos.x + img_size.x, pos.y + img_size.y);
//
//	// Buttom right.
//	glTexCoord2d(1.0, 1.0);
//	glVertex2d(pos.x + img_size.x, pos.y);
//	glEnd();
//	glDisable(GL_BLEND);
//	glDisable(GL_TEXTURE_2D);
//}