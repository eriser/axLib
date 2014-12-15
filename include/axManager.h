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
#include "axObject.h"
#include "axC++.h"
#include "axWindowTree.h"

#include "axMouseManager.h"
#include "axKeyboardManager.h"

typedef std::map<axID, axWindow*> axWindowMap;
typedef std::pair<axID, axWindow*> axWindowPair;

class axWindow;

class axManager : public axMouseManager, public axKeyboardManager
{
public:
	axManager();
	~axManager();

	void Add(axWindow* win);

	// Events.
	void OnPaint();
	void OnFocusIn();
	void OnUpdate();
	void OnSize();

    std::string _managerName;

private:
	axWindowMap _windows; // Map for drawing windows.
	axWindowTree _windowTree;
    
    bool FindIfWindowNeedsToDraw(axWindow* win);
};

/// @}
#endif //__AX_MANAGER__
