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
#ifndef __AX_MOUSE_MANAGER__
#define __AX_MOUSE_MANAGER__

/// @defgroup Core
/// @{

#include "axC++.h"

class axWindow;
class axWindowTree;

class axMouseManager
{
public:
	axMouseManager();
    
    void SetWindowTree(axWindowTree* tree);

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
    
    bool IsGrab();
    bool IsMouseHoverWindow(axWindow* win);
    bool IsEventReachWindow() const;

private:
	axWindowTree* _windowTree;

	// Mouse.
	axWindow* _mouseCaptureWindow;
	axWindow* _pastWindow;
	axWindow* _currentWindow;
	axPoint _mousePosition;
    
    bool _evtHasReachWindow;
    void VerifyAndProcessWindowChange();
};

/// @}
#endif //__AX_MOUSE_MANAGER__
