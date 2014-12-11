#include "main.h"
#include <unistd.h>

//-----------------------------------------------------------------------------
// CalculatorLogic
//-----------------------------------------------------------------------------
CalculatorLogic::CalculatorLogic() :
				_value_left(0),
				_result(0),
				_string_value("0"),
				_operator(CALC_EQUAL)
{
}

void CalculatorLogic::SetOperation(const CalcOperator& operation)
{
	switch (operation)
	{
	case CALC_RESET:
		_result = _value_left = 0; 
		_string_value = "0"; 
		_operator = CALC_EQUAL;
		break;
	case CALC_SIGN: 
		if (_string_value[0] == '-') 
			_string_value.erase(_string_value.begin());
		else _string_value.insert(0, "-"); break;
		
	case CALC_EQUAL: Equal(); break;
	default: TwoSideOperation(operation);
	}
}

void CalculatorLogic::TwoSideOperation(const CalcOperator& operation)
{
	if (_operator != CALC_EQUAL) Equal();
	_value_left = stod(_string_value);
	_operator = operation;
	_string_value = "0";
}

void CalculatorLogic::Equal()
{
	double v = stod(_string_value);

	switch (_operator)
	{
	case CALC_RESET: _result = 0; break;
	case CALC_ADD: _result = _value_left + v; break;
	case CALC_MIN: _result = _value_left - v; break;
	case CALC_MUL: _result = _value_left * v; break;
	case CALC_POW: _result = pow(_value_left, v); break;
	case CALC_MODULO: _result = int(floor(_value_left)) % int(floor(v)); break;
	case CALC_DIV: v == 0 ? _result = 0 : _result = _value_left / v; break;
	}

	_operator = CALC_EQUAL;
	_string_value = to_string(_result);
}

void CalculatorLogic::AddNumber(const string& number)
{
	if (number == ".")
	{
		// If not found.
		if (_string_value.find('.') == std::string::npos)
			_string_value += number;
	}
	else
		_string_value += number;
}

double CalculatorLogic::GetValue()
{
	return stod(_string_value);
}

//-----------------------------------------------------------------------------
// Screen
//-----------------------------------------------------------------------------
CalcScreen::CalcScreen(axWindow* parent, const axRect& rect):
axPanel(parent, rect), _value(0)
{
}

void CalcScreen::SetValue(const double& value)
{
	_value = value;
	Update();
}

void CalcScreen::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());

	gc->SetColor(axColor(0.5, 0.5, 0.5), 0.8);
	gc->DrawRectangle(axRect(axPoint(0, 0), rect.size));

//
//	char buf[1024];
//	readlink("/proc/self/exe", buf, sizeof(buf)-1);
//	string path(buf);
//	path = path.substr(0, path.find_last_of("/"));
//	path.push_back('/');
//
//	string font_path = path + string("tmp/FreeSans.ttf");
	// cout << "Paint path : " << font_path << endl;


	string val = to_string(_value);
	gc->SetColor(axColor(0.0, 0.0, 0.0));
//	gc->SetFontType(font_path);
	gc->SetFontSize(14);
	gc->DrawString(val, axPoint(5, 23));
}

