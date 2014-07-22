#include "axNumberBox.h"

axNumberBox::axNumberBox(axApp* app,
						 axWindow* parent,
						 const axRect& rect,
						 const axNumberBoxEvents& events,
						 const axNumberBoxInfo& info,
						 axFlag flags,
                         double value,
                         axFloatRange range,
                         axControlType type,
                         axControlUnit unit,
                         axControlInterpolation interpolation,
                         string label):

                        axPanel(app, parent, rect),
						// Members.
						_events(events),
						_info(info),
						_flags(flags),
                        _range(range),
                        _type(type),
                        _unit(unit),
                        _interpolation(interpolation),
                        _currentColor(_info.normal)
{
    // Clip value with min and max.
    {
        double v = value;
        axCLIP( v, _range.min, _range.max );
        _value = v;
    }

    _zeroToOneValue = ( _value - _range.min ) /
                      double( _range.max - _range.min );

    Update();
}

double axNumberBox::GetValue()
{
    return _value;
}

void axNumberBox::OnMouseEnter()
{
    _currentColor = _info.hover;
    Update();
}

void axNumberBox::OnMouseLeave()
{
    if(!IsGrabbed())
    {
        _currentColor = _info.normal;
        Update();
    }
}

void axNumberBox::OnMouseLeftDown(const axPoint& pos)
{

	_clickPosY = (pos - GetAbsoluteRect().position).y;
    GrabMouse();
    Update();

}

void axNumberBox::OnMouseLeftUp(const axPoint& pos)
{
    if(IsGrabbed())
    {
        UnGrabMouse();

        if(!GetAbsoluteRect().IsPointInside(pos))
        {
            _currentColor = _info.normal;
        }

        if(_events.value_change)
        {
        	_events.value_change(axNumberBoxMsg(_value));
        }
        Update();
    }
}

void axNumberBox::OnMouseLeftDragging(const axPoint& pos)
{
	axPoint pt(GetAbsoluteRect().position);
	axPoint p = pos - pt;

	_clickPosY = p.y - _clickPosY;

    double v = -_clickPosY / 1000.0;
    _zeroToOneValue += v;
    
    axCLIP( _zeroToOneValue, 0.0, 1.0 );

    _value = _zeroToOneValue * ( _range.max - _range.min ) + _range.min;

    if(_events.value_change)
    {
    	_events.value_change(axNumberBoxMsg(_value));
    }	
    Update();
}

void axNumberBox::OnPaint()
{
	axGC* gc = GetGC();
	axSize size = GetSize();
	axRect rect0(0, 0, size.x, size.y);

    gc->SetColor(_currentColor);
    gc->DrawRectangle(rect0);

    gc->SetColor(_info.contour);
    gc->DrawRectangleContour(rect0);

    gc->SetColor(_info.font_color);
    gc->SetFontSize(10);
    string v = to_string(_value);
    v.resize(4);
    gc->DrawStringAlignedCenter(v, rect0);
}

//axNumberBoxControl::axNumberBoxControl( axApp* app,
//                                        axWindow* parent,
//                                        const axID& id,
//                                        const string& label,
//                                        const axNumberBoxEvents& events,
//                                        const axNumberBoxInfo& box_info,
//                                        const axPoint& pos ):
//                                        axWindow( app, parent, id,
//                                                  axRect( pos, box_info.box_size + axSize( 4, 20 )) ),
//                                        // Members.
//                                        m_parent( parent ),
//                                        m_currentColor( box_info.bgColorNormal),
//                                        m_label( label ),
//                                        m_eventID( events )
//{
//    SetCustomPaint( true );

//    m_numBox = new axNumberBox( app, this, axID_ANY, events, box_info, axPoint( 2, 20 ) );

//    Connect( events.valueChange, EVT( OnValueChange ) );
//}

//void axNumberBoxControl::OnValueChange()
//{
//    GetParent()->TriggerEvent( m_eventID.valueChange );
//}

//double axNumberBoxControl::GetValue()
//{
//    return m_numBox->GetValue();
//}

//void axNumberBoxControl::OnPaint()
//{
//    axGC gc( GetBackBuffer() );
//    axSize size = GetSize();

//    gc.SetColor( m_currentColor );

//    gc.DrawRectangle( axRect(0, 0, size.x, size.y) );

//    gc.DrawTextAligned( m_label,
//                        axTEXT_CENTER,
//                        m_currentColor.GetColorRGB(),
//                        "8",
//                        axRect( 0, 0, size.x, 20 ) );

//    FlipScreen( gc );
//}
