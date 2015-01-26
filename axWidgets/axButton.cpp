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
#include "axObjectLoader.h"
#include "axDebugButton.h"
#include "axMath.h"

/*******************************************************************************
 * axButon::Flags.
 ******************************************************************************/
const axFlag axButton::Flags::SINGLE_IMG = axFLAG_1;
const axFlag axButton::Flags::IMG_RESIZE = axFLAG_2;
const axFlag axButton::Flags::CAN_SELECTED = axFLAG_3;

/*******************************************************************************
 * axButon::Msg.
 ******************************************************************************/
axButton::Msg::Msg()
{
    _sender = nullptr;
}

axButton::Msg::Msg(axButton* sender, const string& msg)
{
    _sender = sender;
    _msg = msg;
}

axButton* axButton::Msg::GetSender() const
{
    return _sender;
}

string axButton::Msg::GetMsg() const
{
    return _msg;
}

axMsg* axButton::Msg::GetCopy()
{
    return new axButton::Msg(*this);
}

/*******************************************************************************
 * axButon::Info.
 ******************************************************************************/
axButton::Info::Info() :
axInfo()
{
    
}

axButton::Info::Info(const axColor& normal_color,
                     const axColor& hover_color,
                     const axColor& clicked_color,
                     const axColor& selected_color,
                     const axColor& contour_color,
                     const axColor& font_color_) :
// Heritage.
axInfo(),
// Members.
normal(normal_color),
hover(hover_color),
clicking(clicked_color),
selected(selected_color),
contour(contour_color),
font_color(font_color_)
{
    
}

axButton::Info::Info(const std::string& path):
// Heritage.
axInfo(path)
{
    axWidgetLoader loader;
    axVectorPairString att = loader.GetAttributes(path);
    
    SetAttributes(att);
}

axButton::Info::Info(const axVectorPairString& attributes)
{
    SetAttributes(attributes);
}

axStringVector axButton::Info::GetParamNameList() const
{
    return axStringVector{"normal",
                          "hover",
                          "clicking",
                          "selected",
                          "contour",
                          "font_color"};
}

std::string axButton::Info::GetAttributeValue(const std::string& name)
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
    else if(name == "contour")
    {
        return contour.ToString();
    }
    else if(name == "font_color")
    {
        return font_color.ToString();
    }
    
    return "";
}

void axButton::Info::SetAttribute(const axStringPair& attribute)
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
    else if(attribute.first == "contour")
    {
        contour.LoadFromString(attribute.second);
    }
    else if(attribute.first == "font_color")
    {
        font_color.LoadFromString(attribute.second);
    }
}

/*******************************************************************************
 * axButton::Builder.
 ******************************************************************************/
axButton::Builder::Builder(axWindow* parent):
axWidgetBuilder(parent),
//_parent(win),
_past(nullptr),
_flags(0)
{
    
}

axWidget* axButton::Builder::Create(const axVectorPairString& attributes)
{
    std::string name;
    axPoint pos;
    axButton::Events evts;
    axWindow* parent = GetParent();
    
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
            _info = axButton::Info(s.second);
        }
        else if(s.first == "img")
        {
            _img = s.second;
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
            evts.button_click = parent->GetEventFunction(s.second);
        }
        else if(s.first == "msg")
        {
            _msg = s.second;
        }
    }
    
    axButton* btn = new axButton(parent, axRect(pos, _size), evts,
                                 _info, _img, _label, _flags, _msg);
    
    parent->GetResourceManager()->Add(name, btn);
    return btn;
}

/*******************************************************************************
 * axButon::axButton.
 ******************************************************************************/
axButton::axButton(axWindow* parent,
                   const axRect& rect,
                   const axButton::Events& events,
                   const axButton::Info& info,
                   std::string img_path,
                   std::string label,
                   axFlag flags,
                   std::string msg):
// Heritage.
axWidget(parent, rect, new axButton::Info(info)),
// Members.
_events(events),
_label(label),
_flags(flags),
_nCurrentImg(axBTN_NORMAL),
_selected(false),
_msg(msg),
_font(nullptr)
{
    _currentColor = &static_cast<axButton::Info*>(_info)->normal;
    
    _btnImg = new axImage(img_path);
    
    _font = new axFont(0);
    
    if(_events.button_click)
    {
        AddConnection(Events::BUTTON_CLICK, _events.button_click);
    }
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
        
        if (_selected)
        {
            if (_currentColor == &static_cast<axButton::Info*>(_info)->normal)
            {
                _currentColor = &static_cast<axButton::Info*>(_info)->selected;
                Update();
            }
        }
        else
        {
            if (_currentColor == &static_cast<axButton::Info*>(_info)->selected)
            {
                _currentColor = &static_cast<axButton::Info*>(_info)->normal;
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
    _currentColor = &static_cast<axButton::Info*>(_info)->clicking;
    _nCurrentImg = axBTN_DOWN;
    
    GrabMouse();
    
    PushEvent(Events::BUTTON_CLICK, new Msg(this, _msg));
    
    Update();
}

void axButton::OnMouseLeftUp(const axPoint& pos)
{
    if (IsGrabbed())
    {
        UnGrabMouse();
        
        if (IsMouseHoverWindow())
        {
            _currentColor = &static_cast<axButton::Info*>(_info)->hover;
            _nCurrentImg = axBTN_HOVER;
            _selected = true;
        }
        else
        {
            if (_selected)
            {
                _currentColor = &static_cast<axButton::Info*>(_info)->selected;
                _nCurrentImg = axBTN_SELECTED;
            }
            else
            {
                _currentColor = &static_cast<axButton::Info*>(_info)->normal;
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
        _currentColor = &static_cast<axButton::Info*>(_info)->hover;
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
            _currentColor = &static_cast<axButton::Info*>(_info)->selected;
            _nCurrentImg = axBTN_SELECTED;
        }
        else
        {
            _currentColor = &static_cast<axButton::Info*>(_info)->normal;
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
        rect0.size.x -= 1;
        rect0.size.y -= 1;
        
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
                gc->DrawPartOfImage(_btnImg, axPoint(0, _nCurrentImg * rect.size.y),
                                    rect0.size, axPoint(0, 0));
            }
        }
        
        if_not_empty(_label)
        {
            gc->SetColor(static_cast<axButton::Info*>(_info)->font_color, 1.0);
            gc->DrawStringAlignedCenter(*_font, _label, rect0);
        }
        
        gc->SetColor(static_cast<axButton::Info*>(_info)->contour);
        gc->DrawRectangleContour(rect0);
}


