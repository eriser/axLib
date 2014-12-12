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
#ifndef __AX_MANAGER__
#define __AX_MANAGER__

/// @defgroup Core
/// @{

#include <map>

//#include "axID.h"
#include "axC++.h"
#include "axWindow.h"
#include "axWindowTree.h"
//#include "axEventManager.h"

typedef std::map<axID, axWindow*> axWindowMap;
typedef std::pair<axID, axWindow*> axWindowPair;

class axManager
{
public:
	axManager();
	~axManager();

	void InitManager(const axSize& size);

	void Add(axWindow* win);
    void AddPriorityWindow(axWindow* win);

	bool IsGrab();
	bool IsMouseHoverWindow(axWindow* win);

	// Events.
	void OnPaint();
	void OnFocusIn();
	void OnUpdate();
	void OnSize();

	// Mouse events.
	void OnMouseMotion(const axPoint& pos);
    void OnMouseLeftDragging(const axPoint& pos);
	void OnMouseLeftDown(const axPoint& pos);
    void OnMouseLeftDoubleClick(const axPoint& pos);
	void OnMouseLeftUp(const axPoint& pos);
	void OnMouseRightDown();
	void OnMouseRightUp();
	void GrabMouse(axWindow* win);
	void UnGrabMouse();

    // Keyboard events.
    // void OnCtrl();
    // void OnAlt();
    // void OnShift();
    void OnKeyDown(const char& key);
    void OnKeyUp(const char& key);
    void OnKeyDeleteDown();
    void OnBackSpaceDown();
    void OnLeftArrowDown();
    void OnRightArrowDown();
    
    void GrabKey(axWindow* win);
    void UnGrabKey();
    
    bool IsKeyGrab(axWindow* win) const;

    // bool IsCtrlDown();
    // bool IsAltDown();
    // bool IsShiftDown();
    
    bool IsEventReachWindow() const;

    std::string _managerName;

private:
	axWindowMap _windows; // Map for drawing windows.
	axWindowTree _windowTree;

	// Mouse.
	axWindow* _mouseCaptureWindow;
	axWindow* _pastWindow;
	axWindow* _currentWindow;
	axPoint _mousePosition;
    
    axWindow* _keyGrabbedWindow;
    
    bool _evtHasReachWindow;
    
    
    
    void VerifyAndProcessWindowChange();

};

/// @}
#endif //__AX_MANAGER__
