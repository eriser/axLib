#include "main.h"

// //-----------------------------------------------------------------------------
// // CalculatorLogic
// //-----------------------------------------------------------------------------
// CalculatorLogic::CalculatorLogic() :
// 				_value_left(0),
// 				_result(0),
// 				_string_value("0"),
// 				_operator(CALC_EQUAL)
// {
// }

// void CalculatorLogic::SetOperation(const CalcOperator& operation)
// {
// 	switch (operation)
// 	{
// 	case CALC_RESET:
// 		_result = _value_left = 0; 
// 		_string_value = "0"; 
// 		_operator = CALC_EQUAL;
// 		break;
// 	case CALC_SIGN: 
// 		if (_string_value[0] == '-') 
// 			_string_value.erase(_string_value.begin());
// 		else _string_value.insert(0, "-"); break;
		
// 	case CALC_EQUAL: Equal(); break;
// 	default: TwoSideOperation(operation);
// 	}
// }

// void CalculatorLogic::TwoSideOperation(const CalcOperator& operation)
// {
// 	if (_operator != CALC_EQUAL) Equal();
// 	_value_left = stod(_string_value);
// 	_operator = operation;
// 	_string_value = "0";
// }

// void CalculatorLogic::Equal()
// {
// 	double v = stod(_string_value);

// 	switch (_operator)
// 	{
// 	case CALC_RESET: _result = 0; break;
// 	case CALC_ADD: _result = _value_left + v; break;
// 	case CALC_MIN: _result = _value_left - v; break;
// 	case CALC_MUL: _result = _value_left * v; break;
// 	case CALC_POW: _result = pow(_value_left, v); break;
// 	case CALC_MODULO: _result = int(floor(_value_left)) % int(floor(v)); break;
// 	case CALC_DIV: v == 0 ? _result = 0 : _result = _value_left / v; break;
// 	}

// 	_operator = CALC_EQUAL;
// 	_string_value = to_string(_result);
// }

// void CalculatorLogic::AddNumber(const string& number)
// {
// 	if (number == ".")
// 	{
// 		// If not found.
// 		if (_string_value.find('.') == std::string::npos)
// 			_string_value += number;
// 	}
// 	else
// 		_string_value += number;
// }

// double CalculatorLogic::GetValue()
// {
// 	return stod(_string_value);
// }

// //-----------------------------------------------------------------------------
// // Screen
// //-----------------------------------------------------------------------------
// CalcScreen::CalcScreen(axApp* app, axWindow* parent, const axRect& rect):
// axPanel(app, parent, rect), _value(0)
// {
// }

// void CalcScreen::SetValue(const double& value)
// {
// 	_value = value;
// 	Update();
// }

// void CalcScreen::OnPaint()
// {
// 	axGC* gc = GetGC();
// 	axRect rect(GetRect());

// 	gc->SetColor(axColor(0.5, 0.5, 0.5), 0.8);
// 	gc->DrawRectangle(axRect(axPoint(0, 0), rect.size));

// 	string val = to_string(_value);
// 	gc->SetColor(axColor("#FFFFFF"));
// 	gc->SetFontType("tmp/FreeSans.ttf");
// 	gc->SetFontSize(14);
// 	gc->DrawString(val, axPoint(5, 23));
// }

// //-----------------------------------------------------------------------------
// // Calculator
// //-----------------------------------------------------------------------------
// Calculator::Calculator(axApp* app, axWindow* parent, const axRect& rect):
// 			axPanel(app, parent, rect)
// {
// 	_screen = new CalcScreen(app, this, axRect(5, 5, 155, 40));

// 	std::function<void (axButtonMsg)> number_fct = GetOnNumber();
// 	std::function<void (axButtonMsg)> oper_fct = GetOnOperation();
// 	axButtonEvents number_event(number_fct);
// 	axButtonEvents operation_event(oper_fct);

