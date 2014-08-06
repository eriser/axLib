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

DirectoryNavigation::DirectoryNavigation(const string& folder_path):
					 _currentDirectory(folder_path)
{
	SetDestinationPath(folder_path);
}

void DirectoryNavigation::SetDestinationPath(const string& path)
{
	_currentDirectory = path;

	_dirName.clear();

	DIR *d = opendir(path.c_str());
	dirent* dir;

	if(d)
	{
		while((dir = readdir(d)) != NULL)
		{
			IconType icon = FindType(dir);
			_dirName.push_back(FileInfo(dir->d_name, icon));
		}

		closedir(d);
	}

	_dirName.pop_front();
	_dirName.pop_front();

	std::sort(_dirName.begin(), _dirName.end());
}

string DirectoryNavigation::GetCurrentDirectoryName() const
{
	return _currentDirectory;
}

deque<DirectoryNavigation::FileInfo>* DirectoryNavigation::GetFileInfoDeque()
{
	return &_dirName;
}

DirectoryNavigation::IconType DirectoryNavigation::FindType(dirent* dir)
{
	if(dir->d_type == DT_REG)
	{
		string ext(axGetExtension(dir->d_name));

		if(ext == "c" || ext == "cpp") 
		{
			// _dirName.push_back(FileInfo(dir->d_name, ICON_PROGFILE));
			return ICON_PROGFILE;
		}
		else if(ext == "png") 
		{
			// _dirName.push_back(FileInfo(dir->d_name, ICON_PNG));
			return ICON_PNG;
		}
		else if(ext == "seq") 
		{
			// _dirName.push_back(FileInfo(dir->d_name, ICON_PNG));
			return ICON_MIDI;
		}
		else
		{
			// _dirName.push_back(FileInfo(dir->d_name, ICON_NONE));
			return ICON_NONE;
		}
	}
	else
	{
		// _iconType.push_back(ICON_NONE);
		//_dirName.push_back(FileInfo(dir->d_name, ICON_FOLDER));
		return ICON_FOLDER;
	}
}

void DirectoryNavigation::PrintDirectoryContent()
{
	for(auto& i : _dirName)
	{
		cout << i.first << endl;
	}
}

string DirectoryNavigation::GetParentDirectory() const
{
	return _currentDirectory.substr(0, _currentDirectory.find_last_of("/"));
}

void DirectoryNavigation::GoToParentDirectory()
{
	SetDestinationPath(GetParentDirectory());
}

void DirectoryNavigation::GoToFolder(const string& folder_name)
{
	string path = _currentDirectory + "/" + folder_name;
	SetDestinationPath(path);
}

FolderContent::FolderContent(axApp* app, axWindow* parent, const axRect& rect, DirectoryNavigation* navigation):
							 axPanel(app, parent, rect),
							 _dirNavigation(navigation)
{
	string path = app->GetCurrentAppDirectory();

	_selected_file = -1;

	// SetScrollDecay(axPoint(0, 50));

	_icons.insert(IconPair(DirectoryNavigation::ICON_FOLDER, 
						   new axImage(path + "folder.png")));

	_icons.insert(IconPair(DirectoryNavigation::ICON_PROGFILE, 
						   new axImage(path + "file.png")));

	_icons.insert(IconPair(DirectoryNavigation::ICON_PNG, 
						   new axImage(path + "PNG-icon.png")));

	_icons.insert(IconPair(DirectoryNavigation::ICON_MIDI, 
						   new axImage(path + "midi.png")));
}

void FolderContent::OnMouseMotion(const axPoint& mousePos)
{
	axPoint pos(mousePos - GetAbsoluteRect().position);
	pos += GetScrollDecay();

	deque<DirectoryNavigation::FileInfo>& dirNames = *_dirNavigation->GetFileInfoDeque();

	double height = dirNames.size() * 24.0;

	if(pos.y > 0)
	{
		double y = double(pos.y - 0) / height;
		
		if(dirNames.size())
		{
			int temp = y * (dirNames.size());

			if(_selected_file != temp)
			{
				_selected_file = temp;
				Update();
			}
		}
		else
		{
			_selected_file = -1;
		}
	}
}

