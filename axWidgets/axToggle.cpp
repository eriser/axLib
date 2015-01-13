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

/*******************************************************************************
 * axToggle::Flags.
 ******************************************************************************/
const axFlag axToggle::Flags::SINGLE_IMG = axFLAG_1;
const axFlag axToggle::Flags::IMG_RESIZE = axFLAG_2;
const axFlag axToggle::Flags::CLICK_ON_LEFT_DOWN = axFLAG_3;
const axFlag axToggle::Flags::CANT_UNSELECT_WITH_MOUSE = axFLAG_4;

/*******************************************************************************
 * axToggle::Msg.
 ******************************************************************************/
axToggle::Msg::Msg()
{
    _sender = nullptr;
}

axToggle::Msg::Msg(axToggle* sender, const bool& selected)
{
    _sender = sender;
    _selected = selected;
    _msg = std::string("");
}

axToggle::Msg::Msg(axToggle* sender,
                   const bool& selected,
                   const std::string& msg)
{
    _sender = sender;
    _selected = selected;
    _msg = msg;
}

axToggle* axToggle::Msg::GetSender() const
{
    return _sender;
}

bool axToggle::Msg::GetSelected() const
{
    return _selected;
}

string axToggle::Msg::GetMsg() const
{
    return _msg;
}

axMsg* axToggle::Msg::GetCopy()
{
    return new Msg(*this);
}

/*******************************************************************************
 * axToggle::Info.
 ******************************************************************************/
axToggle::Info::Info()
{
    
}

axToggle::Info::Info(const axColor& normal_color,
                     const axColor& hover_color,
                     const axColor& clicked_color,
                     const axColor& selected_color,
                     const axColor& selectedHover_color,
                     const axColor& selectedClicking_color,
                     const axColor& contour_color,
                     const axColor& font_color_) :
normal(normal_color),
hover(hover_color),
clicking(clicked_color),
selected(selected_color),
selected_hover(selectedHover_color),
selected_clicking(selectedClicking_color),
contour(contour_color),
font_color(font_color_)
{
    
}

void axToggle::Info::SerializeOutput(const std::string& path)
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
        selected_hover.SerializeOutput(file);
        selected_clicking.SerializeOutput(file);
        
        contour.SerializeOutput(file);
        font_color.SerializeOutput(file);
    }
}

void axToggle::Info::SerializeInput(const std::string& path)
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
        selected_hover.SerializeInput(file);
        selected_clicking.SerializeInput(file);
        
        contour.SerializeInput(file);
        font_color.SerializeInput(file);
    }
}

/*******************************************************************************
 * axToggle::Builder.
 ******************************************************************************/
axToggle::Builder::Builder(axPanel* parent,
                           const axSize& size,
                           const axToggle::Info& info,
                           string img_path,
                           string label,
                           axFlag flags,
                           int nextPositionDelta,
                           axDirection direction):
_parent(parent),
_size(size),
_info(info),
_img(img_path),
_label(label),
_flags(flags),
_direction(direction),
_nextPositionDelta(nextPositionDelta),
_past(nullptr)
{
    
}

axToggle::Builder::Builder(axWindow* win):
_parent(win),
_past(nullptr)
{
    
}

axToggle* axToggle::Builder::Create(axVectorPairString attributes)
{
    std::string name;
    axPoint pos;
    axToggle::Events evts;
    for(auto& s : attributes)
    {
        if(s.first == "name")
        {
            name = s.second;
        }
        else if(s.first == "rect")
        {
            axStringVector strVec;
            strVec = GetVectorFromStringDelimiter(s.second, ",");
            
            pos = axPoint(stoi(strVec[0]),
                          stoi(strVec[1]));
            
            _size = axSize(stoi(strVec[2]),
                           stoi(strVec[3]));
        }
        else if(s.first == "info")
        {
            _info.SerializeInput(s.second);
        }
        else if(s.first == "label")
        {
            _label = s.second;
        }
        else if(s.first == "flags")
        {
            _flags = stoi(s.second);
        }
        else if(s.first == std::string("event"))
        {
            evts.button_click = _parent->GetEventFunction(s.second);
        }
    }
    
    axToggle* tog = new axToggle(_parent, axRect(pos, _size),
                                 evts,
                                 _info, _img, _label);
    
    _parent->GetResourceManager()->Add(name, tog);
    return tog;
    
}

/*******************************************************************************
 * axToggle::axToggle.
 ******************************************************************************/
axToggle::axToggle(axWindow* parent,
                   const axRect& rect,
                   const axToggle::Events& events,
                   const axToggle::Info& info,
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
_msg(msg)
{
	_currentColor = &_info.normal;
    _btnImg = new axImage(img_path);
    
    if(_events.button_click)
    {
        AddConnection(Events::BUTTON_CLICK, _events.button_click);
    }
}

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
    if(_selected && IsFlag(Flags::CANT_UNSELECT_WITH_MOUSE, _flags))
    {
        // Don't do anything.
    }
    else
    {
        // Only switch selection on toggle_on_left_down.
        if (IsFlag(Flags::CLICK_ON_LEFT_DOWN, _flags))
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
        
        if (IsFlag(Flags::CLICK_ON_LEFT_DOWN, _flags))
        {
            PushEvent(Events::BUTTON_CLICK, new Msg(this, _selected, _msg));
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
            if (!IsFlag(Flags::CLICK_ON_LEFT_DOWN, _flags))
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
            if (!IsFlag(Flags::CLICK_ON_LEFT_DOWN, _flags))
            {
                PushEvent(Events::BUTTON_CLICK, new Msg(this, _selected, _msg));
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
        if (IsFlag(Flags::SINGLE_IMG, _flags))
		{
			gc->DrawImageResize(_btnImg, axPoint(0, 0), rect.size, 1.0);
		}
		else
		{
            axPoint pos(0, _nCurrentImg * _btnImg->GetSize().y / 4);
            axSize size(_btnImg->GetSize().x, _btnImg->GetSize().y / 4);
            gc->DrawPartOfImageResize(_btnImg, pos, size,
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


