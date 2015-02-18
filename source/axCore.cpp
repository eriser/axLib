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
#include "axCore.h"
#include "axGraphicInterface.h"

axCore::axCore():
// Members.
_needToDraw(true),
_popupNeedToDraw(true)
{
    _windowManager = nullptr;
    _popupManager = nullptr;
}

void axCore::InitManagers()
{
    _windowManager = new axManager();
    _windowManager->_managerName = std::string("WindowManager");
    _popupManager = new axManager();
    _popupManager->_managerName = std::string("PopupManager");
}

int axCore::InitGL()
{
    axGraphicInterface::Init();
    return true;
}

axManager* axCore::GetWindowManager()
{
	return _windowManager;
}

axManager* axCore::GetPopupManager()
{
	 return _popupManager;
}

void axCore::ResizeGLScene(const axSize& size)
{
    // Prevent a division by zero.
	_size = axSize(size.x, size.y == 0 ? 1 : size.y);
    
    axGraphicInterface::Resize(_size);
    
	_needToDraw = true;
	_popupNeedToDraw = true;
    
    GetWindowManager()->OnSize();
}

axSize axCore::GetGlobalSize() const
{
	return _size;
}

void axCore::SetGlobalSize(const axSize& size)
{
    _size = size;
}

void axCore::UpdateAll()
{
	_needToDraw = true;
	_popupNeedToDraw = true;
}

int axCore::DrawGLScene()
{
	if (_needToDraw == true)
	{
        axGraphicInterface::Draw(_size);
		
        GetWindowManager()->OnPaint();
        GetPopupManager()->OnPaint();
        
        _needToDraw = false;
        
		return true;
	}
	return false;
}