//-----------------------------------------------------------------------------
// Calculator
//-----------------------------------------------------------------------------
Calculator::Calculator(axWindow* parent, const axRect& rect):
			axPanel(parent, rect)
{
	_screen = new CalcScreen(this, axRect(5, 5, 155, 40));

//	std::function<void (axButtonMsg)> number_fct = GetOnNumber();
//	std::function<void (axButtonMsg)> oper_fct = GetOnOperation();
    axButtonEvents number_event;
    number_event.button_click = GetOnNumber();
    axButtonEvents operation_event;
    operation_event.button_click = GetOnOperation();

//	char buf[1024];
//	readlink("/proc/self/exe", buf, sizeof(buf)-1);
//	string path(buf);
//	path = path.substr(0, path.find_last_of("/"));
//	path.push_back('/');

//	cout << path << endl;
    
    std::string path = axApp::GetInstance()->GetAppDirectory();

	BtnInfo btns_info[20] =
	{
		BtnInfo(to_string(CALC_RESET), path+string("C.png"), operation_event),
		BtnInfo(to_string(CALC_SIGN), path+string("signe.png"), operation_event),
		BtnInfo(to_string(CALC_POW), path+string("pow.png"), operation_event),
		BtnInfo(to_string(CALC_MODULO), path+string("modulo.png"), operation_event),
		BtnInfo("7", path+string("7.png"), number_event),
		BtnInfo("8", path+string("8.png"), number_event),
		BtnInfo("9", path+string("9.png"), number_event),
		BtnInfo(to_string(CALC_ADD), path + string("plus.png"), operation_event),
		BtnInfo("4", path+string("4.png"), number_event),
		BtnInfo("5", path+string("5.png"), number_event),
		BtnInfo("6", path+string("6.png"), number_event),
		BtnInfo(to_string(CALC_MIN), path + string("minus.png"), operation_event),
		BtnInfo("1", path+string("1.png"), number_event),
		BtnInfo("2", path+string("2.png"), number_event),
		BtnInfo("3", path+string("3.png"), number_event),
		BtnInfo(to_string(CALC_MUL), path + string("mul.png"), operation_event),
		BtnInfo(".", path+string("dot.png"), number_event),
		BtnInfo("0", path+string("0.png"), number_event),
		BtnInfo(to_string(CALC_EQUAL), path+string("equal.png"), operation_event),
		BtnInfo(to_string(CALC_DIV), path+string("div.png"), operation_event)
	};

	createButtons(this, btns_info);
}

void Calculator::createButtons(axWindow* parent, BtnInfo info[20])
{
//	char buf[1024];
//	readlink("/proc/self/exe", buf, sizeof(buf)-1);
//	string path(buf);
//	path = path.substr(0, path.find_last_of("/"));
//	path.push_back('/');

    axButtonInfo btn_info;//(path + string("tmp/axStandardButton.axobj"));
    btn_info.normal = axColor(0.6, 0.6, 0.6);
    btn_info.hover = axColor(0.8, 0.8, 0.8);
    btn_info.clicking = axColor(0.5, 0.5, 0.5);
    btn_info.contour = axColor(0.0, 0.0, 0.0);
    btn_info.selected = btn_info.normal;

	axSize btn_size(35, 35);
	int delta = btn_size.x + 5;
	axPoint pos(5, 55);

	axRect btn_rect(pos, btn_size);
			
	for (int i = 0; i < 20; i++, i % 4 ? pos.x += delta : pos(5, pos.y+=delta))
	{
		axRect btn_rect(pos, btn_size);
		_btns.push_back(new axButton(this,	btn_rect, info[i].events,
			btn_info, info[i].img, "", 0, info[i].msg));						 
	}
}

void Calculator::OnNumber(const axButtonMsg& msg)
{
	_logic.AddNumber(msg.GetMsg());
	_screen->SetValue(_logic.GetValue());
}

void Calculator::OnOperation(const axButtonMsg& msg)
{
	_logic.SetOperation((CalcOperator)stoi(msg.GetMsg()));
	_screen->SetValue(_logic.GetValue());
}

void Calculator::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    
    gc->SetColor(axColor(0.4, 0.4, 0.4));
    gc->DrawRectangle(axRect(axPoint(0, 0), rect.size));
}


void axMain::MainEntryPoint(axApp* app)
{
    Calculator* calc = new Calculator(nullptr, axRect(0, 0, 200, 255));
}
//---------------------------------------------------------------------------
//int main()
//{
//	axApp app(axSize(165, 255));
//	Calculator* calc = new Calculator(nullptr, axRect(0, 0, 250, 250));
//	
//	app.MainLoop();
//
//	return 0;
//}
