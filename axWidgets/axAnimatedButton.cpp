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
#include "axAnimatedButton.h"

axAnimatedButton::axAnimatedButton(axWindow* parent,
                                   const axRect& rect,
                                   const axButtonEvents& events,
                                   string img_path,
                                   string label):
axButton(parent, rect, events, axBUTTON_TRANSPARENT, img_path, label)
{
    _fadeValue = 1.0;
    _nPastCurrentImg = _nCurrentImg;
    
    _fadeTimer = new axTimer();
    _fadeTimer->AddConnection(0, GetOnFadeTimer());
    _fadeTimeMs = 400.0;
}

void axAnimatedButton::OnFadeTimer(const axTimerMsg& msg)
{
    _fadeValueMutex.lock();
    _fadeValue += (20.0 / _fadeTimeMs);
    _fadeValue = axClamp<double>(_fadeValue, 0.0, 1.0);
    _fadeValueMutex.unlock();
    Update();
}

void axAnimatedButton::SetupTimer()
{
    _fadeTimer->StopTimer();
    
    _fadeValueMutex.lock();
    _fadeValue = 1.0 - _fadeValue;
    _fadeValueMutex.unlock();
    
    _fadeTimer->StartTimer(20, _fadeTimeMs);
}

void axAnimatedButton::OnMouseLeftDown(const axPoint& pos)
{
    _nPastCurrentImg = _nCurrentImg;
    axButton::OnMouseLeftDown(pos);
}

void axAnimatedButton::OnMouseLeftUp(const axPoint& pos)
{
    _nPastCurrentImg = _nCurrentImg;
    axButton::OnMouseLeftUp(pos);
    
    SetupTimer();
}

void axAnimatedButton::OnMouseEnter()
{
    _nPastCurrentImg = _nCurrentImg;
    axButton::OnMouseEnter();
    
    SetupTimer();
}

void axAnimatedButton::OnMouseLeave()
{
    if(!IsGrabbed())
    {
        _nPastCurrentImg = _nCurrentImg;
        
        if(_nCurrentImg != axButton::axBTN_NORMAL)
        {
            SetupTimer();
        }
    }
    
    axButton::OnMouseLeave();
}

void axAnimatedButton::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(*_currentColor);
    gc->DrawRectangle(rect0);
    
    // Draw image.
    if (_btnImg->IsImageReady())
    {
        _fadeValueMutex.lock();
        
        gc->DrawPartOfImage(_btnImg, axPoint(0, _nPastCurrentImg * rect.size.y),
                            rect.size, axPoint(0, 0), (1.0 - _fadeValue));
        
        gc->DrawPartOfImage(_btnImg, axPoint(0, _nCurrentImg * rect.size.y),
                            rect.size, axPoint(0, 0), _fadeValue);
        _fadeValueMutex.unlock();
        
    }
    
    // Draw Font.
    if_not_empty(_label)
    {
        gc->SetColor(_info.font_color);
        gc->SetFontSize(12);
        gc->DrawStringAlignedCenter(_label, rect0);
    }
    
    gc->SetColor(_info.contour);
    gc->DrawRectangleContour(axRect(axPoint(0, 0), rect.size));
}


