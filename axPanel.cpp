#include "axPanel.h"

axPanel::axPanel(axApp* app, axWindow* parent, const axRect& rect) : 
		axWindow(parent, rect)
{
    cout << "ADD window ct." << endl;
	if (parent != nullptr)
	{
		if (parent->GetIsPopup())
		{
			app->AddPopWindow(this);
			GetIsPopup() = true;
		}
		else
		{
            cout << "ADD window." << endl;
			app->AddWindow(this);
		}
	}
	else
	{
        cout << "ADD window first." << endl;
		app->AddWindow(this);
	}

	_app = app;
	//_isInPopup = false;
}

axPanel::axPanel(int f, axApp* app, axWindow* parent, const axRect& rect) :
axWindow(parent, rect)
{
	app->AddPopWindow(this);
	_app = app;
	GetIsPopup() = true;
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
	return _app;
}

void axPanel::GrabMouse()
{
	_app->GetWindowManager()->GrabMouse(this);
}

void axPanel::UnGrabMouse()
{
	_app->GetWindowManager()->UnGrabMouse();
}

void axPanel::UpdateAll()
{
	_app->UpdateAll();
}

void axPanel::Update()
{
	UpdateAll();
}

bool axPanel::IsGrabbed()
{
	return _app->GetWindowManager()->IsGrab();
}

bool axPanel::IsMouseHoverWindow()
{
	return _app->GetWindowManager()->IsMouseHoverWindow(this);
}

void axPanel::DeleteWindow(axWindow* win)
{
	//_app->GetWindowManager()->
}
