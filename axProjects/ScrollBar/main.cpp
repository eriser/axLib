#include "main.h"

#include <sys/types.h>
#include <unistd.h>

ScrollWindow::ScrollWindow(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect)
{
	axButtonInfo btn_info(axColor(0.8, 0.0, 0.0),
						  axColor(0.9, 0.0, 0.0),
						  axColor(0.8, 0.0, 0.0),
						  axColor(0.8, 0.0, 0.0),
						  axColor(0.0, 0.0, 0.0),
						  axColor(0.0, 0.0, 0.0));

	// function<void (axButtonMsg)> btnFct(GetOnColorChange());

	// int x = 0, y = 0, xDelta = 20;

	axButton* btn = new axButton(app, this, 
								 axRect(0, 0, 50, 50), 
								 axButtonEvents(), 
								 btn_info, "", "Test");

	axButton* btn2 = new axButton(app, this, 
								 axRect(0, rect.size.y * 0.5, 50, 50), 
								 axButtonEvents(), 
								 btn_info, "", "Test");
}


void ScrollWindow::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(1.0, 0.0, 0.0));
	gc->DrawRectangle(axRect(0, 0, rect.size.x, rect.size.y * 0.5));

	gc->SetColor(axColor(1.0, 1.0, 0.0));
	gc->DrawRectangle(axRect(0, rect.size.y * 0.5, rect.size.x, rect.size.y * 0.5));
}

MyWindow::MyWindow(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect)
{
	function<void (axScrollBarMsg)> scroll(GetOnScroll());
	scroll_bar = new axScrollBar(app, this, 
								 axRect(0, 0, 14, rect.size.y), 
								 axScrollBarEvents(scroll), 
								 axScrollBarInfo());


	scroll_bar->setInputInfo(rect.size.y, 1000, 0);

	scroll_win = new ScrollWindow(app, this, axRect(14, 0, rect.size.x - 14, 1000));
}

void MyWindow::OnScroll(const axScrollBarMsg& msg)
{
	cout << msg.GetMsg() << endl;
	// scroll_win->SetScrollDecay(axPoint(0, stof(msg.GetMsg())));
	scroll_win->SetPosition(axPoint(14, - stof(msg.GetMsg())));
}

void MyWindow::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(1.0, 0.0, 0.0));
	gc->DrawRectangle(rect0);
}

Desktop::Desktop(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect)
{
	mywin = new MyWindow(app, this, axRect(50, 50, 300, 300));
}

void Desktop::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->DrawRectangleColorFade(
		axRect(0, 30, rect0.size.x, rect0.size.y - 30),
		axColor(0.5, 0.5, 0.5), 1,
		axColor(0.3, 0.3, 0.3), 1);

	gc->DrawRectangleColorFade(
		axRect(0, 0, rect0.size.x, 30),
		axColor(0.9, 0.9, 0.9), 1,
		axColor(0.5, 0.5, 0.5), 1);
}

int main(int argc, const char* argv[])
{
	axApp app(axSize(500, 500));

	Desktop* desktop = new Desktop(&app, nullptr, 
							   axRect(0, 0, 500, 500));

	app.MainLoop();
	

	return 0;
}
