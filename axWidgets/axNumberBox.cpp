#include "axNumberBox.h"

axNumberBox::axNumberBox(axWindow* parent,
						 const axRect& rect,
						 const axNumberBoxEvents& events,
						 const axNumberBoxInfo& info,
						 axFlag flags,
                         double value,
                         axFloatRange range,
                         axControlType type,
                         axControlUnit unit,
                         axControlInterpolation interpolation,
                         std::string label):

                        axPanel(parent, rect),
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
    double v = value;
    _value = axClamp<double>(v, _range.left, _range.left);
    
    _zeroToOneValue = _range.GetZeroToOneValue(_value);
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

    double delta = p.y - _clickPosY;
    
    _clickPosY = p.y;

    double v = -delta / 100.0;
    _zeroToOneValue += v;
    
    _zeroToOneValue = axClamp<double>(_zeroToOneValue, 0.0, 1.0);
    _value = _range.GetValueFromZeroToOne(_zeroToOneValue);

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
    std::string v = to_string(_value);
    v.resize(4);
    gc->DrawStringAlignedCenter(v, rect0);
}
