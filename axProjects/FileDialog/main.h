#include "axLib.h"
//#include "axImage.h"

#include <vector>
#include <string>
#include <cmath>
#include <dirent.h> 
#include <algorithm>
#include <map>

class DirectoryNavigation
{
public:
	DirectoryNavigation(const string& folder_path);

	enum IconType
	{
		ICON_FOLDER,
		ICON_PROGFILE,
		ICON_EXEC,
		ICON_PNG,
		ICON_MIDI,
		ICON_NONE
	};

	typedef pair<string, IconType> FileInfo;

	void SetDestinationPath(const string& path);

	string GetParentDirectory() const;

	string GetCurrentDirectoryName() const;

	IconType FindType(dirent* dir);

	deque<FileInfo>* GetFileInfoDeque();

	void PrintDirectoryContent();

	void GoToParentDirectory();

	void GoToFolder(const string& folder_name);


private:
	deque<DirectoryNavigation::FileInfo> _dirName;

	string _currentDirectory;
};


//------------------------
// Desktop app.
//------------------------
class FolderContent: public axPanel
{
public:
	FolderContent(axApp* app, axWindow* parent, const axRect& rect, 
				 DirectoryNavigation* navigation);

	void SetDirectoryNavigation(DirectoryNavigation* navigation);

	// axEVENT(axButtonMsg, OnBtn);

private:
	vector<axButton*> _btns;
	axImage *file_img, 
			*png_img, 
			*png_midi, 
			*folder;

	typedef pair<DirectoryNavigation::IconType, axImage*> IconPair;
	map<DirectoryNavigation::IconType, axImage*> _icons;

	DirectoryNavigation* _dirNavigation;

	int _selected_file;

	virtual void OnMouseLeftDown(const axPoint& mousePos);
	virtual void OnMouseMotion(const axPoint& mousePos);
	virtual void OnPaint();
};


class FileDialog: public axPanel
{
public:
	FileDialog(axApp* app, axWindow* parent, const axRect& rect, const string& path);

	DirectoryNavigation* GetDirectoryNavigation();

	axEVENT(axButtonMsg, OnBackBtn);
	axEVENT(axButtonMsg, OnCancel);

private:
	vector<axButton*> _btns;
	DirectoryNavigation* _dirNavigation;
	FolderContent* _folderContent;

	void OnCancel(const axButtonMsg& msg);
	void OnBackBtn(const axButtonMsg& msg);
	virtual void OnPaint();
};