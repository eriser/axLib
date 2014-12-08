#include "axNumberBox.h"

axNumberBox::axNumberBox(axWindow* parent,
						 const axRect& rect,
						 const axNumberBoxEvents& events,
						 const axNumberBoxInfo& info,
                         std::string img_path,
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
                        _currentColor(_info.normal),
                        _nCurrentImg(axNUM_BOX_NORMAL)
{
    _bgImg = new axImage(img_path);
    
    double v = value;
    std::cout << v << std::endl;
    _value = axClamp<double>(value, _range.left, _range.right);
    
    
    std::cout << _value << std::endl;
    
    _zeroToOneValue = _range.GetZeroToOneValue(_value);
    
    std::cout << _zeroToOneValue << std::endl;
    
    if(_events.value_change)
    {
        AddConnection(axNumberBoxEvents::VALUE_CHANGE,
                      _events.value_change);
    }
}

double axNumberBox::GetValue()
{
    return _value;
}

void axNumberBox::OnMouseEnter()
{
    _currentColor = _info.hover;
    _nCurrentImg = axNUM_BOX_HOVER;
    Update();
}

void axNumberBox::OnMouseLeave()
{
    
    if(!IsGrabbed())
    {
        _currentColor = _info.normal;
        _nCurrentImg = axNUM_BOX_NORMAL;
        Update();
    }
}

void axNumberBox::OnMouseLeftDown(const axPoint& pos)
{

	_clickPosY = (pos - GetAbsoluteRect().position).y;
    _nCurrentImg = axNUM_BOX_DOWN;
    GrabMouse();
    Update();

}

void axNumberBox::OnMouseLeftUp(const axPoint& pos)
{
    if(IsGrabbed())
    {
        UnGrabMouse();
        
        if(IsMouseHoverWindow())
        {
            _currentColor = _info.hover;
            _nCurrentImg = axNUM_BOX_HOVER;
        }
        else
        {
            _currentColor = _info.normal;
            _nCurrentImg = axNUM_BOX_NORMAL;
        }

        PushEvent(axNumberBoxEvents::VALUE_CHANGE, new axNumberBoxMsg(_value));
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

    PushEvent(axNumberBoxEvents::VALUE_CHANGE, new axNumberBoxMsg(_value));
    
    Update();
}

void axNumberBox::OnPaint()
{
	axGC* gc = GetGC();
	axSize size = GetSize();
	axRect rect0(0, 0, size.x, size.y);

    gc->SetColor(_currentColor);
    gc->DrawRectangle(rect0);

    if(_bgImg->IsImageReady())
    {
        if (axFlag_exist(axNUMBER_BOX_SINGLE_IMG, _flags))
        {
            gc->DrawImageResize(_bgImg, axPoint(0, 0), rect0.size);
        }
        else
        {
            gc->DrawPartOfImage(_bgImg, axPoint(0, _nCurrentImg * rect0.size.y),
                                rect0.size, axPoint(0, 0));
        }
        
    }

    gc->SetColor(_info.font_color);
    gc->SetFontSize(10);
    std::string v = to_string(_value);
    v.resize(4);
    gc->DrawStringAlignedCenter(v, rect0);
    
    gc->SetColor(_info.contour);
    gc->DrawRectangleContour(rect0);
}