// 	BtnInfo btns_info[20] =
// 	{
// 		BtnInfo(to_string(CALC_RESET), "tmp/C.png", operation_event),
// 		BtnInfo(to_string(CALC_SIGN), "tmp/signe.png", operation_event),
// 		BtnInfo(to_string(CALC_POW), "tmp/pow.png", operation_event),
// 		BtnInfo(to_string(CALC_MODULO), "tmp/modulo.png", operation_event),
// 		BtnInfo("7", "tmp/7.png", number_event),
// 		BtnInfo("8", "tmp/8.png", number_event),
// 		BtnInfo("9", "tmp/9.png", number_event),
// 		BtnInfo(to_string(CALC_ADD), "tmp/plus.png", operation_event),
// 		BtnInfo("4", "tmp/4.png", number_event),
// 		BtnInfo("5", "tmp/5.png", number_event),
// 		BtnInfo("6", "tmp/6.png", number_event),
// 		BtnInfo(to_string(CALC_MIN), "tmp/minus.png", operation_event),
// 		BtnInfo("1", "tmp/1.png", number_event),
// 		BtnInfo("2", "tmp/2.png", number_event),
// 		BtnInfo("3", "tmp/3.png", number_event),
// 		BtnInfo(to_string(CALC_MUL), "tmp/mul.png", operation_event),
// 		BtnInfo(".", "tmp/dot.png", number_event),
// 		BtnInfo("0", "tmp/0.png", number_event),
// 		BtnInfo(to_string(CALC_EQUAL), "tmp/equal.png", operation_event),
// 		BtnInfo(to_string(CALC_DIV), "tmp/div.png", operation_event)
// 	};

// 	createButtons(app, this, btns_info);
// }

// void Calculator::createButtons(axApp* app, axWindow* parent, BtnInfo info[20])
// {
// 	axButtonInfo btn_info("tmp/axStandardButton.axobj");

// 	axSize btn_size(35, 35);
// 	int delta = btn_size.x + 5;
// 	axPoint pos(5, 55);

// 	axRect btn_rect(pos, btn_size);
			
// 	for (int i = 0; i < 20; i++, i % 4 ? pos.x += delta : pos(5, pos.y+=delta))
// 	{
// 		axRect btn_rect(pos, btn_size);
// 		_btns.push_back(new axButton(app, this,	btn_rect, info[i].events, 
// 			btn_info, info[i].img, "", 0, info[i].msg));						 
// 	}
// }

// void Calculator::OnNumber(const axButtonMsg& msg)
// {
// 	_logic.AddNumber(msg.GetMsg());
// 	_screen->SetValue(_logic.GetValue());
// }

// void Calculator::OnOperation(const axButtonMsg& msg)
// {
// 	_logic.SetOperation((CalcOperator)stoi(msg.GetMsg()));
// 	_screen->SetValue(_logic.GetValue());
// }
// //---------------------------------------------------------------------------
// //---------------------------------------------------------------------------
// MyGrid::MyGrid(axApp* app, axWindow* parent, const axRect& rect):
// 			axPanel(app, parent, rect)
// {
// 	axGridInfo grid_info("tmp/axStandardGrid.axobj");
// 	axButtonInfo btn_info;

// 	int btn_x_pos = 50;
// 	int btn_y_pos = 40;
// 	axColor base = axColor(0.0, 0.8, 0.0);
// 	btn_info.normal = base;
// 	btn_info.hover = axColor(base.GetRed(), base.GetGreen() + 0.1,  base.GetBlue());
// 	btn_info.clicking = axColor(base.GetRed(), base.GetGreen() - 0.2,  base.GetBlue());
// 	btn_info.selected = base;
// 	btn_info.contour = axColor(0.0, 0.0, 0.0);

// 	std::function<void(axButtonMsg)> btn_fct = GetOnColorBtnGreen();
// 	axButton* btn = new axButton(app, this, axRect(btn_x_pos, btn_y_pos, 20, 20), 
// 								 axButtonEvents(btn_fct), btn_info);
	
