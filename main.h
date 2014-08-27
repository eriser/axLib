#include "axLib.h"
//#include "axImage.h"

#include <vector>
#include <string>
#include <cmath>

#define axEvent virtual void

//------------------------
// Desktop app.
//------------------------
class Desktop: public axPanel
{
public:
	Desktop(axApp* app, axWindow* parent, const axRect& rect);

	axEVENT(axButtonMsg, OnOpenFile);
	axEVENT(axButtonMsg, OnOpenWindow);
	axEVENT(axButtonMsg, OnShowAppDirectory);
	axEVENT(int, OnClosePopupWindow);

private:
	axButton* openfile;
	axButton* openWindow;

	void OnClosePopupWindow(const int& msg);
	void OnOpenFile(const axButtonMsg& msg);
	void OnOpenWindow(const axButtonMsg& msg);
	void OnShowAppDirectory(const axButtonMsg& msg);

	virtual void OnPaint();
};

class MyTest : public axPanel
{
public:
	MyTest(axApp* app, axWindow* parent, const axRect& rect);
	void SetCloseCallback(axEvtFunction(int) fct)
	{
		_closeFct = fct;
	}

	axEVENT(axButtonMsg, OnClick);
private:
	axEvtFunction(int) _closeFct;

	void OnClick(const axButtonMsg& msg)
	{
		cout << "TEST" << endl;
		//PostQuitMessage(0);
		//GetApp()->GetCore()->KillPopGLWindow();
	
		if (_closeFct)
			_closeFct(0);

	}
	virtual void OnPaint();


	//virtual void OnMouseLeftDown(const axPoint& pos);
};