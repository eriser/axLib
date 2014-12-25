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
#include "axKnob.h"

/********************************************************************************//**
* axKnob.
***********************************************************************************/
axKnob::axKnob( axWindow* parent,
               const axRect& rect,
               const axKnobEvents& events,
               const axKnobInfo& info,
               axFlag flags,
               double value):
// Heritage.
axPanel(parent, rect),
// Members.
_events(events),
_info(info),
m_currentBgColor(_info.bgColorNormal),
//m_nCurrentImg(0),
m_knobValue(0.0),
_zeroToOneValue(0.0),
_range(0.0, 1.0)
{
    m_knobImg = new axImage(_info.img_path);
	_bgAlpha = 1.0;
    
    m_nCurrentImg = m_knobValue * (_info.n_knob - 1);
    
    if(_events.value_change)
    {
        AddConnection(0, _events.value_change);
    }
}

void  axKnob::OnMouseLeftDown(const axPoint& pos)
{
    _clickPosY = (pos - GetAbsoluteRect().position).y;

    GrabMouse();
    Update();
    
    HideMouse();

//    if(_events.value_change)
//    {
//        _events.value_change(axKnobMsg(m_knobValue));
//    }
    PushEvent(0, new axKnobMsg(m_knobValue));

}

void axKnob::OnMouseLeftUp(const axPoint& pos)
{
    if( IsGrabbed() )
    {
        ShowMouse();
        UnGrabMouse();
        Update();

        PushEvent(0, new axKnobMsg(m_knobValue));
//        if(_events.value_change)
//        {
//            _events.value_change(axKnobMsg(m_knobValue));
//        }
    }
}

void  axKnob::OnMouseLeftDragging(const axPoint& position)
{
    int cur_img = m_nCurrentImg;
    axPoint pt(GetAbsoluteRect().position);
    axPoint p = position - pt;
    
    double delta = p.y - _clickPosY;
    
    _clickPosY = p.y;
    
    double v = -delta / 100.0;
    _zeroToOneValue += v;
    
    _zeroToOneValue = axClamp<double>(_zeroToOneValue, 0.0, 1.0);
    m_knobValue = _range.GetValueFromZeroToOne(_zeroToOneValue);

    m_nCurrentImg = m_knobValue * ( _info.n_knob - 1 ) ;
    
    if( m_nCurrentImg != cur_img )
    {
        Update();
    }

//    if(_events.value_change)
//    {
//        _events.value_change(axKnobMsg(m_knobValue));
//    }
    PushEvent(0, new axKnobMsg(m_knobValue));
}

void axKnob::SetValue(const axFloat& value, bool callValueChangeEvent)
{
	int cur_img = m_nCurrentImg;
	_zeroToOneValue = axClamp<double>(value, 0.0, 1.0);
    m_knobValue = _zeroToOneValue;
	m_nCurrentImg = m_knobValue * (_info.n_knob - 1);

	if (m_nCurrentImg != cur_img)
	{
		Update();
	}

//	if (_events.value_change)
//	{
//		_events.value_change(axKnobMsg(m_knobValue));
//	}
    
    if(callValueChangeEvent)
    {
        PushEvent(0, new axKnobMsg(m_knobValue));
    }
}

void axKnob::OnPaint()
{
    axGC* gc = GetGC();
    axSize size = GetSize();
    axRect rect0(0, 0, size.x, size.y);
	
	gc->SetColor(m_currentBgColor);

    gc->DrawRectangle(rect0);

    gc->DrawPartOfImage(m_knobImg,
                        axPoint( m_nCurrentImg * _info.knob_size.x, 0),
                        _info.knob_size,
                        axPoint(0, 0));
}

axKnobControl::axKnobControl(axWindow* parent,
                             const axRect& rect,
                             const axKnobEvents& events,
                             const axKnobInfo& info,
                             const std::string& label,
                             axFlag flags,
                             double value):
// Heritage.
axPanel(parent, rect),
// Members.
_label(label),
_value("0.00")
{
    //---------------------------------------------------
    axKnobEvents evts;
    evts.value_change = GetOnKnobValueChange();
    
    axPoint knobPos((rect.size.x - info.knob_size.x) * 0.5,
                    20 + (rect.size.y - 40 - info.knob_size.y) * 0.5);

    _knob = new axKnob(this, axRect(knobPos, info.knob_size),
                       evts, info, flags, value);
    
    if(events.value_change)
    {
        _knob->AddConnection(0, events.value_change);
    }
}

void axKnobControl::SetValue(const double& value)
{
    _knob->SetValue(value);
}

void axKnobControl::OnKnobValueChange(const axKnobMsg& msg)
{
    std::string v = to_string(msg.GetValue());
    v.resize(4);
    _value = v;
    Update();
}

void axKnobControl::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect0(axPoint(0, 0), GetRect().size);
    
    gc->SetColor(axColor(0.5, 0.5, 0.5, 0.3));
    gc->DrawRectangle(rect0);
    
    axRect labelRect(0, 0, rect0.size.x - 1, 20);
    gc->SetColor(axColor(0.6, 0.6, 0.6, 0.3));
    gc->DrawRectangle(labelRect);
    
    gc->SetFontSize(12);
    gc->SetColor(axColor(0.0, 0.0, 0.0));
    gc->DrawStringAlignedCenter(_label, labelRect);
    
    gc->SetFontSize(10);
    gc->DrawStringAlignedCenter(_value, axRect(0, rect0.size.y - 20,
                                               rect0.size.x, 20));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0, 0.3));
    gc->DrawRectangleContour(rect0);

}
