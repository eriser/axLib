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
#include "axDebugButton.h"

axDebugButton::axDebugButton(axWindow* parent) :
// Heritage.
axPanel(parent, axRect(axPoint(parent->GetSize().x - 7,
                               parent->GetSize().y - 7),
                       axSize(7, 7))),
// Members.
//_events(events),
_info(axColor(0.9, 0.0, 0.0, 0.5),
      axColor(1.0, 0.0, 0.0, 0.5),
      axColor(0.85, 0.0, 0.0, 0.5),
      axColor(0.9, 0.0, 0.0, 0.5),
      axColor(0.0, 0.0, 0.0, 0.5),
      axColor(0.0, 0.0, 0.0, 0.5)),
_label(""),
_flags(0),
_nCurrentImg(axBTN_NORMAL),
_selected(false),
_msg(""),
//_font(0)
_font(nullptr)
{
    _currentColor = &_info.normal;
    
    _btnImg = new axImage("");
    
    _font = new axFont(0);
    
    if(_events.button_click)
    {
//        AddConnection(axButton::Events::BUTTON_CLICK, _events.button_click);
    }
}

void axDebugButton::SetMsg(const string& msg)
{
    _msg = msg;
}

void axDebugButton::SetSelected(const bool& selected)
{
    if (_selected != selected)
    {
        _selected = selected;
        
        if (_selected)
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

void axDebugButton::SetLabel(const std::string& label)
{
    _label = label;
    Update();
}

void axDebugButton::OnMouseLeftDown(const axPoint& pos)
{
    _currentColor = &_info.clicking;
    _nCurrentImg = axBTN_DOWN;

    GrabMouse();
    
    PushEvent(axButton::Events::BUTTON_CLICK,
              new axButton::Msg(nullptr, _msg));
    
    Update();
}

void axDebugButton::OnMouseLeftDragging(const axPoint &pos)
{
    axWindow* panel = GetParent()->GetParent();
    axRect panelRect = panel->GetAbsoluteRect();
    
    axPoint position(pos);
    position = position - panelRect.position;
    position.x = position.x - GetParent()->GetSize().x + 5;
    position.y = position.y - GetParent()->GetSize().y + 5;


    position.x = axClamp<int>(position.x, 0,
                              panelRect.size.x - GetParent()->GetSize().x);
    
    position.y = axClamp<int>(position.y, 0,
                              panelRect.size.y - GetParent()->GetSize().y);
    
    
    GetParent()->SetPosition(position);
}

void axDebugButton::OnMouseLeftUp(const axPoint& pos)
{
    if (IsGrabbed())
    {
//        ShowMouse();
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

void axDebugButton::OnMouseEnter()
{
    if (!IsGrabbed())
    {
        _currentColor = &_info.hover;
        _nCurrentImg = axBTN_HOVER;
        Update();
    }
}

void axDebugButton::OnMouseLeave()
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

void axDebugButton::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(*_currentColor);
    gc->DrawRectangle(rect0);
    
    if (_btnImg->IsImageReady())
    {
        if (IsFlag(axButton::Flags::SINGLE_IMG, _flags))
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
        gc->DrawStringAlignedCenter(*_font, _label, rect0);
    }
    
    gc->SetColor(_info.contour);
    gc->DrawRectangleContour(axRect(axPoint(0, 0), rect.size));
}



