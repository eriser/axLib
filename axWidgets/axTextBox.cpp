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
#include "axTextBox.h"
#include "axObjectLoader.h"

/*******************************************************************************
 * axTextBox::Flags.
 ******************************************************************************/
const axFlag axTextBox::Flags::FLASHING_CURSOR = axFLAG_1;
const axFlag axTextBox::Flags::CONTOUR_HIGHLIGHT = axFLAG_2;
const axFlag axTextBox::Flags::CONTOUR_NO_FADE = axFLAG_3;

/*******************************************************************************
 * axTextBox::Msg.
 ******************************************************************************/
axTextBox::Msg::Msg()
{
    _sender = nullptr;
}

axTextBox::Msg::Msg(axTextBox* sender, const string& msg)
{
    _sender = sender;
    _msg = msg;
}

axTextBox* axTextBox::Msg::GetSender() const
{
    return _sender;
}

std::string axTextBox::Msg::GetMsg() const
{
    return _msg;
}

axMsg* axTextBox::Msg::GetCopy()
{
    return new axTextBox::Msg(*this);
}

/*******************************************************************************
 * axTextBox::Info.
 ******************************************************************************/
axTextBox::Info::Info() :
axInfo()
{
    
}

axTextBox::Info::Info(const axColor& normalColor,
                      const axColor& hoverColor,
                      const axColor& highlightColor,
                      const axColor& selectedColor,
                      const axColor& selected_shadowColor,
                      const axColor& cursorColor,
                      const axColor& contourColor,
                      const axColor& font_colorColor) :
// Heritage.
axInfo(),
// Members.
normal(normalColor),
hover(hoverColor),
highlight(highlightColor),
selected(selectedColor),
selected_shadow(selected_shadowColor),
cursor(cursorColor),
contour(contourColor),
font_color(font_colorColor)
{
    
}

axTextBox::Info::Info(const std::string& path):
// Heritage.
axInfo(path)
{
    axWidgetLoader loader;
    axVectorPairString att = loader.GetAttributes(path);
    
    SetAttributes(att);
}

axTextBox::Info::Info(const axVectorPairString& attributes)
{
    SetAttributes(attributes);
}

axStringVector axTextBox::Info::GetParamNameList() const
{
    return axStringVector
    {
        "normal",
        "hover",
        "highlight",
        "selected",
        "selected_shadow",
        "cursor",
        "contour",
        "font_color"
    };
}

