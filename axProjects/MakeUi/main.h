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

	axEVENT(axButtonMsg, OnBtn);
	axEVENT(axButtonMsg, OnBtn2);
	axEVENT(axButtonMsg, OnBtn3);

private:
	vector<axButton*> _btns;


	void OnBtn(const axButtonMsg& msg);
	void OnBtn2(const axButtonMsg& msg);
	void OnBtn3(const axButtonMsg& msg);

	virtual void OnPaint();
};