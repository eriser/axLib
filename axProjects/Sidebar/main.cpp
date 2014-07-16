#include "main.h"

#include <sys/types.h>
#include <unistd.h>


string GetCurrentAppDirectory()
{
	char buf[1024];
	readlink("/proc/self/exe", buf, sizeof(buf)-1);
	string path(buf);
	path = path.substr(0, path.find_last_of("/"));
	path.push_back('/');

	return path;
}

void ExecApplication(const string& app_name)
{
	pid_t p_id = fork();

	// Child.
	if (p_id == 0)                
	{
		execl(app_name.c_str(), 0);
		exit(0);
	}

	// Failed to fork.
	else if (p_id < 0)            
	{
	    cerr << "Failed to open " << app_name << endl;
	    exit(1);
	    // Throw exception
	}
}

Desktop::Desktop(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect)
{
	string path = GetCurrentAppDirectory();

	_bgImg = new axImage(path+string("woodSide.png"));

	axButtonInfo btn_info(path+string("ressources/axStandardButton.axobj"));

	function<void (axButtonMsg)> btnFct(GetOnBtn());

	int x = 7, y = 6, y_delta = 45;

	axButton* b = nullptr;

	b = new axButton(app, this, 
					 axRect(x, y, 32, 32), 
					 axButtonEvents(btnFct), 
					 btn_info, path+string("folder.png"), "", 
					 axBUTTON_SINGLE_IMG);

	_btns.push_back(b);
	y += y_delta;
	b->SetBackgroundAlpha(0.4);


	btnFct = GetOnBtn2();
	b = new axButton(app, this, axRect(x, y, 32, 32), 
							    axButtonEvents(btnFct), 
							    btn_info, path+string("calculator2.png"), "", 
								axBUTTON_SINGLE_IMG);
	_btns.push_back(b);
	y += y_delta;
	b->SetBackgroundAlpha(0.4);
}

void Desktop::OnBtn(const axButtonMsg& msg)
{
	ExecApplication("/home/alexarse/Desktop/axLib/axProjects/MakeUi/main");
}


void Desktop::OnBtn2(const axButtonMsg& msg)
{
	ExecApplication("/home/alexarse/Desktop/axLib/axProjects/Calculator/main");
}

void Desktop::OnBtn3(const axButtonMsg& msg)
{

}

void Desktop::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor("#777777"), 1.0);
	gc->DrawRectangle(rect0);

	gc->DrawRectangleColorFade(rect0,
		axColor(0.92, 0.92, 0.92), 0.8,
		axColor(0.5, 0.5, 0.5), 0.5);

	if (_bgImg->IsImageReady())
	{
			gc->DrawImageResize(_bgImg, axPoint(0, 0), rect0.size, 1.0);
	}
}

void SetNoBorder(axApp* app)
{
	typedef struct Hints
	{
	    unsigned long   flags;
	    unsigned long   functions;
	    unsigned long   decorations;
	    long            inputMode;
	    unsigned long   status;
	} Hints;
	
	Window win = static_cast<axCoreX11*>(app->GetCore())->GetWindow();
	Display* dpy = static_cast<axCoreX11*>(app->GetCore())->GetDisplay();

	// Code to remove decoration.
	Hints hints;
	Atom property;
	hints.flags = 2;
	hints.decorations = 0;
	property = XInternAtom(dpy, "_MOTIF_WM_HINTS", true);
	XChangeProperty(dpy, win, property, property, 32, 
					PropModeReplace, (unsigned char *)&hints, 5);
}

int main()
{
	axApp app(axSize(45, 700));
	SetNoBorder(&app);

	Desktop* desktop = new Desktop(&app, nullptr, 
								   axRect(0, 0, 45, 700));
	app.MainLoop();

	return 0;
}