void FolderContent::OnMouseLeftDown(const axPoint& mousePos)
{
	deque<DirectoryNavigation::FileInfo>& dirNames = *_dirNavigation->GetFileInfoDeque();

	// If a folder is selected.
	if(_selected_file != -1 && _selected_file < dirNames.size() && 
	   dirNames[_selected_file].second == DirectoryNavigation::ICON_FOLDER)
	{
		_dirNavigation->GoToFolder(dirNames[_selected_file].first);
		Update();
	}
	else
	{
		// If file is selected.
		if(dirNames[_selected_file].second != DirectoryNavigation::ICON_FOLDER)
		{
			string path = _dirNavigation->GetCurrentDirectoryName() + 
						  "/" + dirNames[_selected_file].first;

			// This cout is use for interprocess communication.
			// DO NOT REMOVE.
			cout << path << endl;

			exit(0);
		}
	}
}

void FolderContent::OnPaint()
{
	BlockDrawing();

	axGC* gc = GetGC();
	axRect rect0(axPoint(0, 0), GetRect().size);

	gc->SetColor(axColor(0.9, 0.9, 0.9), 1.0);
	gc->DrawRectangle(axRect(1, 1, rect0.size.x - 2, rect0.size.y -1));

	// Folder Content.
	axColor col1(0.7, 0.7, 0.7);
	axColor col2(0.8, 0.8, 0.8);

	int y = 0;
	int x = 40;
	gc->SetFontSize(12);

	deque<DirectoryNavigation::FileInfo>& dirNames = *_dirNavigation->GetFileInfoDeque();

	for(int i = 0; i < dirNames.size(); i++)
	{
		if(i % 2) gc->SetColor(col1);
		else gc->SetColor(col2);
		
		// Draw file background.
		axRect backfileRect(1, y, rect0.size.x - 1, 24);
		gc->DrawRectangle(backfileRect);

		// Draw file highlight.
		if(_selected_file == i)
		{
			gc->SetColor(axColor(0.7, 0.7, 1.0), 0.5);

			axRect selected_rect(1, y, rect0.size.x - 2, 24);
			gc->DrawRectangle(selected_rect);
			gc->DrawRectangleColorFade(selected_rect, 
						  			   axColor(0.7, 0.7, 0.7), 0.5, 
						 			   axColor(0.8, 0.8, 0.8), 0.5);
		}

		// Draw file icon.
		if(dirNames[i].second != DirectoryNavigation::ICON_NONE)
		{
			gc->DrawImageResize(_icons[dirNames[i].second], axPoint(3, y), axSize(24, 24));
		}

		// Draw file name.
		gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
		gc->DrawString(dirNames[i].first, axPoint(x, y+4));

		y += 24;
	
		if(i > 30)
		{
			break;
		}
	}

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(axRect(1, 1, rect0.size.x - 1, rect0.size.y - 1));

	UnBlockDrawing();
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
	// cout << "TEST" << endl;
	// if(string(argv[0]) == string("main"))
	// {
		path = argv[0];
	// }

	axApp app(axSize(500, 530));
	SetNoBorder(&app);

	TitleBar* titlebar = new TitleBar(&app, nullptr, axRect(0, 0, 500, 530));

	FileDialog* file_dialog = new FileDialog(&app, nullptr, 
								  axRect(3, 30, 494, 497), path);


	
	
	// file_dialog->Reparent(titlebar, axPoint(0, 0));


	// app.GetCore();
	// XReparentWindow(dpy, (Window)win, parent, 3, 30);

	// Window win = static_cast<axCoreX11*>(app->GetCore())->GetWindow();
	// Display* dpy = static_cast<axCoreX11*>(app->GetCore())->GetDisplay();

	app.MainLoop();

	return 0;
}