std::string axTextBox::Info::GetAttributeValue(const std::string& name)
{
    if(name == "normal")
    {
        return normal.ToString();
    }
    else if(name == "hover")
    {
        return hover.ToString();
    }
    else if(name == "highlight")
    {
        return highlight.ToString();
    }
    else if(name == "selected")
    {
        return selected.ToString();
    }
    else if(name == "selected_shadow")
    {
        return selected_shadow.ToString();
    }
    else if(name == "cursor")
    {
        return cursor.ToString();
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

void axTextBox::Info::SetAttribute(const axStringPair& attribute)
{
    if(attribute.first == "normal")
    {
        normal.LoadFromString(attribute.second);
    }
    else if(attribute.first == "hover")
    {
        hover.LoadFromString(attribute.second);
    }
    else if(attribute.first == "highlight")
    {
        highlight.LoadFromString(attribute.second);
    }
    else if(attribute.first == "selected")
    {
        selected.LoadFromString(attribute.second);
    }
    else if(attribute.first == "selected_shadow")
    {
        selected_shadow.LoadFromString(attribute.second);
    }
    else if(attribute.first == "cursor")
    {
        cursor.LoadFromString(attribute.second);
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
 * axTextBox::Builder.
 ******************************************************************************/
axTextBox::Builder::Builder(axWindow* parent):
axWidgetBuilder(parent),
_flags(0)
{
    
}

axWidget* axTextBox::Builder::Create(const axVectorPairString& attributes)
{
    std::string name;
    axPoint pos;
    axTextBox::Events evts;
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
            
            pos = axPoint(stoi(strVec[0]), stoi(strVec[1]));
            _size = axSize(stoi(strVec[2]), stoi(strVec[3]));
        }
        else if(s.first == "info")
        {
            _info = axTextBox::Info(s.second);
        }
        else if(s.first == "img")
        {
            _imgPath = s.second;
        }
        else if(s.first == "label")
        {
            _label = s.second;
        }
        else if(s.first == "flags")
        {
            _flags = stoi(s.second);
        }
        else if(s.first == std::string("event_button_click"))
        {
            evts.button_click = parent->GetEventFunction(s.second);
        }
        else if(s.first == std::string("event_enter_click"))
        {
            evts.enter_click = parent->GetEventFunction(s.second);
        }
    }
    
    axTextBox* btn = new axTextBox(parent, axRect(pos, _size), evts,
                                   _info, _imgPath, _label, _flags);
    
    parent->GetResourceManager()->Add(name, btn);
    return btn;
}

/*******************************************************************************
 * axTextBox::Info.
 ******************************************************************************/
axTextBox::axTextBox(axWindow* parent,
                     const axRect& rect,
                     const axTextBox::Events& events,
                     const axTextBox::Info& info,
                     std::string img_path,
                     std::string label,
                     axFlag flags) :
// Heritage.
axWidget(parent, rect, new Info(info)),
// Members.
_events(events),
//_info(info),
_label(label),
_flags(flags),
_nCurrentImg(axBTN_NORMAL),
_cursorBarXPosition(5),
_lastCharXPosition(5),
_flashingCursor(nullptr),
_cursorFlashActive(true),
_isHightlight(false),
_findClickCursorIndex(false),
_font(nullptr),
_maxNumChar(10000000)
{
	_currentColor = &static_cast<Info*>(_info)->normal;

    _btnImg = new axImage(img_path);
    
    if(_events.button_click)
    {
        AddConnection(Events::BUTTON_CLICK, _events.button_click);
    }
    
    if(_events.enter_click)
    {
        AddConnection(Events::ENTER_CLICK, _events.enter_click);
    }
    
    if(IsFlag(Flags::FLASHING_CURSOR, _flags))
    {
        _flashingCursor = new axTimer();
        _flashingCursor->AddConnection(0, GetOnFlashingCursorTimer());
    }
    
    _cursorIndex = (int)_label.size();
    
    _font = new axFont(0);
    
    
    // 
    //SetShownRect(axRect(-5, -5, rect.size.x + 10, rect.size.y + 10));
}

void axTextBox::SetLabel(const std::string& label)
{
    _label = label;
    _lastCharXPosition = 0;
    _cursorIndex = 0;
    Update();
}

void axTextBox::SetMaximumChar(const unsigned int& max_number_of_char)
{
    _maxNumChar = max_number_of_char;
    if(_maxNumChar > 0 && _label.size() > _maxNumChar)
    {
        _label.resize(_maxNumChar);
        Update();
    }
}

std::string axTextBox::GetLabel() const
{
    return _label;
}

void axTextBox::OnMouseLeftDown(const axPoint& pos)
{
    _findClickCursorIndex = true;
    _clickPosition = pos - GetAbsoluteRect().position;
    
    if(_isHightlight)
    {
        _isHightlight = false;
    }
    
    GrabMouse();
    GrabKey();
    Update();
}

void axTextBox::OnFlashingCursorTimer(const axTimerMsg& msg)
{
    _cursorFlashActive = !_cursorFlashActive;
    Update();
}

void axTextBox::OnMouseLeftUp(const axPoint& pos)
{
    if(IsGrabbed())
    {
        UnGrabMouse();
    }
}

void axTextBox::OnMouseEnter()
{

}

void axTextBox::OnMouseLeave()
{

}

void axTextBox::OnMouseLeftDragging(const axPoint& pos)
{
    _clickPosition = pos - GetAbsoluteRect().position;
    Update();
    
}

void axTextBox::OnMouseLeftDoubleClick(const axPoint& pos)
{
    _isHightlight = true;
    Update();
}

void axTextBox::OnWasKeyUnGrabbed()
{
    if(IsFlag(Flags::FLASHING_CURSOR, _flags))
    {
        _flashingCursor->StopTimer();
    }
    
    _currentColor = &static_cast<Info*>(_info)->normal;
    
    Update();
}

void axTextBox::OnWasKeyGrabbed()
{
    _currentColor = &static_cast<Info*>(_info)->selected;
    
    if(IsFlag(Flags::FLASHING_CURSOR, _flags))
    {
        _flashingCursor->StartTimer(500);
    }
    
    Update();
}

void axTextBox::OnKeyDown(const char& key)
{
    // If all text is highlighted.
    if(_isHightlight)
    {
        _label.resize(0);
        _label.insert(0, &key);
        _cursorIndex = 1;
        _isHightlight = false;
        Update();
    }
    // Insert char in _label.
    else
    {
        // Can't add another char label size if bigger than _maxNumChar.
        if(_label.size() != _maxNumChar)
        {
            if(_lastCharXPosition < GetRect().size.x - 10)
            {
                _label.insert(_cursorIndex, &key);
                ++_cursorIndex;
                Update();
            }
        }
    }
}

void axTextBox::OnBackSpaceDown()
{
    if(_isHightlight)
    {
        _label.resize(0);
        _cursorIndex = 0;
        _isHightlight = false;
        Update();
    }
    else if(_label.size() && _cursorIndex)
    {
        _label.erase(_cursorIndex-1, 1);
        --_cursorIndex;
        
        if(_cursorIndex < 0)
        {
            _cursorIndex = 0;
        }
        Update();
    }
    
}

void axTextBox::OnKeyDeleteDown()
{
    if(_isHightlight)
    {
        _label.resize(0);
        _cursorIndex = 0;
        _isHightlight = false;
        Update();
    }
    else if(_label.size() && _cursorIndex < _label.size())
    {
        _label.erase(_cursorIndex, 1);
        --_cursorIndex;
        
        if(_cursorIndex < 0)
        {
            _cursorIndex = 0;
        }
        Update();
    }
}

void axTextBox::OnLeftArrowDown()
{
    --_cursorIndex;
    
    if(_cursorIndex < 0)
    {
        _cursorIndex = 0;
    }
    
    if(_isHightlight)
    {
        _isHightlight = false;
    }

    
    Update();
}

void axTextBox::OnRightArrowDown()
{
    ++_cursorIndex;
    
    if(_cursorIndex > _label.size())
    {
        _cursorIndex = _label.size();
    }
    
    if(_isHightlight)
    {
        _isHightlight = false;
    }

    
    Update();
}

void axTextBox::OnEnterDown()
{
    PushEvent(Events::ENTER_CLICK, new axTextBox::Msg(this, _label));
//    UnGrabKey();
}

void axTextBox::DrawContourRectangle(axGC* gc)
{
    if(IsFlag(Flags::CONTOUR_HIGHLIGHT, _flags))
    {
        if(IsKeyGrab())
        {
            axRect rect(GetRect());
            
            if(IsFlag(Flags::CONTOUR_NO_FADE, _flags)) // Shadow fade.
            {
                gc->SetColor(static_cast<Info*>(_info)->selected_shadow);
                gc->DrawRectangle(axRect(axPoint(-5, -5),
                                         axSize(rect.size + axSize(9, 9))));
            }
            else
            {
                axColor col(static_cast<Info*>(_info)->selected_shadow);
                gc->SetColor(col);
                
                int nRect = 5;
                for(int i = 0; i < nRect; i++)
                {
                    gc->DrawRectangleContour(axRect(axPoint(-i, -i),
                                                    axSize(rect.size + axSize(2*i, 2*i))));
                    
                    double alpha = static_cast<Info*>(_info)->selected_shadow.GetAlpha();
                    double mu = double(i) / double(nRect);
                    
                    col.SetAlpha(alpha - alpha * mu);
                    gc->SetColor(col);
                }
            }
        }
    }
}

void axTextBox::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(GetDrawingRect());
    
//    DrawContourRectangle(gc);

	gc->SetColor(*_currentColor);
	gc->DrawRectangle(rect0);
    
    axPoint next_pos(5, 5);
    
    if_not_empty(_label)
    {
        _cursorBarXPosition = 5;
        
        gc->SetColor(static_cast<Info*>(_info)->font_color);
        
        // Start drawing label.
        for(int i = 0; i < _label.size(); i++)
        {
            int x_past_pos = next_pos.x;
            next_pos = gc->DrawChar(*_font, _label[i], next_pos);
            
            if(_isHightlight) // hightlight on.
            {
                gc->SetColor(static_cast<Info*>(_info)->highlight);
                gc->DrawRectangle(axRect(x_past_pos, 5,
                                         next_pos.x - x_past_pos, rect0.size.y - 10));
            }
            
            if(_findClickCursorIndex)
            {
                if(_clickPosition.x >= x_past_pos &&
                   _clickPosition.x < next_pos.x)
                {
                    _cursorIndex = i;
                    _cursorBarXPosition = x_past_pos;
                }
                else if(i == _label.size() - 1 && _clickPosition.x > next_pos.x)
                {
                    _cursorIndex = i + 1;
                    _cursorBarXPosition = next_pos.x;
                }
            }
            else if(_cursorIndex - 1 == i)
            {
                _cursorBarXPosition = next_pos.x;
            }
        }
        
        if(_findClickCursorIndex)
        {
            _findClickCursorIndex = false;
        }
        
        _lastCharXPosition = next_pos.x;
    }
    else
    {
        _cursorBarXPosition = 5;
    }

    if(IsKeyGrab() && _cursorFlashActive)
    {
        gc->SetColor(static_cast<Info*>(_info)->cursor);
  
        gc->DrawLine(axPoint(_cursorBarXPosition, 5),
                     axPoint(_cursorBarXPosition, rect0.size.y - 5));
    }

	gc->SetColor(static_cast<Info*>(_info)->contour);
	gc->DrawRectangleContour(rect0);
}
