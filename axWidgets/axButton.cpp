#include "axButton.h"

axButton::axButton(
	axApp* app,
	axWindow* parent,
	const axRect& rect,
	const axButtonEvents& events,
	const axButtonInfo& info,
	string img_path,
	string label,
	axFlag flags,
	string msg) :
	// Heritage.
	axPanel(app, parent, rect),
	// Members.
	_events(events),
	_info(info),
	_label(label),
	_flags(flags),
	_nCurrentImg(axBTN_NORMAL),
	_selected(false),
	test(1.0, 1.0, 0.0),
	_bgAlpha(1.0),
	_msg(msg)
{
	_currentColor = &_info.normal;

	//if_not_empty(img_path)
	//{
		_btnImg = new axImage(img_path);
	//}
	
}

void axButton::SetMsg(const string& msg)
{
	_msg = msg;
}

void axButton::SetSelected(const bool& selected)
{
	if (_selected != selected)
	{
		_selected = selected;

		if (_selected == true)
		{
			if (_currentColor == &_info.normal)
			{
				_currentColor = &_info.selected;
				Update();
			}
		}
		else
		{
			if (_currentColor == &_info.selected)
			{
				_currentColor = &_info.normal;
				Update();
			}
		}
	}
}

void axButton::SetBackgroundAlpha(const float& alpha)
{
	_bgAlpha = alpha;
	Update();
}

void axButton::OnMouseLeftDown(const axPoint& pos)
{
	_currentColor = &_info.clicking;

	GrabMouse();

	if (_events.button_click)
	{
		_events.button_click(axButtonMsg(this, _msg));
	}

	Update();
}

void axButton::OnMouseLeftUp(const axPoint& pos)
{
	if (IsGrabbed())
	{
		UnGrabMouse();
		
		if (IsMouseHoverWindow())
		{
			_currentColor = &_info.hover;
			_nCurrentImg = axBTN_HOVER;
			_selected = true;
		}
		else
		{
			if (_selected)
			{
				_currentColor = &_info.selected;
				_nCurrentImg = axBTN_SELECTED;
			}
			else
			{
				_currentColor = &_info.normal;
				_nCurrentImg = axBTN_NORMAL;
			}
		}

		Update();
	}
}

void axButton::OnMouseEnter()
{
	//DSTREAM << "ENTER" << endl;
	if (!IsGrabbed())
	{
		_currentColor = &_info.hover;
		_nCurrentImg = axBTN_HOVER;
		Update();
	}
}

void axButton::OnMouseLeave()
{
	if (!IsGrabbed())
	{
		if (_selected)
		{
			_currentColor = &_info.selected;
			_nCurrentImg = axBTN_SELECTED;
		}
		else
		{
			_currentColor = &_info.normal;
			_nCurrentImg = axBTN_NORMAL;
		}
	}

	Update();
}

void axButton::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(*_currentColor, _bgAlpha);
	gc->DrawRectangle(rect0);

	if (_btnImg->IsImageReady())
	{
		if (axFlag_exist(axBUTTON_SINGLE_IMG, _flags))
		{
			//gc->DrawImageResize(_btnImg, axPoint(0, _nCurrentImg * rect.size.y),
			//				    rect.size, axPoint(0, 0));

			gc->DrawImageResize(_btnImg, axPoint(0, 0), rect.size, 1.0);
		}
		else
		{
			gc->DrawPartOfImage(_btnImg, axPoint(0, _nCurrentImg * rect.size.y),
							    rect.size, axPoint(0, 0));
		}

	}


	//DSTREAM << "DRAWSTRING" << endl;
	//string t = "4";


	if_not_empty(_label)
	{
		// cout << "FONT" << endl;
		gc->SetColor(_info.font_color, 1.0);
		//gc->SetFontType("/home/alexarse/Desktop/axLib/ressources/axFonts/FreeSans.ttf");
		gc->SetFontSize(12);
		gc->DrawStringAlignedCenter(_label, rect0);
		// cout << "END FONT" << endl;
	}

	gc->SetColor(_info.contour, _bgAlpha);
	gc->SetColor(_info.contour, _bgAlpha);
	gc->DrawRectangleContour(axRect(axPoint(0, 0), rect.size));
}