// 	base = axColor(0.0, 0.8, 0.8);
// 	btn_info.normal = base;
// 	btn_info.hover = axColor(base.GetRed(), base.GetGreen(),  base.GetBlue() + 0.1);
// 	btn_info.clicking = axColor(base.GetRed(), base.GetGreen(),  base.GetBlue() - 0.2);
// 	btn_info.selected = base;

// 	btn_x_pos += 25;
// 	btn_fct = GetOnColorBtnBlue();
// 	axButton* btn2 = new axButton(app, this, axRect(btn_x_pos, btn_y_pos, 20, 20), 
// 								 axButtonEvents(btn_fct), btn_info);
	

// 	base = axColor(0.8, 0.0, 0.0);
// 	btn_info.normal = base;
// 	btn_info.hover = axColor(base.GetRed() + 0.1, base.GetGreen(),  base.GetBlue());
// 	btn_info.clicking = axColor(base.GetRed() - 0.2, base.GetGreen(),  base.GetBlue());
// 	btn_info.selected = base;

// 	btn_x_pos += 25;
// 	btn_fct = GetOnColorBtnRed();

// 	axButton* btn3 = new axButton(app, this, axRect(btn_x_pos, btn_y_pos, 20, 20), 
// 								 axButtonEvents(btn_fct), btn_info);

// 	_grid = new axGrid(app, this, axRect(55, 100, 540, 300), 
// 					   axGridEvents(), grid_info, axSize(16, 10));
// }

// void MyGrid::OnColorBtnRed(const axButtonMsg& msg)
// {
// 	_grid->SetElementColor(axColor(0.8, 0.0, 0.0));
// }

// void MyGrid::OnColorBtnBlue(const axButtonMsg& msg)
// {
// 	_grid->SetElementColor(axColor(0.0, 0.8, 0.8));
// }

// void MyGrid::OnColorBtnGreen(const axButtonMsg& msg)
// {
// 	_grid->SetElementColor(axColor(0.0, 0.8, 0.0));
// }

// void MyGrid::OnPaint()
// {
// 	axGC* gc = GetGC();
// 	axRect rect(GetRect());
// 	axRect rect0(axPoint(0, 0), rect.size);

// 	gc->SetColor(axColor(1.0, 1.0, 1.0), 1.0);
// 	gc->DrawRectangle(rect0);
// }

// TopMenuBar::TopMenuBar(axApp* app, axWindow* parent, const axRect& rect):
// 			axPanel(app, parent, rect)
// {

// }

// void TopMenuBar::OnPaint()
// {
// 	axGC* gc = GetGC();
// 	axRect rect(GetRect());
// 	axRect rect0(axPoint(0, 0), rect.size);

// 	gc->SetColor(axColor(0.3, 0.3, 0.3), 0.6);
// 	gc->DrawRectangle(rect0);
// }

// SideMenuBar::SideMenuBar(axApp* app, axWindow* parent, const axRect& rect):
// 			axPanel(app, parent, rect)
// {
// 	//_wallPaper = new axImage("ressources/osx_mav.png");

// 	int y = 50;
// 	axSize icon_size(30, 30);
// 	axButtonInfo btn_info("ressources/axObjects/axStandardButton.axobj");

// 	//_icons.push_back(new axImage("ressources/Sound-Mixer.png"));
// 	_icons.push_back(new axButton(app, this, 
// 								  axRect(axPoint(0, y), icon_size), 
// 								  axButtonEvents(), 
// 								  btn_info, 
// 								  "ressources/Sound-Mixer.png", // Image.
// 								  "", // Label.
// 								  axBUTTON_SINGLE_IMG));

// 	y += 50;

// 	_icons.push_back(new axButton(app, this, 
// 								  axRect(axPoint(0, y), icon_size), 
// 								  axButtonEvents(), 
// 								  btn_info, 
// 								  "ressources/desktop-folder-3.png", // Image.
// 								  "", // Label.
// 								  axBUTTON_SINGLE_IMG));

