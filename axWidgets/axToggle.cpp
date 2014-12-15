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
#include "axToggle.h"

axToggle::axToggle(axWindow* parent,
                   const axRect& rect,
                   const axToggleEvents& events,
                   const axToggleInfo& info,
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
_nCurrentImg(axTOG_NORMAL),
_selected(false),
test(1.0, 1.0, 0.0),
//_bgAlpha(1.0),
_msg(msg)
{
	_currentColor = &_info.normal;
    _btnImg = new axImage(img_path);
    
    if(_events.button_click)
    {
        AddConnection(axToggleEvents::BUTTON_CLICK,
                      _events.button_click);
    }
}

//axToggle::axToggle(axWindow* parent,
//                   const axToggleEvents& events,
//                   const string& path):
//axPanel(parent, axRect(0, 0, 20, 20)),
//// Members.
//_events(events),
//_nCurrentImg(axTOG_NORMAL),
//_selected(false),
//_bgAlpha(1.0)
//{
//
//}

void axToggle::SetMsg(const string& msg)
{
	_msg = msg;
}

void axToggle::SetSelected(const bool& selected)
{
	if (_selected != selected)
	{
		_selected = selected;

		if (_selected == true)
		{
            _nCurrentImg = axTOG_SELECTED;
			if (_currentColor == &_info.normal)
			{
				_currentColor = &_info.selected;
				Update();
			}
		}
		else
		{
            _nCurrentImg = axTOG_NORMAL;
			if (_currentColor == &_info.selected)
			{
				_currentColor = &_info.normal;
				Update();
			}
		}
	}
}

void axToggle::SetBackgroundAlpha(const float& alpha)
{
	_bgAlpha = alpha;
	Update();
}

void axToggle::OnMouseLeftDown(const axPoint& pos)
{
    if(_selected && axFlag_exist(axTOGGLE_CANT_UNSELECT_WITH_MOUSE, _flags))
    {
        // Don't do anything.
    }
    else
    {
        // Only switch selection on toggle_on_left_down.
        if (axFlag_exist(axTOGGLE_CLICK_ON_LEFT_DOWN, _flags))
        {
            _selected = !_selected;
        }
        
        if(_selected)
        {
            _currentColor = &_info.selected_clicking;
        }
        else
        {
            _currentColor = &_info.clicking;
        }
        
        GrabMouse();
        
        if (axFlag_exist(axTOGGLE_CLICK_ON_LEFT_DOWN, _flags))
        {
            PushEvent(axToggleEvents::BUTTON_CLICK,
                      new axToggleMsg(this, _selected, _msg));
//            if (_events.button_click)
//            {
//                _events.button_click(axToggleMsg(this, _selected, _msg));
//            }
        }
        Update();
    }
}

void axToggle::OnMouseLeftUp(const axPoint& pos)
{
	if (IsGrabbed())
	{
		UnGrabMouse();
		
        // If mouse release if still on the toggle.
		if (IsMouseHoverWindow())
		{
            // Only invert selection on toggle_on_left_up.
            if (!axFlag_exist(axTOGGLE_CLICK_ON_LEFT_DOWN, _flags))
            {
                _selected = !_selected;
            }
            
            if (_selected)
            {
                _currentColor = &_info.selected_hover;
            }
            else
            {
                _currentColor = &_info.hover;
            }
			
			_nCurrentImg = axTOG_HOVER;
            
            // If toggle on left up.
            if (!axFlag_exist(axTOGGLE_CLICK_ON_LEFT_DOWN, _flags))
            {
//                std::cout << "axToggle::OnMouseLeftUp" << std::endl;
                PushEvent(axToggleEvents::BUTTON_CLICK,
                          new axToggleMsg(this, _selected, _msg));
//                if (_events.button_click)
//                {
//                    _events.button_click(axToggleMsg(this, _selected, _msg));
//                }
            }
		}
		else
		{
			if (_selected)
			{
				_currentColor = &_info.selected;
				_nCurrentImg = axTOG_SELECTED;
			}
			else
			{
				_currentColor = &_info.normal;
				_nCurrentImg = axTOG_NORMAL;
			}
		}

		Update();
	}
}

void axToggle::OnMouseEnter()
{
	if (!IsGrabbed())
	{
        if(_selected)
        {
            _currentColor = &_info.selected_hover;
        }
        else
        {
            _currentColor = &_info.hover;
        }
		
		_nCurrentImg = axTOG_HOVER;
        
        
		Update();
	}
}

void axToggle::OnMouseLeave()
{
	if (!IsGrabbed())
	{
		if (_selected)
		{
			_currentColor = &_info.selected;
			_nCurrentImg = axTOG_SELECTED;
		}
		else
		{
			_currentColor = &_info.normal;
			_nCurrentImg = axTOG_NORMAL;
		}
	}

	Update();
}

void axToggle::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(*_currentColor);
	gc->DrawRectangle(rect0);

	if (_btnImg->IsImageReady())
	{
		if (axFlag_exist(axTOGGLE_SINGLE_IMG, _flags))
		{
			gc->DrawImageResize(_btnImg, axPoint(0, 0), rect.size, 1.0);
		}
		else
		{
            gc->DrawPartOfImageResize(_btnImg,
                                      axPoint(0, _nCurrentImg * _btnImg->GetSize().y / 4),
                                      axSize(_btnImg->GetSize().x, _btnImg->GetSize().y / 4),
                                      axRect(axPoint(0, 0), GetRect().size));
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


