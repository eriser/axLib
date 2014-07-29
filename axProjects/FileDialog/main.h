#include "axLib.h"
//#include "axImage.h"

#include <vector>
#include <string>
#include <cmath>
#include <dirent.h> 
#include <algorithm>

//------------------------
// Desktop app.
//------------------------
class Desktop: public axPanel
{
public:
	Desktop(axApp* app, axWindow* parent, const axRect& rect, const string& path);

	axEVENT(axButtonMsg, OnBtn);
	axEVENT(axButtonMsg, OnBtn2);
	axEVENT(axButtonMsg, OnBtn3);

	enum IconType
	{
		ICON_FOLDER,
		ICON_PROGFILE,
		ICON_EXEC,
		ICON_PNG,
		ICON_NONE
	};

	typedef pair<string, IconType> FileInfo;
private:
	vector<axButton*> _btns;
	deque<FileInfo> _dirName;
	deque<IconType> _iconType;

	axImage* folder, *back;
	axImage* file_img, *png_img;

	// int _filesHeight;

	int _selected_file;
	void FillDirectoryVector(const string& dir);


	void OnBtn(const axButtonMsg& msg);
	void OnBtn2(const axButtonMsg& msg);
	void OnBtn3(const axButtonMsg& msg);

	virtual void OnMouseLeftDown(const axPoint& mousePos);
	virtual void OnMouseMotion(const axPoint& mousePos);
	virtual void OnPaint();
};