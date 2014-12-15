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
#include "axManager.h"
#include "axWindow.h"
#include "axMath.h"
#include "axApp.h"
#include "axCore.h"

axManager::axManager() :
// Heritage.
axMouseManager(),
axKeyboardManager()
{
    axMouseManager::SetWindowTree(&_windowTree);
    axKeyboardManager::SetWindowTree(&_windowTree);
}

axManager::~axManager()
{
}

void axManager::Add(axWindow* win)
{
	_windows.insert(axWindowPair(win->GetId(), win));
	_windowTree.AddWindow(win);
}

bool axManager::FindIfWindowNeedsToDraw(axWindow* win)
{
    bool need_drawing = true;
    
    if (win->IsShown())
    {
        deque<axWindow*> parents = _windowTree.GetWindowParents(win);
        
        // If one of the parent is hidden then we don't draw the window.
        for (auto& c : parents)
        {
            if (c->IsShown() == false)
            {
                need_drawing = false;
                break;
            }
        }
        
        return need_drawing;
    }
    else
    {
        return false;
    }
}

void axManager::OnPaint()
{
    _windowTree.DrawTree();
}

void axManager::OnFocusIn()
{

}

void axManager::OnUpdate()
{

}

void axManager::OnSize()
{
    for (auto& x : _windows)
    {
        axWindow* win = x.second;
        win->OnResize();
    }
}