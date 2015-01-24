#include "main.h"

#include <sys/types.h>
#include <unistd.h>

Desktop::Desktop(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect)
{
	char buf[1024];
	readlink("/proc/self/exe", buf, sizeof(buf)-1);
	string path(buf);
	path = path.substr(0, path.find_last_of("/"));
	path.push_back('/');

	function<void (axButtonMsg)> btnFct(GetOnBtn());

	// vector<axButton*> btns;
	axButton* btn = new axButton(app, this, 
								 axRect(10, 5, 15, 15), 
								 axButtonEvents(btnFct), 
								 axButtonInfo(axColor(0.5, 0.5, 0.5),
											  axColor(0.5, 0.5, 0.5),
											  axColor(0.5, 0.5, 0.5),
											  axColor(0.5, 0.5, 0.5),
											  axColor(0.5, 0.5, 0.5),
											  axColor(0.5, 0.5, 0.5)),
								 path+string("x.png"));


	btnFct = GetOnBtn();
	axButton* btn2 = new axButton(app, this, 
							 axRect(30, 5, 15, 15), 
							 axButtonEvents(btnFct), 
							 axButtonInfo(axColor(0.5, 0.5, 0.5),
										  axColor(0.5, 0.5, 0.5),
										  axColor(0.5, 0.5, 0.5),
										  axColor(0.5, 0.5, 0.5),
										  axColor(0.5, 0.5, 0.5),
										  axColor(0.5, 0.5, 0.5)),
							 path+string("x.png"));



	btn->SetBackgroundAlpha(0.0);
	btn2->SetBackgroundAlpha(0.0);
}

void Desktop::OnBtn(const axButtonMsg& msg)
{
	cout << "TESTTTTTT" << endl;
}


void Desktop::OnBtn2(const axButtonMsg& msg)
{
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
	int win = 0;
	int width = 300, height = 300;

	if(argc == 4)
	{
		win = stoi(argv[1]);
		width = stoi(argv[2]);
		height = stoi(argv[3]);
	}

	if(win != 0)
	{
		axApp app(axSize(width + 6, height + 40));

		Window parent = static_cast<axCoreX11*>(app.GetCore())->GetWindow();
		Display* dpy = static_cast<axCoreX11*>(app.GetCore())->GetDisplay();

		Desktop* desktop = new Desktop(&app, nullptr, 
								   axRect(0, 0, width + 6, height + 40));

		XReparentWindow(dpy, (Window)win, parent, 3, 30);

		app.MainLoop();
	}

	return 0;
}
