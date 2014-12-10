#include "axButton.h"

axButton::axButton(axWindow* parent,
	const axRect& rect,
	const axButtonEvents& events,
	const axButtonInfo& info,
	string img_path,
	string label,
	axFlag flags,
	string msg) :
	// Heritage.
	axPanel(parent, rect),
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
    
    if(_events.button_click)
    {
        AddConnection(axButtonEvents::BUTTON_CLICK, _events.button_click);
    }
	
}

axButton::axButton(axWindow* parent,
		 const axButtonEvents& events,
		 const string& path):
		 axPanel(parent, axRect(0, 0, 20, 20)),
		// Members.
		_events(events),
		_nCurrentImg(axBTN_NORMAL),
		_selected(false),
		_bgAlpha(1.0)
{
	ifstream file;
	file.open(path);

	if (file.fail())
	{
		cerr << "Problem opening file " << path << endl;
	}
	else
	{
		if (!file.fail())
		{
			axRect rect;
			string rect_info;
			file >> rect.position.x >> rect.position.y >> rect.size.x >> rect.size.y;

			SetPosition(rect.position);
			SetSize(rect.size);

			string info_path;
			file >> info_path;
			_info = axButtonInfo(info_path);

			string img_path;
			file >> img_path;
			_btnImg = new axImage(img_path);

			file >> _label;

			int flag0 = 0;
			int flag1 = 0;
			file >> flag0;
			file >> flag1;
			_flags = (unsigned int)flag0 | (unsigned int)flag1;
			_msg = "";
			
		}
	}

	_currentColor = &_info.normal;

	file.close();
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

//void axButton::SetBackgroundAlpha(const float& alpha)
//{
//	_bgAlpha = alpha;
//	Update();
//}

void axButton::SetLabel(const std::string& label)
{
    _label = label;
    Update();
}

void axButton::OnMouseLeftDown(const axPoint& pos)
{
	_currentColor = &_info.clicking;
    _nCurrentImg = axBTN_DOWN;

	GrabMouse();

    PushEvent(axButtonEvents::BUTTON_CLICK, new axButtonMsg(this, _msg));

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
                std::cout << "Btn selected." << std::endl;
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

	gc->SetColor(*_currentColor);
	gc->DrawRectangle(rect0);

	if (_btnImg->IsImageReady())
	{
		if (axFlag_exist(axBUTTON_SINGLE_IMG, _flags))
		{
			gc->DrawImageResize(_btnImg, axPoint(0, 0), rect.size, 1.0);
		}
		else
		{
			gc->DrawPartOfImage(_btnImg, axPoint(0, _nCurrentImg * rect.size.y),
							    rect.size, axPoint(0, 0));
		}

	}

	if_not_empty(_label)
	{
		gc->SetColor(_info.font_color, 1.0);
		gc->SetFontSize(12);
		gc->DrawStringAlignedCenter(_label, rect0);
	}

	gc->SetColor(_info.contour);
	gc->DrawRectangleContour(axRect(axPoint(0, 0), rect.size));
}


