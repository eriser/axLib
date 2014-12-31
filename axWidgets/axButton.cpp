/*******************************************************************************
 * Copyright (c) 2013 Alexandre Arsenault.
 *
 * This file is part of axLibrary.
 *
 * axLibrary is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axLibrary License.
 *
 * axLibrary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axLibrary. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axLibrary, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 ******************************************************************************/
#include "axButton.h"


/*******************************************************************************
 * axButonMsg.
 ******************************************************************************/
axButtonMsg::axButtonMsg()
{
    _sender = nullptr;
}

axButtonMsg::axButtonMsg(axButton* sender, const string& msg)
{
    _sender = sender;
    _msg = msg;
}

axButton* axButtonMsg::GetSender() const
{
    return _sender;
}

string axButtonMsg::GetMsg() const
{
    return _msg;
}

axMsg* axButtonMsg::GetCopy()
{
    return new axButtonMsg(*this);
}

/*******************************************************************************
 * axButonInfo.
 ******************************************************************************/
axButtonInfo::axButtonInfo()
{
    
}

axButtonInfo::axButtonInfo(const axColor& normal_color,
                           const axColor& hover_color,
                           const axColor& clicked_color,
                           const axColor& selected_color,
                           const axColor& contour_color,
                           const axColor& font_color_) :
normal(normal_color),
hover(hover_color),
clicking(clicked_color),
selected(selected_color),
contour(contour_color),
font_color(font_color_)
{
    
}

axButtonInfo::axButtonInfo(const std::string& path)
{
    SerializeInput(path);
}

void axButtonInfo::SerializeOutput(const std::string& path)
{
    fstream file;
    file.open(path, std::fstream::out | std::fstream::binary);
    
    if (file.fail())
    {
        std::cerr << "Problem opening file " << path << std::endl;
    }
    else
    {
        normal.SerializeOutput(file);
        hover.SerializeOutput(file);
        clicking.SerializeOutput(file);
        selected.SerializeOutput(file);
        contour.SerializeOutput(file);
        font_color.SerializeOutput(file);
    }
    
}

void axButtonInfo::SerializeInput(const std::string& path)
{
    fstream file;
    file.open(path, std::fstream::in | std::fstream::binary);
    
    if (file.fail())
    {
        std::cerr << "Problem opening file " << path << std::endl;
    }
    else
    {
        normal.SerializeInput(file);
        hover.SerializeInput(file);
        clicking.SerializeInput(file);
        selected.SerializeInput(file);
        contour.SerializeInput(file);
        font_color.SerializeInput(file);
    }
}

/*******************************************************************************
 * axButon.
 ******************************************************************************/
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
_msg(msg)
{
	_currentColor = &_info.normal;

    _btnImg = new axImage(img_path);
    
    if(_events.button_click)
    {
        AddConnection(axButtonEvents::BUTTON_CLICK, _events.button_click);
    }
	
}

axButton::axButton(axWindow* parent,
                   const axButtonEvents& events,
                   const string& path):
// Heritage.
axPanel(parent, axRect(0, 0, 20, 20)),
// Members.
_events(events),
_nCurrentImg(axBTN_NORMAL),
_selected(false)
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
		if (IsFlag(axBUTTON_SINGLE_IMG, _flags))
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


