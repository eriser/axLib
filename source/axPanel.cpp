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
