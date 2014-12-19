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
#include "axKeyboardManager.h"
#include "axWindow.h"
#include "axWindowTree.h"

axKeyboardManager::axKeyboardManager() :
	// Members
    _windowTree(nullptr),
    _keyGrabbedWindow(nullptr)
{

}

void axKeyboardManager::SetWindowTree(axWindowTree* tree)
{
    _windowTree = tree;
}

void axKeyboardManager::OnKeyDown(const char& key)
{
    //    std::cout << key << std::endl;
    if(_keyGrabbedWindow != nullptr)
    {
        _keyGrabbedWindow->OnKeyDown(key);
    }
}

void axKeyboardManager::OnEnterDown()
{
    if(_keyGrabbedWindow != nullptr)
    {
        _keyGrabbedWindow->OnEnterDown();
    }
}

void axKeyboardManager::OnKeyUp(const char& key)
{
    
}

void axKeyboardManager::OnBackSpaceDown()
{
    if(_keyGrabbedWindow != nullptr)
    {
        _keyGrabbedWindow->OnBackSpaceDown();
    }
}

void axKeyboardManager::GrabKey(axWindow* win)
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

void axKeyboardManager::UnGrabKey()
{
    if(_keyGrabbedWindow != nullptr)
    {
        _keyGrabbedWindow->OnWasKeyUnGrabbed();
        _keyGrabbedWindow = nullptr;
    }
    
}

bool axKeyboardManager::IsKeyGrab(axWindow* win) const
{
    return (win == _keyGrabbedWindow);
}

void axKeyboardManager::OnLeftArrowDown()
{
    if(_keyGrabbedWindow != nullptr)
    {
        _keyGrabbedWindow->OnLeftArrowDown();
    }
    
}

void axKeyboardManager::OnRightArrowDown()
{
    if(_keyGrabbedWindow != nullptr)
    {
        _keyGrabbedWindow->OnRightArrowDown();
    }
}

void axKeyboardManager::OnKeyDeleteDown()
{
    if(_keyGrabbedWindow != nullptr)
    {
        _keyGrabbedWindow->OnKeyDeleteDown();
    }
}