// 	y += 50;

// 	_icons.push_back(new axButton(app, this, 
// 								  axRect(axPoint(0, y), icon_size), 
// 								  axButtonEvents(), 
// 								  btn_info, 
// 								  "ressources/repeat_refresh_arrow_media-128.png", // Image.
// 								  "", // Label.
// 								  axBUTTON_SINGLE_IMG));

// 	y += 50;

// 	_icons.push_back(new axButton(app, this, 
// 								  axRect(axPoint(0, y), icon_size), 
// 								  axButtonEvents(), 
// 								  btn_info, 
// 								  "ressources/Piano-keys-1127205224.png", // Image.
// 								  "", // Label.
// 								  axBUTTON_SINGLE_IMG));

// 	_icons[0]->SetBackgroundAlpha(0.1);
// 	_icons[1]->SetBackgroundAlpha(0.1);
// 	_icons[2]->SetBackgroundAlpha(0.1);
// 	_icons[3]->SetBackgroundAlpha(0.1);
// }

// void SideMenuBar::OnPaint()
// {
// 	axGC* gc = GetGC();
// 	axRect rect(GetRect());
// 	axRect rect0(axPoint(0, 0), rect.size);

// 	gc->SetColor(axColor(0.6, 0.6, 0.6), 0.6);
// 	gc->DrawRectangle(rect0);
// }

Desktop::Desktop(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect)
{
	//_wallPaper = new axImage("ressources/dark_wallpaper.png");

//	_sideBar = new SideMenuBar(app, this, axRect(0, 30, 30, rect.size.y - 30));
//
//	_topBar = new TopMenuBar(app, this, axRect(0, 0, rect.size.x, 30));
}

void DrawAntiAliasingLine(axGC* gc, const axPoint& point1, const axPoint& point2, const int& width, axColor color)
{
	int x0 = point1.x, y0 = point1.y, x1 = point2.x, y1 = point2.y;
	double 	dx = x1 - x0,
			dy = y1 - y0;



	double D = sqrt(dx * dx + dy * dy);
	double cos_theta = dx / D;
	double sin_theta = dy / D;
	double wo2 = width * 0.5;

	int wx = abs(dx) + width, wy = abs(dy) + width;

	double rx = x0 + wo2 * cos_theta,
		   ry = y0 + wo2 * sin_theta;

	//gc->DrawLine(axPoint(100, 100), axPoint(200, 300));

	int n = 0, nData = 4;
	float* data = new float[wx * wy * nData];

	for(int j = 0; j < wy; j++)
	{
		for(int i = 0; i < wx; i++)
		{
			data[n] = 0.5;
			data[n+1] = 0.0;
			data[n+2] = 0.3;

			double px = i + rx, 
				   py = j + ry;


			// if(px < x0 && py < y0)
			// {
			// 	//cout << "py : " << py << " , y0 = " << y0 << endl;
			// 	double d = sqrt(pow(px - x0, 2) + pow(py - y0, 2));

			// 	cout << d << endl;

			// 	if(d < width * 0.5)
			// 	{
			// 		data[n+3] = 0.5 * (cos(d / (width * 0.5) * M_PI) + 1.0);
			// 		//data[n+3] = ((w * 0.5) - d)/(w * 0.5);
			// 		//cout << "TEST" << endl;
			// 	}
			// 	else
			// 	{
			// 		data[n+3] = 0.0;
			// 	}
			// }

			// else
			// {
				double d = abs(dy * px - dx * py - x0 * y1 + x1 * y0) / D;

				if(d < width * 0.5)
				{
					data[n+3] = 0.5 * (cos(d / (width * 0.5) * M_PI) + 1.0);
					//data[n+3] = ((w * 0.5) - d)/(w * 0.5);
				}
				else
				{
					data[n+3] = 0.0;
				}
			// }

			
			n += nData;
		}
	}

	GLuint texture;
	glBindTexture(GL_TEXTURE_2D, texture);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(
			GL_TEXTURE_2D, // Targer.
			0, // Level.
			GL_RGBA, // InternalFormat.
			wx, // Width.
			wy, // Height.
			0, // Border.
			GL_RGBA, // Format.
			GL_FLOAT, // Type.
			data // Data.
			);

	glEnable(GL_BLEND);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glDrawPixels(wx, wy, GL_RGBA, GL_FLOAT, data);


	gc->DrawTexture(texture, axRect(x0, y0, wx, wy));
}

