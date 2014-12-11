#include "axLib.h"
//#include "axImage.h"

#include <vector>
#include <string>
#include <cmath>

#define axEvent virtual void

enum CalcOperator
{
	CALC_ADD,
	CALC_MIN,
	CALC_MUL,
	CALC_DIV,
	CALC_POW,
	CALC_MODULO,
	CALC_SIGN,
	CALC_NUMBER,
	CALC_EQUAL,
	CALC_RESET
};

class CalculatorLogic
{
public:
	CalculatorLogic();
	void SetOperation(const CalcOperator& operation);
	double GetValue();
	void AddNumber(const string& number);
	
private:
	string _string_value;
	double _value_left;
	double _result;
	CalcOperator _operator;

	void TwoSideOperation(const CalcOperator& operation);
	void Equal();
};

class CalcScreen : public axPanel
{
public:
	CalcScreen(axWindow* parent, const axRect& rect);

	void SetValue(const double& value);

private:
	virtual void OnPaint();
	double _value;;
};

class Calculator: public axPanel
{
public:
	Calculator(axWindow* parent, const axRect& rect);

	axEVENT_ACCESSOR(axButtonMsg, OnNumber);
	axEVENT_ACCESSOR(axButtonMsg, OnOperation);

private:
	vector<axButton*> _btns;
	CalcScreen* _screen;
	CalculatorLogic _logic;

	struct BtnInfo
	{
		BtnInfo(){}
		BtnInfo(const string& m, const string& i, 
				axButtonEvents& evt) :
				msg(m), img(i), events(evt){}

		string msg, img;
		axButtonEvents events;
	};
			 
	void createButtons(axWindow* parent, BtnInfo info[20]);

	// Events
	void OnOperation(const axButtonMsg& msg);
	void OnNumber(const axButtonMsg& msg);
    void OnPaint();
};
//-----------------------------------------------------------------------------
