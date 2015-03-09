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
#include "axObjectLoader.h"

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
axToggle::Info::Info() :
axInfo()
{
    
}

axToggle::Info::Info(const axColor& normal_color,
                     const axColor& hover_color,
                     const axColor& clicked_color,
                     const axColor& selected_color,
                     const axColor& selectedHover_color,
                     const axColor& selectedClicking_color,
                     const axColor& contour_color,
                     const axColor& font_color_,
                     const std::string& imgPath,
                     const bool& singleImg) :
// Heritage.
axInfo(),
// Members.
normal(normal_color),
hover(hover_color),
clicking(clicked_color),
selected(selected_color),
selected_hover(selectedHover_color),
selected_clicking(selectedClicking_color),
contour(contour_color),
font_color(font_color_),
img(imgPath),
single_img(singleImg)
{
    
}

axToggle::Info::Info(const std::string& path):
// Heritage.
axInfo(path)
{
    axWidgetLoader loader;
    axVectorPairString att = loader.GetAttributes(path);
    
    SetAttributes(att);
}

axToggle::Info::Info(const axVectorPairString& attributes):
axInfo()
{
    SetAttributes(attributes);
}

axStringVector axToggle::Info::GetParamNameList() const
{
    return axStringVector{"normal", "hover", "clicking",
                          "selected", "selected_hover", "selected_clicking",
                          "contour", "font_color", "img", "single_img"};
}

std::string axToggle::Info::GetAttributeValue(const std::string& name)
{
    if(name == "normal")
    {
        return normal.ToString();
    }
    else if(name == "hover")
    {
        return hover.ToString();
    }
    else if(name == "clicking")
    {
        return clicking.ToString();
    }
    else if(name == "selected")
    {
        return selected.ToString();
    }
    else if(name == "selected_hover")
    {
        return selected_hover.ToString();
    }
    else if(name == "selected_clicking")
    {
        return selected_clicking.ToString();
    }
    else if(name == "contour")
    {
        return contour.ToString();
    }
    else if(name == "font_color")
    {
        return font_color.ToString();
    }
    else if(name == "img")
    {
        return img;
    }
    else if(name == "single_img")
    {
        return std::to_string(single_img);
    }
    
    return "";
}

void axToggle::Info::SetAttribute(const axStringPair& attribute)
{
    if(attribute.first == "normal")
    {
        normal.LoadFromString(attribute.second);
    }
    else if(attribute.first == "hover")
    {
        hover.LoadFromString(attribute.second);
    }
    else if(attribute.first == "clicking")
    {
        clicking.LoadFromString(attribute.second);
    }
    else if(attribute.first == "selected")
    {
        selected.LoadFromString(attribute.second);
    }
    else if(attribute.first == "selected_hover")
    {
        selected_hover.LoadFromString(attribute.second);
    }
    else if(attribute.first == "selected_clicking")
    {
        selected_clicking.LoadFromString(attribute.second);
    }
    else if(attribute.first == "contour")
    {
        contour.LoadFromString(attribute.second);
    }
    else if(attribute.first == "font_color")
    {
        font_color.LoadFromString(attribute.second);
    }
    else if(attribute.first == "img")
    {
        img = attribute.second;
    }
    else if(attribute.first == "single_img")
    {
        single_img = stoi(attribute.second) == 1 ? true : false;
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
    std::string msg;
    
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
            _info = axToggle::Info(s.second);
        }
        else if(s.first == "label")
        {
            _label = s.second;
        }
        else if(s.first == "flags")
        {
            _flags = stoi(s.second);
        }
        else if(s.first == "msg")
        {
            msg = s.second;
        }
        else if(s.first == std::string("event"))
        {
            evts.button_click = _parent->GetEventFunction(s.second);
        }
    }
    
    axToggle* tog = new axToggle(_parent, axRect(pos, _size),
                                 evts,
                                 _info, _img, _label, _flags, msg);
    
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
                   std::string img_path,
                   std::string label,
                   axFlag flags,
                   std::string msg) :
