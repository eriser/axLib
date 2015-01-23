#include "axLib.h"
//#include "axImage.h"

#include <vector>
#include <string>
#include <cmath>

#define axEvent virtual void

//-----------------------------------------------------------------------------
// enum CalcOperator
// {
// 	CALC_ADD,
// 	CALC_MIN,
// 	CALC_MUL,
// 	CALC_DIV,
// 	CALC_POW,
// 	CALC_MODULO,
// 	CALC_SIGN,
// 	CALC_NUMBER,
// 	CALC_EQUAL,
// 	CALC_RESET
// };

// class CalculatorLogic
// {
// public:
// 	CalculatorLogic();
// 	void SetOperation(const CalcOperator& operation);
// 	double GetValue();
// 	void AddNumber(const string& number);
	
// private:
// 	string _string_value;
// 	double _value_left;
// 	double _result;
// 	CalcOperator _operator;

// 	void TwoSideOperation(const CalcOperator& operation);
// 	void Equal();
// };

// class CalcScreen : public axPanel
// {
// public:
// 	CalcScreen(axApp* app, axWindow* parent, const axRect& rect);

// 	void SetValue(const double& value);

// private:
// 	virtual void OnPaint();
// 	double _value;;
// };

// class Calculator: public axPanel
// {
// public:
// 	Calculator(axApp* app, axWindow* parent, const axRect& rect);

// 	axEVENT(axButtonMsg, OnNumber);
// 	axEVENT(axButtonMsg, OnOperation);

// private:
// 	vector<axButton*> _btns;
// 	CalcScreen* _screen;
// 	CalculatorLogic _logic;

// 	struct BtnInfo
// 	{
// 		BtnInfo(){}
// 		BtnInfo(const string& m, const string& i, 
// 				axButtonEvents& evt) :
// 				msg(m), img(i), events(evt){}

// 		string msg, img;
// 		axButtonEvents events;
// 	};
			 
// 	void createButtons(axApp* app, axWindow* parent, BtnInfo info[20]);

// 	// Events
// 	void OnOperation(const axButtonMsg& msg);
// 	void OnNumber(const axButtonMsg& msg);
// };
// //-----------------------------------------------------------------------------

// class MyGrid: public axPanel
// {
// public:
// 	MyGrid(axApp* app, axWindow* parent, const axRect& rect);

// 	axEVENT(axGridMsg, OnMoveElement);
// 	axEVENT(axButtonMsg, OnColorBtnRed);
// 	axEVENT(axButtonMsg, OnColorBtnBlue);
// 	axEVENT(axButtonMsg, OnColorBtnGreen);

// private:
// 	axGrid* _grid;

// 	// Events
// 	void OnMoveElement(const axGridMsg& msg);
// 	void OnColorBtnRed(const axButtonMsg& msg);
// 	void OnColorBtnBlue(const axButtonMsg& msg);
// 	void OnColorBtnGreen(const axButtonMsg& msg);
// 	//void OnNumber(const axButtonMsg& msg);
// 	virtual void OnPaint();
// };

// It would be nice if different EQ and COMP section in MixerTrack 
// could be load as a plugin.
class MixerTrackPlugin : public axPanel
{
public:
	MixerTrackPlugin(axApp* app, axWindow* parent, const axRect& rect);

	void Solo();
	void UnSolo();
	void Mute();
	void UnMute();
};

class MixerSend : public axPanel
{
public:
	MixerSend(axApp* app, axWindow* parent, const axRect& rect);
};

class MixerVolume : public axPanel
{
public:
	MixerVolume(axApp* app, axWindow* parent, const axRect& rect);

	void SetVolume(const double& value);
};

class MixerTrack : public axPanel
{
	MixerTrack(axApp* app, axWindow* parent, const axRect& rect);

	MixerTrackPlugin* _effects;
	MixerVolume* _volume;
	MixerSend* _sends;
};

class Mixer : public axPanel
{
public:
	Mixer(axApp* app, axWindow* parent, const axRect& rect);

	void AddTrack();
	void AddTrack(const int& position);

	deque<MixerTrack*> _tracks;
};
//------------------------------------------------------

class TopMenuBar: public axPanel
{
public:
	TopMenuBar(axApp* app, axWindow* parent, const axRect& rect);

private:
	//vector<axButton*> _icons;

	virtual void OnPaint();
};

class SideMenuBar: public axPanel
{
public:
	SideMenuBar(axApp* app, axWindow* parent, const axRect& rect);

private:
	vector<axButton*> _icons;

	virtual void OnPaint();
};

//------------------------
// Desktop app.
//------------------------
class Desktop: public axPanel
{
public:
	Desktop(axApp* app, axWindow* parent, const axRect& rect);

	axEVENT(axButtonMsg, OnBtn);
	axEVENT(axButtonMsg, OnBtn2);

private:
	// axImage* _wallPaper;
	// SideMenuBar* _sideBar;
	// TopMenuBar* _topBar;
	//vector<axButton*> _icons;

	void OnBtn(const axButtonMsg& msg);
	void OnBtn2(const axButtonMsg& msg);

	virtual void OnPaint();
};