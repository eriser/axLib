#include "axPanel.h"

axPanel::axPanel(axApp* app, axWindow* parent, const axRect& rect) : 
		axWindow(parent, rect)
{
	app->AddWindow(this);
	_app = app;
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
