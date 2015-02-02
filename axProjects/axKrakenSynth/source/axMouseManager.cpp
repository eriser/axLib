/*******************************************************************************
 * Copyright (c) 2013 Alexandre Arsenault.
 *
 * This file is part of axLibrary.
 *
 * axLibrary is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axLibrary License.
 *
 * axLibrary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axLibrary. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axLibrary, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 ******************************************************************************/
#include "axMouseManager.h"
#include "axWindow.h"
#include "axWindowTree.h"

axMouseManager::axMouseManager() :
	// Members
    _windowTree(nullptr),
	_mouseCaptureWindow(nullptr),
	_pastWindow(nullptr),
	_currentWindow(nullptr),
    _evtHasReachWindow(false)
{

}

void axMouseManager::SetWindowTree(axWindowTree* tree)
{
    _windowTree = tree;
}

bool axMouseManager::IsEventReachWindow() const
{
    return _evtHasReachWindow;
}


void axMouseManager::VerifyAndProcessWindowChange()
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

void axMouseManager::OnMouseLeftDragging(const axPoint& pos)
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

void axMouseManager::OnMouseMotion(const axPoint& pos)
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
		axWindow* win = _windowTree->FindMousePosition(pos);
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

void axMouseManager::OnMouseLeftDoubleClick(const axPoint& pos)
{
    _mousePosition = pos;
    
    if_not_null(_mouseCaptureWindow)
    {
        //_mouseCaptureWindow->OnMouseLeftDown(pos);
        //_evtHasReachWindow = true;
    }
    else
    {
        axWindow* win = _windowTree->FindMousePosition(pos);
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

void axMouseManager::OnMouseLeftDown(const axPoint& pos)
{
	_mousePosition = pos;

	if_not_null(_mouseCaptureWindow)
	{
		_mouseCaptureWindow->OnMouseLeftDown(pos);
        _evtHasReachWindow = true;
	}
	else
	{
		axWindow* win = _windowTree->FindMousePosition(pos);
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

void axMouseManager::OnMouseRightDown(const axPoint& pos)
{
    _mousePosition = pos;
    
    if_not_null(_mouseCaptureWindow)
    {
        _mouseCaptureWindow->OnMouseRightDown(pos);
        _evtHasReachWindow = true;
    }
    else
    {
        axWindow* win = _windowTree->FindMousePosition(pos);
        _currentWindow = win;
        
        if_not_null(win)
        {
            
            win->OnMouseRightDown(pos);
            _evtHasReachWindow = true;
        }
        else
        {
            _evtHasReachWindow = false;
        }
        
        VerifyAndProcessWindowChange();
        
    }
}

void axMouseManager::OnMouseLeftUp(const axPoint& pos)
{
	_mousePosition = pos;

	if_not_null(_mouseCaptureWindow)
	{
		_currentWindow = _windowTree->FindMousePosition(pos);
		_mouseCaptureWindow->OnMouseLeftUp(pos);
        _evtHasReachWindow = true;
        //return true;
	}
	else
	{
		axWindow* win = _windowTree->FindMousePosition(pos);
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

void axMouseManager::OnMouseRightUp()
{
	//DEBUG
//    return false;
    _evtHasReachWindow = false;
}

void axMouseManager::GrabMouse(axWindow* win)
{
	_mouseCaptureWindow = win;
}

void axMouseManager::UnGrabMouse()
{
	_mouseCaptureWindow = nullptr;
}

bool axMouseManager::IsGrab()
{
	return (_mouseCaptureWindow != nullptr);
}

bool axMouseManager::IsMouseHoverWindow(axWindow* win)
{
	return (_currentWindow == win);
}
