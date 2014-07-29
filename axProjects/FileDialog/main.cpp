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

void Desktop::FillDirectoryVector(const string& path)
{
	_dirName.clear();
	// _iconType.clear();

	DIR *d = opendir(path.c_str());
	struct dirent *dir;

	if(d)
	{
	while((dir = readdir(d)) != NULL)
	{
		if(dir->d_type == DT_REG)
		{
			// _iconType.push_back(ICON_FOLDER);
			// _dirName.push_back(FileInfo(dir->d_name, ICON_NONE));

			// string fn = dir->d_name;

			// string ext(fn.substr(fn.find_last_of(".") + 1));

			string ext(axGetExtension(dir->d_name));

 			if(ext == "c" || ext == "cpp") 
 			{
 				_dirName.push_back(FileInfo(dir->d_name, ICON_PROGFILE));
 			}
 			else if(ext == "png") 
 			{
 				_dirName.push_back(FileInfo(dir->d_name, ICON_PNG));
 			}
 			else
 			{
 				_dirName.push_back(FileInfo(dir->d_name, ICON_NONE));
 			}
		}
		else
		{
			// _iconType.push_back(ICON_NONE);
			_dirName.push_back(FileInfo(dir->d_name, ICON_FOLDER));
		}
	}

	closedir(d);
	}
	_dirName.pop_front();
	_dirName.pop_front();
	// _iconType.pop_front();
	// _iconType.pop_front();

	std::sort(_dirName.begin(), _dirName.end());

	_dirName.push_front(FileInfo(path, ICON_FOLDER));
}

Desktop::Desktop(axApp* app, axWindow* parent, const axRect& rect, const string& folder_path):
			axPanel(app, parent, rect)
{
	string path = app->GetCurrentAppDirectory();

	_selected_file = -1;
	// string path = GetCurrentAppDirectory();

	axButtonInfo btn_info(string(path + "ressources/axStandardButton.axobj"));

	function<void (axButtonMsg)> btnFct(GetOnBtn());

	axButton* b = new axButton(app, this, 
					 axRect(5, 5, 22, 22), 
					 axButtonEvents(btnFct), 
					 btn_info, path + "back.png", "",
					  axBUTTON_SINGLE_IMG | axBUTTON_IMG_RESIZE);

	b->SetBackgroundAlpha(0.5);

	if(folder_path != "")
	{
		FillDirectoryVector(folder_path);
	}
	else
	{
		FillDirectoryVector("/home/alexarse/Desktop/axLib/axProjects/FileDialog/none");
	}
	

	folder = new axImage(path + "folder.png");
	file_img = new axImage(path + "file.png");
	png_img = new axImage(path + "PNG-icon.png");
	// back = new axImage("back.png");
}

void Desktop::OnBtn(const axButtonMsg& msg)
{
	string back_path = _dirName[0].first;
	back_path = back_path.substr(0, back_path.find_last_of("/"));
	FillDirectoryVector(back_path);
	Update();
}

void Desktop::OnMouseMotion(const axPoint& mousePos)
{
	axPoint pos(mousePos - GetAbsoluteRect().position);

	double height = (_dirName.size()-1.0) * 24.0;

	if(pos.y > 50)
	{
		double y = double(pos.y - 50) / height;
		if(_dirName.size()-1)
		{
			int temp = y * (_dirName.size()-1);

			if(_selected_file != temp + 1)
			{
				_selected_file = temp + 1;
				Update();
			}
		}
		else
		{
			_selected_file = -1;
		}
	}
}


void Desktop::OnBtn2(const axButtonMsg& msg)
{
}

void Desktop::OnBtn3(const axButtonMsg& msg)
{
}

void Desktop::OnMouseLeftDown(const axPoint& mousePos)
{
	if(_selected_file != 0 && 
	   _selected_file != -1 && 
	   _selected_file < _dirName.size() && 
	   _dirName[_selected_file].second == ICON_FOLDER)
	{
		string path = _dirName[0].first + "/" + _dirName[_selected_file].first;

		FillDirectoryVector(path);
		Update();
	}
	else
	{
		if(_dirName[_selected_file].second != ICON_FOLDER)
		{
			string path = _dirName[0].first + "/" + _dirName[_selected_file].first;
			cout << path << endl;

			exit(0);
			// cout << "FILE : " << path << endl;
		}
	}
}

void Desktop::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.9, 0.9, 0.9), 1.0);
	gc->DrawRectangle(axRect(1, 1, rect0.size.x - 2, rect0.size.y -1));

	// gc->DrawRectangleColorFade(axRect(1, 1, rect0.size.x - 2, rect0.size.y -1), 
	// 						   axColor(0.3, 0.3, 0.3), 0.3, 
	// 						   axColor(0.8, 0.8, 0.8), 0.5);

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);

	// Icon bar.
	gc->SetColor(axColor("#777777"), 1.0);

	axRect iconBarRect(1, 1, rect0.size.x - 1, 31);
	gc->DrawRectangle(iconBarRect);
	gc->DrawRectangleColorFade(iconBarRect, 
							  axColor(0.3, 0.3, 0.3), 1.0, 
							  axColor(0.4, 0.4, 0.4), 1.0);

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(iconBarRect);

	// Folder name.
	gc->SetColor(axColor("#88888888"), 1.0);
	gc->DrawRectangle(axRect(1, 32, rect0.size.x - 2, 20));

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->SetFontSize(12);
	gc->DrawString(_dirName[0].first, axPoint(5, 32));


	// Folder Content.
	axColor col1(0.7, 0.7, 0.7);
	axColor col2(0.8, 0.8, 0.8);

	int y = 50;
	int x = 40;
	for(int i = 1; i < _dirName.size(); i++)
	{

		if(i < 30)
		{
			if(i % 2)
			{
				gc->SetColor(col1);
			}
			else
			{
				gc->SetColor(col2);
			}

			gc->DrawRectangle(axRect(1, y, rect0.size.x - 2, 24));

			if(_selected_file == i)
			{
				gc->SetColor(axColor(0.7, 0.7, 1.0), 0.5);
				gc->DrawRectangle(axRect(1, y, rect0.size.x - 2, 24));
			}

			if(_dirName[i].second == ICON_FOLDER)
			{
				gc->DrawImageResize(folder, axPoint(3, y), axSize(24, 24));
			}
			else if(_dirName[i].second == ICON_PROGFILE)
			{
				gc->DrawImageResize(file_img, axPoint(3, y), axSize(24, 24));
			}
			else if(_dirName[i].second == ICON_PNG)
			{
				gc->DrawImageResize(png_img, axPoint(3, y), axSize(24, 24));
			}

			gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
			gc->DrawString(_dirName[i].first, axPoint(x, y+4));
			y += 24;
		}
		else
		{
			break;
		}
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

int main(int argc, char* argv[])
{
	string path;

	// if(string(argv[0]) == string("main"))
	// {
		path = argv[0];
	// }

	// cerr << "ARGC : " << argc << endl;
	// cerr << "ARGV[0]" << argv[0] << endl;

	axApp app(axSize(500, 500));
	// SetNoBorder(&app);

	Desktop* desktop = new Desktop(&app, nullptr, 
								   axRect(0, 0, 500, 500), path);
	app.MainLoop();

	return 0;
}
