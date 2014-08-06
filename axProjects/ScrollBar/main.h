#include "axLib.h"
//#include "axImage.h"

#include <vector>
#include <string>
#include <cmath>

#define axEvent virtual void

class ScrollWindow: public axPanel
{
public:
	ScrollWindow(axApp* app, axWindow* parent, const axRect& rect);

private:
	virtual void OnPaint();
};

class MyWindow: public axPanel
{
public:
	MyWindow(axApp* app, axWindow* parent, const axRect& rect);

	axEVENT(axScrollBarMsg, OnScroll);
private:
	axScrollBar* scroll_bar;
	ScrollWindow* scroll_win;

	void OnScroll(const axScrollBarMsg& msg);

	virtual void OnPaint();
};

//------------------------
// Desktop app.
//------------------------
class Desktop: public axPanel
{
public:
	Desktop(axApp* app, axWindow* parent, const axRect& rect);

private:

	MyWindow* mywin;

	virtual void OnPaint();
};