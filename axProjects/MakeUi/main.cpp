#include "main.h"

#include <sys/types.h>
#include <unistd.h>

Desktop::Desktop(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect)
{

	// function<void (axButtonMsg)> btnFct(GetOnBtn());

	vector<axButton*> btns;
	// axButton* btn = new axButton(app, this, 
	// 							 axRect(20, 20, 30, 30), 
	// 							 axButtonEvents(btnFct), 
	// 							 axButtonInfo(axColor(1.0, 0.0, 0.0),
	// 										  axColor(1.0, 1.0, 0.0),
	// 										  axColor(1.0, 1.0, 1.0),
	// 										  axColor(0.0, 1.0, 0.0),
	// 										  axColor(1.0, 1.0, 1.0),
	// 										  axColor(1.0, 0.0, 1.0)));


	axButtonInfo btn_info(axColor(1.0, 0.0, 0.0),
						  axColor(1.0, 1.0, 0.0),
						  axColor(1.0, 1.0, 1.0),
						  axColor(0.0, 1.0, 0.0),
						  axColor(1.0, 1.0, 1.0),
						  axColor(1.0, 0.0, 1.0));

	function<void (axButtonMsg)> btnFct(GetOnBtn());

	btns.push_back(new axButton(app, this, 
								 axRect(20, 20, 30, 30), 
								 axButtonEvents(btnFct), 
								 btn_info));

	//function<void (axButtonMsg)> btnFct(GetOnBtn2());
	btnFct = GetOnBtn2();
	btns.push_back(new axButton(app, this, 
							 axRect(55, 20, 30, 30), 
							 axButtonEvents(btnFct), 
							 btn_info));

	btnFct = GetOnBtn3();
	btns.push_back(new axButton(app, this, 
							 axRect(90, 20, 30, 30), 
							 axButtonEvents(btnFct), 
							 btn_info));

}

void Desktop::OnBtn(const axButtonMsg& msg)
{
	pid_t p_id = fork();

	// Child.
	if (p_id == 0)                
	{
	  // // Code only executed by child process

	  // sIdentifier = "Child Process: ";
	  // globalVariable++;
	  // iStackVariable++;
		cout << "Click child" << endl;

		//execl(const char *path, const char *arg0, const char *arg1, const char *arg2, ... const char *argn, (char *) 0);
		execl("/home/alexarse/Desktop/axLib/axProjects/MakeUi/main", 0);
		// execl("/bin/bash/", "subl", 0);
		//execl("/usr/bin/gedit", 0);
		exit(0);
	}

	// Failed to fork.
	else if (p_id < 0)            
	{
	    cerr << "Failed to fork" << endl;
	    exit(1);
	    // Throw exception
	}

	// Parent.
	else                                   
	{
		cout << "Click." << endl;
	  // Code only executed by parent process
	  // sIdentifier = "Parent Process:";
	}
}


void Desktop::OnBtn2(const axButtonMsg& msg)
{
	pid_t p_id = fork();

	// Child.
	if (p_id == 0)                
	{
	  // // Code only executed by child process
		// cout << "Click child" << endl;

		//execl(const char *path, const char *arg0, const char *arg1, const char *arg2, ... const char *argn, (char *) 0);
		int d = static_cast<axCoreX11*>(GetApp()->GetCore())->GetWindow();
		string arg = std::to_string(d);
		execl("/home/alexarse/Desktop/axLib/axProjects/Calculator/main", arg.c_str(), arg.c_str(), 0);

		exit(0);
	}

	// Failed to fork.
	else if (p_id < 0)            
	{
	    cerr << "Failed to fork" << endl;
	    exit(1);
	    // Throw exception
	}
}

void Desktop::OnBtn3(const axButtonMsg& msg)
{
	pid_t p_id = fork();

	// Child.
	if (p_id == 0)                
	{
	  // // Code only executed by child process
		// cout << "Click child" << endl;

		//execl(const char *path, const char *arg0, const char *arg1, const char *arg2, ... const char *argn, (char *) 0);
		int d = static_cast<axCoreX11*>(GetApp()->GetCore())->GetWindow();
		string arg = std::to_string(d);
		execl("/usr/bin/xterm", 0);

		exit(0);
	}

	// Failed to fork.
	else if (p_id < 0)            
	{
	    cerr << "Failed to fork" << endl;
	    exit(1);
	    // Throw exception
	}
}

void Desktop::OnPaint()
{
	//cout << "test" << endl;
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.7, 0.7, 0.7), 1.0);
	gc->DrawRectangle(rect0);
// cout << "test" << endl;
	// gc->SetColor(axColor(0.9, 0.9, 0.9));
	// gc->SetFontType("FreeSans.ttf");
	// gc->SetFontSize(13);
	// gc->DrawStringAlignedCenter("Alexandre", axRect(5, 5, 100, 30));
}

int main()
{
	axApp app(axSize(300, 300));
	Desktop* desktop = new Desktop(&app, nullptr, 
								   axRect(0, 0, 300, 300));
	app.MainLoop();

	return 0;
}