void Desktop::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(1.0, 1.0, 1.0), 1.0);
	gc->DrawRectangle(rect0);

	axColor color(0.5, 0.0, 0.3);
	gc->SetColor(color, 1.0);
	
	double N = 100;
	for(int i = 1; i < int(N); i++)
	{
		double y0 = sin((i-1) / N * 2.0 * M_PI) * 200.0 + 250.0;
		double x0 = ((i-1) / N) * 500.0;

		double y1 = sin(i / N * 2.0 * M_PI) * 200.0 + 250.0;
		double x1 = ((i) / N) * 500.0;

		// double x = (i / 100.0) * 500.0;
		// double y = sin(i / 50.0 * 2.0 * M_PI) * 250.0 + 250.0;

		// cout << x << " " << y << endl;

		///DrawAntiAliasingLine(gc, axPoint(x0, y0), axPoint(x1, y1), 3, color);
		gc->DrawLine(axPoint(x0, y0), axPoint(x1, y1));

	}
	

	//DrawAntiAliasingLine(gc, axPoint(100, 100), axPoint(200, 200), 3, color);
	//DrawAntiAliasingLine(gc, axPoint(100, 100), axPoint(100, 200), 3, color);
	// gc->DrawLine(axPoint(100 + 10, 100), axPoint(200 + 10, 500));
	
//  	GLenum  	format,
//  	GLenum  	type,


}
// C Specification

// void glDrawPixels(	GLsizei  	width,
//  	GLsizei  	height,
//  	GLenum  	format,
//  	GLenum  	type,
//  	const GLvoid *  	data);
// Parameters

// width, height
// Specify the dimensions of the pixel rectangle to be written into the frame buffer.

// format
// Specifies the format of the pixel data. Symbolic constants GL_COLOR_INDEX, GL_STENCIL_INDEX, GL_DEPTH_COMPONENT, GL_RGB, GL_BGR, GL_RGBA, GL_BGRA, GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_LUMINANCE, and GL_LUMINANCE_ALPHA are accepted.

// type
// Specifies the data type for data. Symbolic constants GL_UNSIGNED_BYTE, GL_BYTE, GL_BITMAP, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, GL_FLOAT, GL_UNSIGNED_BYTE_3_3_2, GL_UNSIGNED_BYTE_2_3_3_REV, GL_UNSIGNED_SHORT_5_6_5, GL_UNSIGNED_SHORT_5_6_5_REV, GL_UNSIGNED_SHORT_4_4_4_4, GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_UNSIGNED_SHORT_5_5_5_1, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_UNSIGNED_INT_8_8_8_8, GL_UNSIGNED_INT_8_8_8_8_REV, GL_UNSIGNED_INT_10_10_10_2, and GL_UNSIGNED_INT_2_10_10_10_REV are accepted.

// data
// Specifies a pointer to the pixel data.

int main()
{
	//axApp app(axSize(165, 255));
	//Calculator* calc = new Calculator(&app, nullptr, axRect(0, 0, 250, 250));

	// axApp app(axSize(600, 500));
	// MyGrid* my_grid = new MyGrid(&app, nullptr, axRect(0, 0, 600, 500));

	axApp app(axSize(500, 500));
	Desktop* desktop = new Desktop(&app, nullptr, 
								   axRect(0, 0, 500, 500));
	
	app.MainLoop();

	return 0;
}
