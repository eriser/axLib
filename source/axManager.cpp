#include "axManager.h"

#include "axPanel.h"

axManager::axManager() :
	// Members
	_mouseCaptureWindow(nullptr),
	_pastWindow(nullptr),
	_currentWindow(nullptr),
    _evtHasReachWindow(false),
    _keyGrabbedWindow(nullptr)
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

bool axManager::IsEventReachWindow() const
{
    return _evtHasReachWindow;
}

void axManager::Add(axWindow* win)
{
    if(win->GetIsPopup() == true)
    {
        std::cout << "axManager::Add in PopupManager." << std::endl;
    }
	_windows.insert(axWindowPair(win->GetId(), win));
	_windowTree.AddWindow(win);
}

void axManager::AddPriorityWindow(axWindow* win)
{
    
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
    
    // Should be order already (child ralative to parent).
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

void axManager::VerifyAndProcessWindowChange()
{
    if (_pastWindow != _currentWindow)
    {
        if_not_null(_currentWindow)
        {
            _currentWindow->OnMouseEnter();
        }
        
        if_not_null(_pastWindow)
        {
            _pastWindow->OnMouseLeave();
        }
        
        _pastWindow = _currentWindow;
    }
}

void axManager::OnMouseLeftDragging(const axPoint& pos)
{
    if(_mouseCaptureWindow != nullptr)
    {
        _mouseCaptureWindow->OnMouseLeftDragging(pos);
        _evtHasReachWindow = true;
    }
    else
    {
        _evtHasReachWindow = false;
    }
}

void axManager::OnMouseMotion(const axPoint& pos)
{
	_mousePosition = pos;

    // Only for Windows and Linux
	if_not_null(_mouseCaptureWindow)
	{
		_mouseCaptureWindow->OnMouseLeftDragging(pos);
        _evtHasReachWindow = true;
	}
	else
	{
		axWindow* win = _windowTree.FindMousePosition(pos);
		_currentWindow = win;

		if_not_null(_currentWindow)
		{
			_currentWindow->OnMouseMotion(pos);
            _evtHasReachWindow = true;
		}
        else
        {
            _evtHasReachWindow = false;
        }

        VerifyAndProcessWindowChange();
	}
}

void axManager::OnMouseLeftDoubleClick(const axPoint& pos)
{
    _mousePosition = pos;
    
    if_not_null(_mouseCaptureWindow)
    {
        //_mouseCaptureWindow->OnMouseLeftDown(pos);
        //_evtHasReachWindow = true;
    }
    else
    {
        axWindow* win = _windowTree.FindMousePosition(pos);
        _currentWindow = win;
        
        if_not_null(win)
        {
            win->OnMouseLeftDoubleClick(pos);
            _evtHasReachWindow = true;
        }
        else
        {
            _evtHasReachWindow = false;
        }
        
        VerifyAndProcessWindowChange();
    }
}

void axManager::OnMouseLeftDown(const axPoint& pos)
{
	_mousePosition = pos;

	if_not_null(_mouseCaptureWindow)
	{
		_mouseCaptureWindow->OnMouseLeftDown(pos);
        _evtHasReachWindow = true;
	}
	else
	{
		axWindow* win = _windowTree.FindMousePosition(pos);
		_currentWindow = win;

		if_not_null(win)
		{
            win->OnMouseLeftDown(pos);
            _evtHasReachWindow = true;
		}
        else
        {
            _evtHasReachWindow = false;
        }

        VerifyAndProcessWindowChange();

	}
}

void axManager::OnMouseLeftUp(const axPoint& pos)
{
	_mousePosition = pos;

	if_not_null(_mouseCaptureWindow)
	{
		_currentWindow = _windowTree.FindMousePosition(pos);
		_mouseCaptureWindow->OnMouseLeftUp(pos);
        _evtHasReachWindow = true;
        //return true;
	}
	else
	{
		axWindow* win = _windowTree.FindMousePosition(pos);
		_currentWindow = win;

		if_not_null(win)
		{
			win->OnMouseLeftUp(pos);
            _evtHasReachWindow = true;
		}
        else
        {
            _evtHasReachWindow = false;
        }

        VerifyAndProcessWindowChange();
	}
    
    //return false;
}

void axManager::OnMouseRightDown()
{
	//DEBUG
//    return false;
    _evtHasReachWindow = false;
}

void axManager::OnMouseRightUp()
{
	//DEBUG
//    return false;
    _evtHasReachWindow = false;
}

void axManager::OnFocusIn()
{
	//DEBUG
}

void axManager::OnUpdate()
{
	//DEBUG
}

void axManager::OnKeyDown(const char& key)
{
//    std::cout << key << std::endl;
    if(_keyGrabbedWindow != nullptr)
    {
        _keyGrabbedWindow->OnKeyDown(key);
    }
}

void axManager::OnKeyUp(const char& key)
{
    
}

void axManager::OnBackSpaceDown()
{
    if(_keyGrabbedWindow != nullptr)
    {
        _keyGrabbedWindow->OnBackSpaceDown();
    }
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

void axManager::GrabKey(axWindow* win)
{
    if(_keyGrabbedWindow != win)
    {
        if(_keyGrabbedWindow != nullptr)
        {
            _keyGrabbedWindow->OnWasKeyUnGrabbed();
        }
        
        win->OnWasKeyGrabbed();
    }
    
    _keyGrabbedWindow = win;
}

void axManager::UnGrabKey()
{
    if(_keyGrabbedWindow != nullptr)
    {
        _keyGrabbedWindow->OnWasKeyUnGrabbed();
        _keyGrabbedWindow = nullptr;
    }
    
}

bool axManager::IsKeyGrab(axWindow* win) const
{
    return (win == _keyGrabbedWindow);
}

void axManager::OnLeftArrowDown()
{
    if(_keyGrabbedWindow != nullptr)
    {
        _keyGrabbedWindow->OnLeftArrowDown();
    }

}

void axManager::OnRightArrowDown()
{
    if(_keyGrabbedWindow != nullptr)
    {
        _keyGrabbedWindow->OnRightArrowDown();
    }
}

void axManager::OnKeyDeleteDown()
{
    if(_keyGrabbedWindow != nullptr)
    {
        _keyGrabbedWindow->OnKeyDeleteDown();
    }
}


