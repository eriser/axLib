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

axTextBox::axTextBox(axWindow* parent,
                             const axRect& rect,
                             const axTextBoxEvents& events,
                             const axTextBoxInfo& info,
                             string img_path,
                             string label,
                             axFlag flags) :
// Heritage.
axPanel(parent, rect),
// Members.
_events(events),
_info(info),
_label(label),
_flags(flags),
_nCurrentImg(axBTN_NORMAL),
_cursorBarXPosition(5),
_lastCharXPosition(5),
_flashingCursor(nullptr),
_cursorFlashActive(true),
_isHightlight(false),
_findClickCursorIndex(false)
{
	_currentColor = &_info.normal;

    _btnImg = new axImage(img_path);
    
    if(_events.button_click)
    {
        AddConnection(axTextBoxEvents::BUTTON_CLICK, _events.button_click);
    }
    
    if(axFlag_exist(axTEXT_BOX_FLASHING_CURSOR, _flags))
    {
        _flashingCursor = new axTimer();
        _flashingCursor->AddConnection(0, GetOnFlashingCursorTimer());
    }
    
    _cursorIndex = _label.size();
}

void axTextBox::SetLabel(const std::string& label)
{
    _label = label;
    Update();
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
    if(axFlag_exist(axTEXT_BOX_FLASHING_CURSOR, _flags))
    {
        _flashingCursor->StopTimer();
    }
    
    _currentColor = &_info.normal;
    
    Update();
}

void axTextBox::OnWasKeyGrabbed()
{
    _currentColor = &_info.selected;
    
    if(axFlag_exist(axTEXT_BOX_FLASHING_CURSOR, _flags))
    {
        _flashingCursor->StartTimer(500);
    }
    
    Update();
}

void axTextBox::OnKeyDown(const char& key)
{
    if(_isHightlight)
    {
        _label.resize(0);
        _label.insert(0, &key);
        _cursorIndex = 1;
        _isHightlight = false;
        Update();
    }
    else
    {
        if(_lastCharXPosition < GetRect().size.x - 10)
        {
            _label.insert(_cursorIndex, &key);
            ++_cursorIndex;
            Update();
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

void axTextBox::DrawContourRectangle(axGC* gc)
{
    axRect rect(GetRect());
    
    if(axFlag_exist(axTEXT_BOX_CONTOUR_HIGHLIGHT, _flags))
    {
        if(IsKeyGrab())
        {
            if(axFlag_exist(axTEXT_BOX_CONOUR_NO_FADE, _flags)) // Shadow fade.
            {
                gc->SetColor(_info.selected_shadow);
                gc->DrawRectangle(axRect(axPoint(-5, -5),
                                         axSize(rect.size + axSize(9, 9))));
            }
            else
            {
                axColor col(_info.selected_shadow);
                gc->SetColor(col);
                
                int nRect = 5;
                for(int i = 0; i < nRect; i++)
                {
                    gc->DrawRectangleContour(axRect(axPoint(-i, -i),
                                                    axSize(rect.size + axSize(2*i, 2*i))));
                    
                    double alpha = _info.selected_shadow.GetAlpha();
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
	axRect rect0(axPoint(0, 0), rect.size);
    
    DrawContourRectangle(gc);

	gc->SetColor(*_currentColor);
	gc->DrawRectangle(rect0);
    
    axPoint next_pos(5, 5);
    
    if_not_empty(_label)
    {
        _cursorBarXPosition = 5;
        
        for(int i = 0; i < _label.size(); i++)
        {
            int x_past_pos = next_pos.x;
            
            gc->SetColor(_info.font_color);
            next_pos = gc->DrawChar(_label[i], next_pos);
            
            if(_isHightlight) // hightlight on.
            {
                gc->SetColor(_info.hightlight);
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
        gc->SetColor(_info.cursor);
  
        gc->DrawLine(axPoint(_cursorBarXPosition, 5),
                     axPoint(_cursorBarXPosition, rect0.size.y - 5));
    }

	gc->SetColor(_info.contour);
	gc->DrawRectangleContour(axRect(axPoint(0, 0), rect.size));
}
