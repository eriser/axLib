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
                axPanel(parent, rect),
                _events(events),
                _info(info),
                // m_knobImg( info.img_path.c_str() ),
                m_currentBgColor( _info.bgColorNormal ),
                m_nCurrentImg( 0 ),
                m_knobValue( 0 )
{
    m_knobImg = new axImage(_info.img_path);
	_bgAlpha = 1.0;
}

void  axKnob::OnMouseLeftDown(const axPoint& pos)
{
    _clickPosY = (pos - GetAbsoluteRect().position).y;

    GrabMouse();
    Update();

    if(_events.value_change)
    {
        _events.value_change(axKnobMsg(m_knobValue));
    }

}

void axKnob::OnMouseLeftUp(const axPoint& pos)
{
    if( IsGrabbed() )
    {
        UnGrabMouse();
        Update();

        if(_events.value_change)
        {
            _events.value_change(axKnobMsg(m_knobValue));
        }
    }
}

void  axKnob::OnMouseLeftDragging(const axPoint& pos)
{
    int cur_img = m_nCurrentImg;


    // double v = - GetDeltaFromUpDownDirectionPoint().y / 1000.0;

    axPoint pt(GetAbsoluteRect().position);
    axPoint p = pos - pt;
    _clickPosY = p.y - _clickPosY;
    double v = -_clickPosY / 1000.0;


    m_knobValue += v;
    axCLIP( m_knobValue, 0.0, 1.0 );
    m_nCurrentImg = m_knobValue * ( _info.n_knob - 1 ) ;

    if( m_nCurrentImg != cur_img )
    {
        Update();
    }

    // cout << "Draggin" << endl;

    if(_events.value_change)
    {
        _events.value_change(axKnobMsg(m_knobValue));
    }

    // GetParent()->TriggerEvent( m_eventsID.motion );
}

void axKnob::SetValue(const axFloat& value)
{
	int cur_img = m_nCurrentImg;
	m_knobValue = value;
	axCLIP(m_knobValue, 0.0, 1.0);
	m_nCurrentImg = m_knobValue * (_info.n_knob - 1);

	if (m_nCurrentImg != cur_img)
	{
		Update();
	}

	if (_events.value_change)
	{
		_events.value_change(axKnobMsg(m_knobValue));
	}
}

void axKnob::OnPaint()
{
    axGC* gc = GetGC();
    axSize size = GetSize();
    axRect rect0(0, 0, size.x, size.y);
	
	gc->SetColor(m_currentBgColor, _bgAlpha);

    gc->DrawRectangle(rect0);

    gc->DrawPartOfImage(m_knobImg,
                       axPoint( m_nCurrentImg * _info.knob_size.x, 0),
                       _info.knob_size,
                       axPoint( 0, 0 ) );
    // FlipScreen( gc );
}

// /********************************************************************************//**
// * axKnobControl.
// ***********************************************************************************/
// axKnobControl::axKnobControl( axApp* app,
//                               axWindow* parent,
//                               const axID& id,
//                               const axKnobEvents& events,
//                               const axKnobInfo& info,
//                               const axPoint& pos ):
//                               axWindow( app, parent, id,
//                                         axRect( pos, info.knob_size + axSize( 14, 40 ) ) ),
//                               // Members.
//                               m_currentBgColor( info.bgColorNormal )
// {
//     SetCustomPaint( true );
//     axKnobEvents knob_events;

//     knob_events.motion = axID_ANY;
//     axRect knob_rect( axPoint( 7, 20 ), info.knob_size );
//     m_knob = new axKnob( app, this, axID_ANY, knob_rect, knob_events, info );

//     Connect( knob_events.motion, EVT( OnKnobChange ) );
// }

// void axKnobControl::OnKnobChange()
// {
//     Update();
// }

// void axKnobControl::OnPaint()
// {
//     axGC gc( GetBackBuffer() );
//     axSize size = GetSize();

//     gc.SetBackgroundColor( m_currentBgColor );
//     gc.SetForegroundColor( m_currentBgColor );

//     gc.DrawRectangle( axRect(0, 0, size.x, size.y) );
//     gc.DrawRectangle( axRect(0, 0, size.x, size.y) );

//     gc.SetColor( m_currentBgColor );
//     gc.DrawRectangleContour( axRect(0, 0, size.x, size.y) );

//     gc.DrawTextAligned( "Freq", axTEXT_CENTER,
//                         m_currentBgColor.GetColorRGB(),
//                         "8",
//                         axRect( axPoint(0, 0), axSize( size.x, 20) ) );

//     string s = axFloatToString( m_knob->GetValue() );

//     gc.DrawTextAligned( s, axTEXT_CENTER,
//                         m_currentBgColor.GetColorRGB(),
//                         "8",
//                         axRect( axPoint(0, 20 + m_knob->GetSize().y - 3 ), axSize( size.x, 20) ) );
//     FlipScreen( gc );
// }

