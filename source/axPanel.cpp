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
#include "axPanel.h"
#include "axApp.h"

axPanel::axPanel(axWindow* parent, const axRect& rect) : 
		axWindow(parent, rect)
{
	if (parent != nullptr)
	{
		if (parent->GetIsPopup())
		{
            GetIsPopup() = true;
			axApp::GetInstance()->AddPopWindow(this);
		}
		else
		{
			axApp::GetInstance()->AddWindow(this);
		}
	}
	else
	{
        if(GetIsPopup())
        {
            axApp::GetInstance()->AddPopWindow(this);
        }
        else
        {
            axApp::GetInstance()->AddWindow(this);
        }
		
	}

	//_app = app;
	//_isInPopup = false;
}

axPanel::axPanel(int f, axWindow* parent, const axRect& rect) :
axWindow(parent, rect)
{
    //std::cout << "axPanel : Add popup window" << std::endl;
    GetIsPopup() = true;
	axApp::GetInstance()->AddPopWindow(this);
}

void axPanel::BlockDrawing()
{
	axRect absRect(GetAbsoluteRect());
	axSize gSize(GetApp()->GetCore()->GetGlobalSize());
	
	glScissor(absRect.position.x-1, 
			  gSize.y - absRect.position.y - absRect.size.y,
			  absRect.size.x+1, 
			  absRect.size.y+1);

	glEnable(GL_SCISSOR_TEST);

}

void axPanel::BlockDrawing(const axRect& rect)
{
	axSize gSize(GetApp()->GetCore()->GetGlobalSize());
	
	glScissor(rect.position.x-1, 
			  gSize.y - rect.position.y - rect.size.y, 
			  rect.size.x+1, 
			  rect.size.y+1);

	glEnable(GL_SCISSOR_TEST);
}

void axPanel::UnBlockDrawing()
{
	glDisable(GL_SCISSOR_TEST);
}

axApp* axPanel::GetApp()
{
	//return _app;
	return axApp::GetInstance();

}

void axPanel::ShowMouse()
{
    axApp::GetInstance()->GetCore()->ShowMouse();
}

void axPanel::HideMouse()
{
    axApp::GetInstance()->GetCore()->HideMouse();
}

void axPanel::GrabMouse()
{
    if(GetIsPopup())
    {
        axApp::GetInstance()->GetPopupManager()->GrabMouse(this);
    }
    else
    {
        axApp::GetInstance()->GetWindowManager()->GrabMouse(this);
    }
	
}

void axPanel::UnGrabMouse()
{
    if(GetIsPopup())
    {
        axApp::GetInstance()->GetPopupManager()->UnGrabMouse();
    }
    else
    {
        axApp::GetInstance()->GetWindowManager()->UnGrabMouse();
    }
	
}

void axPanel::GrabKey()
{
    if(GetIsPopup())
    {
        axApp::GetInstance()->GetPopupManager()->GrabKey(this);
    }
    else
    {
        axApp::GetInstance()->GetWindowManager()->GrabKey(this);
    }
}

void axPanel::UnGrabKey()
{
    if(GetIsPopup())
    {
        axApp::GetInstance()->GetPopupManager()->UnGrabKey();
    }
    else
    {
        axApp::GetInstance()->GetWindowManager()->UnGrabKey();
    }
}

bool axPanel::IsKeyGrab()
{
    if(GetIsPopup())
    {
        return axApp::GetInstance()->GetPopupManager()->IsKeyGrab(this);
    }
    else
    {
        return axApp::GetInstance()->GetWindowManager()->IsKeyGrab(this);
    }
    
}

void axPanel::UpdateAll()
{
	axApp::GetInstance()->UpdateAll();
}

void axPanel::Update()
{
	UpdateAll();
}

bool axPanel::IsGrabbed()
{
    if(GetIsPopup())
    {
        return axApp::GetInstance()->GetPopupManager()->IsGrab();
    }
    else
    {
        return axApp::GetInstance()->GetWindowManager()->IsGrab();
    }
	
}

bool axPanel::IsMouseHoverWindow()
{
    if(GetIsPopup())
    {
        return axApp::GetInstance()->GetPopupManager()->IsMouseHoverWindow(this);
    }
    else
    {
        return axApp::GetInstance()->GetWindowManager()->IsMouseHoverWindow(this);
    }
	
}

void axPanel::DeleteWindow(axWindow* win)
{
	//_app->GetWindowManager()->
}
