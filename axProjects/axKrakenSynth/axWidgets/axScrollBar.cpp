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
#include "axScrollBar.h"

axScrollBar::axScrollBar(axWindow* parent,
                         axWindow* handle,
                         const axRect& rect,
                         const axScrollBarEvents& events,
                         const axScrollBarInfo& info,
                         axFlag flags) :
// Heritage.
axPanel(parent, rect),
// Members.
_events(events),
_info(info),
_handle(handle),
_flags(flags),
_yClickDelta(0),
_value(0.0)
{

    axButton::Info btn_info(axColor(0.8, 0.0, 0.0),
                            axColor(0.9, 0.0, 0.0),
                            axColor(0.8, 0.0, 0.0),
                            axColor(0.8, 0.0, 0.0),
                            axColor(0.0, 0.0, 0.0),
                            axColor(0.0, 0.0, 0.0));

	//function<void (axButtonMsg)> btnFct();

//	_btn[0] = new axButton(this, 
//						   axRect(0, 0, 14, 14), 
//						   axButtonEvents(), 
//						   btn_info, 
//						   "/Users/alexarse/Project/axLib/axProjects/MidiSequencer/scrollBar_up.png");
//
//	_btn[1] = new axButton(this, 
//						   axRect(0, rect.size.y - 14, 14, 14), 
//						   axButtonEvents(), 
//						   btn_info, 
//						   "/Users/alexarse/Project/axLib/axProjects/MidiSequencer/scrollBar_down.png");

	_imgHeight = 0;
    _panelSize = rect.size;
    _sliderMaxHeight = rect.size.y - (2.0 * _imgHeight);
    
    double ratio = double(_panelSize.y - rect.size.y) / double(_panelSize.y);
    
    _sliderHeight = _sliderMaxHeight * (1.0 - ratio);
    _sliderPos = 0;
    
    _currentScrollBarColor = &_info.normal;
    
    if(_events.value_change)
    {
        AddConnection(axScrollBarEvents::VALUE_CHANGE, _events.value_change);
    }

}

void axScrollBar::SetPanelSize(const axSize& size)
{
    axSize framSize(GetRect().size);
    _panelSize = size;
    _sliderMaxHeight = framSize.y - (2.0 * _imgHeight);
    
    double ratio = double(_panelSize.y - framSize.y) / double(_panelSize.y);

    _sliderHeight = _sliderMaxHeight * (1.0 - ratio);
    Update();
}

void axScrollBar::OnMouseMotion(const axPoint& position)
{
    axPoint pos = position - GetAbsoluteRect().position;
    
    if(axRect(0, _sliderPos, GetRect().size.x, _sliderHeight).IsPointInside(pos))
    {
        _currentScrollBarColor = &_info.hover;
        Update();
    }
    else if(_currentScrollBarColor != &_info.normal)
    {
       _currentScrollBarColor = &_info.normal;
        Update();
    }
}

void axScrollBar::OnMouseLeftDown(const axPoint& position)
{
	axPoint pos = position - GetAbsoluteRect().position;
    
    if(axRect(0, _sliderPos, GetRect().size.x, _sliderHeight).IsPointInside(pos))
    {
        _currentScrollBarColor = &_info.hover;
        _yClickDelta = pos.y - _sliderPos;

        GrabMouse();
        Update();
    }
}

void axScrollBar::OnMouseLeftUp(const axPoint& pos)
{
	if(IsGrabbed())
	{
		UnGrabMouse();
	}
}

void axScrollBar::OnMouseLeftDragging(const axPoint& position)
{
	axPoint pos = position - GetAbsoluteRect().position;

	if(_sliderHeight < _sliderMaxHeight)
	{
		// m_sliderPosition move with mouse position.
		_sliderPos = pos.y - _yClickDelta;

		// If m_sliderPosition reach bottom, clip m_sliderPosition.
		if(_sliderPos + _sliderHeight > GetRect().size.y - _imgHeight - 1)
		{
			_sliderPos = GetRect().size.y - _imgHeight - 1 - _sliderHeight;
		}

		// Clip top.
		if(_sliderPos < _imgHeight)
        {
            _sliderPos = _imgHeight;
        }
			
		// Slider position ratio.
		_value = (_sliderPos - _imgHeight) / double(_sliderMaxHeight - _sliderHeight - 1);
        _value = axClamp<double>(_value, 0.0, 1.0);

        
        PushEvent(axScrollBarEvents::VALUE_CHANGE,
                  new axScrollBarMsg(this, std::string("")));
        
        _handle->SetScrollDecay(axPoint(0, _value * (_panelSize.y - GetRect().size.y)));

		Update();
	}
}

void axScrollBar::OnMouseEnter()
{
}

void axScrollBar::OnMouseLeave()
{
    if(_currentScrollBarColor != &_info.normal)
    {
        _currentScrollBarColor = &_info.normal;
        Update();
    }
}

void axScrollBar::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect0(axPoint(0, 0), GetRect().size);
//
	gc->SetColor(axColor(1.0, 0.0, 0.0), 1.0);
	gc->DrawRectangle(rect0);
    
    gc->DrawRectangleColorFade(rect0,
                               axColor(0.6, 0.6, 0.6),
                               axColor(0.4, 0.4, 0.4));
//
//	// gc->DrawImageResize(_bgImg, rect0.position, rect0.size);
//
//	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
//	gc->DrawRectangleContour(axRect(axPoint(1, 1), rect0.size - axSize(1, 1) ));
//

    gc->SetColor(*_currentScrollBarColor);
	axRect bar_rect(0, _sliderPos, GetRect().size.x, _sliderHeight);
	gc->DrawRectangle(bar_rect);
    
    gc->SetColor(_info.contour);
    gc->DrawRectangleContour(bar_rect);
//
//	gc->SetColor(axColor(0.3, 0.3, 0.3), 1.0);
//	gc->DrawRectangleContour(bar_rect);
//
	gc->SetColor(_info.contour);
	gc->DrawRectangleContour(rect0);
}
