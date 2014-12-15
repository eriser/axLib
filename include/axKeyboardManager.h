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
#ifndef __AX_KEYBOARD_MANAGER__
#define __AX_KEYBOARD_MANAGER__

/// @defgroup Core
/// @{

#include "axC++.h"

class axWindow;
class axWindowTree;

class axKeyboardManager
{
public:
	axKeyboardManager();
    
    void SetWindowTree(axWindowTree* tree);
    
    void OnKeyDown(const char& key);
    void OnKeyUp(const char& key);
    void OnKeyDeleteDown();
    void OnBackSpaceDown();
    void OnLeftArrowDown();
    void OnRightArrowDown();
    
    void GrabKey(axWindow* win);
    void UnGrabKey();
    
    bool IsKeyGrab(axWindow* win) const;


private:
	axWindowTree* _windowTree;
    axWindow* _keyGrabbedWindow;
};

/// @}
#endif //__AX_KEYBOARD_MANAGER__