// Heritage.
axWidget(parent, rect, new axToggle::Info(info)),
// Members.
_events(events),
_label(label),
_flags(flags),
_nCurrentImg(axTOG_NORMAL),
_selected(false),
test(1.0, 1.0, 0.0),
_msg(msg),
_font(nullptr)
{
    _currentColor = &static_cast<Info*>(_info)->normal;
//    _bgImg = new axImage(img_path);
    _bgImg = new axImage(static_cast<Info*>(_info)->img);
    
	_font = std::unique_ptr<axFont>(new axFont(0));
    _font->SetFontSize(static_cast<Info*>(_info)->font_size);
    
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
            _nCurrentImg = axTOG_SEL_NORMAL;
			if (_currentColor == &static_cast<Info*>(_info)->normal)
			{
				_currentColor = &static_cast<Info*>(_info)->selected;
				Update();
			}
		}
		else
		{
            _nCurrentImg = axTOG_NORMAL;
			if (_currentColor == &static_cast<Info*>(_info)->selected)
			{
				_currentColor = &static_cast<Info*>(_info)->normal;
				Update();
			}
		}
	}
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
            _currentColor = &static_cast<Info*>(_info)->selected_clicking;
            _nCurrentImg = axTOG_SEL_CLICK;
        }
        else
        {
            _currentColor = &static_cast<Info*>(_info)->clicking;
            _nCurrentImg = axTOG_CLICK;
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
                _currentColor = &static_cast<Info*>(_info)->selected_hover;
                _nCurrentImg = axTOG_SEL_HOVER;
            }
            else
            {
                _currentColor = &static_cast<Info*>(_info)->hover;
                _nCurrentImg = axTOG_HOVER;
            }
			
			
            
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
				_currentColor = &static_cast<Info*>(_info)->selected;
				_nCurrentImg = axTOG_SEL_NORMAL;
			}
			else
			{
				_currentColor = &static_cast<Info*>(_info)->normal;
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
            _currentColor = &static_cast<Info*>(_info)->selected_hover;
            _nCurrentImg = axTOG_SEL_HOVER;
        }
        else
        {
            _currentColor = &static_cast<Info*>(_info)->hover;
            _nCurrentImg = axTOG_HOVER;
        }
		
		
		Update();
	}
}

void axToggle::OnMouseLeave()
{
	if (!IsGrabbed())
	{
		if (_selected)
		{
			_currentColor = &static_cast<Info*>(_info)->selected;
			_nCurrentImg = axTOG_SEL_NORMAL;
		}
		else
		{
			_currentColor = &static_cast<Info*>(_info)->normal;
			_nCurrentImg = axTOG_NORMAL;
		}
	}

	Update();
}

void axToggle::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(GetDrawingRect());

	gc->SetColor(*_currentColor);
	gc->DrawRectangle(rect0);

	if (_bgImg->IsImageReady())
	{
        if (static_cast<Info*>(_info)->single_img)
		{
			gc->DrawImageResize(_bgImg, axPoint(0, 0), rect.size, 1.0);
		}
		else
		{
            axPoint pos(0, _nCurrentImg * _bgImg->GetSize().y / 6);
            axSize size(_bgImg->GetSize().x, _bgImg->GetSize().y / 6);
            gc->DrawPartOfImageResize(_bgImg, pos, size,
                                      axRect(axPoint(0, 0), GetRect().size));
		}
	}

	if_not_empty(_label)
	{
        if(_selected)
        {
            gc->SetColor(static_cast<Info*>(_info)->selected_font_color);
        }
        else
        {
            gc->SetColor(static_cast<Info*>(_info)->font_color);
        }
		
		gc->DrawStringAlignedCenter(*_font, _label, rect0);
	}

	gc->SetColor(static_cast<Info*>(_info)->contour);
	gc->DrawRectangleContour(rect0);
}