FileDialog::FileDialog(axApp* app, axWindow* parent, 
					   const axRect& rect,
					   const string& folder_path):
					   axPanel(app, parent, rect)
{
	string path = app->GetCurrentAppDirectory();

	if(folder_path != "")
	{
		_dirNavigation = new DirectoryNavigation(folder_path);
	}
	else
	{
		_dirNavigation = new DirectoryNavigation("/home");
	}

	_folderContent = new FolderContent(app, this, 
									   axRect(axPoint(0, 31), rect.size - axSize(0, 31 + 30)),
									   _dirNavigation);

	axButtonInfo btn_info(string(path + "ressources/axDialogBtn.axobj"));

	function<void (axButtonMsg)> btnFct(GetOnBackBtn());

	axButton* b = new axButton(app, this, 
					  axRect(5, 5, 22, 22), 
					  axButtonEvents(btnFct), 
					  btn_info, "", "<",
					  axBUTTON_SINGLE_IMG | axBUTTON_IMG_RESIZE);

	b->SetBackgroundAlpha(0.5);

	btnFct = GetOnCancel();
	b = new axButton(app, this, 
					  axRect(rect.size.x - 130, rect.size.y - 26, 45, 20), 
					  axButtonEvents(btnFct), 
					  btn_info, "", "Cancel",
					  axBUTTON_SINGLE_IMG | axBUTTON_IMG_RESIZE);

	b->SetBackgroundAlpha(0.5);


	b = new axButton(app, this, 
					  axRect(rect.size.x - 60, rect.size.y - 26, 45, 20), 
					  axButtonEvents(), 
					  btn_info, "", "Select",
					  axBUTTON_SINGLE_IMG | axBUTTON_IMG_RESIZE);

	b->SetBackgroundAlpha(0.5);
}

DirectoryNavigation* FileDialog::GetDirectoryNavigation()
{
	return _dirNavigation;
}

void FileDialog::OnBackBtn(const axButtonMsg& msg)
{
	_dirNavigation->GoToParentDirectory();
	Update();
}

void FileDialog::OnCancel(const axButtonMsg& msg)
{
	cout << "" << endl;
	exit(0);
}


void FileDialog::OnPaint()
{
	axGC* gc = GetGC();

	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	// Background.
	gc->SetColor(axColor(0.9, 0.9, 0.9), 1.0);
	gc->DrawRectangle(rect0);

	// Icon bar.
	axRect iconBarRect(1, 1, rect0.size.x - 1, 31);
	gc->DrawRectangle(iconBarRect);
	gc->DrawRectangleColorFade(iconBarRect, 
							  axColor(0.6, 0.6, 0.6), 1.0, 
							  axColor(0.7, 0.7, 0.7), 1.0);


	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(iconBarRect);

	// Folder name.
	gc->SetColor(axColor(0.2, 0.2, 0.2), 1.0);
	gc->SetFontSize(14);
	gc->DrawString(_dirNavigation->GetCurrentDirectoryName(), axPoint(50, 7));

	axRect buttonBarRect(1, rect0.size.y - 30, rect0.size.x - 1, 30);
	gc->DrawRectangle(buttonBarRect);
	gc->DrawRectangleColorFade(buttonBarRect, 
							  axColor(0.6, 0.6, 0.6), 1.0, 
							  axColor(0.7, 0.7, 0.7), 1.0);

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(buttonBarRect);
}



TitleBar::TitleBar(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect)
{
	string path(app->GetCurrentAppDirectory());

	function<void (axButtonMsg)> btnFct(GetOnBtn());

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

void TitleBar::OnBtn(const axButtonMsg& msg)
{
	cout << "TESTTTTTT" << endl;
	exit(0);
}


void TitleBar::OnBtn2(const axButtonMsg& msg)
{
	exit(0);
}

void TitleBar::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->DrawRectangleColorFade(
		axRect(0, 0, rect0.size.x, rect0.size.y),// - 30),
		axColor(0.5, 0.5, 0.5), 1,
		axColor(0.3, 0.3, 0.3), 1);

	gc->SetColor(axColor(0.4, 0.4, 0.4));
	gc->DrawRectangleContour(axRect(1, 1, rect0.size.x - 1, rect0.size.y - 1));
}