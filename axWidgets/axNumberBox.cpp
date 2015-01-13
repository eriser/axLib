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
#include "axNumberBox.h"
#include "axObjectLoader.h"

/*******************************************************************************
 * axNumberBox::Flags.
 ******************************************************************************/
const axFlag axNumberBox::Flags::SINGLE_IMG = axFLAG_1;
const axFlag axNumberBox::Flags::LABEL = axFLAG_2;
const axFlag axNumberBox::Flags::SLIDER = axFLAG_3;
const axFlag axNumberBox::Flags::NO_MOUSE = axFLAG_4;

/*******************************************************************************
 * axNumberBox::Msg.
 ******************************************************************************/
axNumberBox::Msg::Msg(const double& value):
_value(value)
{
}

double axNumberBox::Msg::GetValue() const
{
    return _value;
}

axMsg* axNumberBox::Msg::GetCopy()
{
    return new Msg(*this);
}

/*******************************************************************************
 * axNumberBox::Info.
 ******************************************************************************/
axNumberBox::Info::Info()
{
    
}

axNumberBox::Info::Info(const axColor& normal_color,
                        const axColor& hover_color,
                        const axColor& clicked_color,
                        const axColor& selected_color,
                        const axColor& contour_color,
                        const axColor& font_color_) :
normal(normal_color),
hover(hover_color),
clicking(clicked_color),
selected(selected_color),
contour(contour_color),
font_color(font_color_)
{
    
}

axNumberBox::Info::Info(const std::string& path)
{
    axWidgetLoader loader;
    axVectorPairString att = loader.GetAttributes(path);
    
    for(auto& n : att)
    {
        if(n.first == "normal")
        {
            normal.LoadFromString(n.second);
        }
        else if(n.first == "hover")
        {
            hover.LoadFromString(n.second);
        }
        else if(n.first == "clicking")
        {
            clicking.LoadFromString(n.second);
        }
        else if(n.first == "selected")
        {
            selected.LoadFromString(n.second);
        }
        else if(n.first == "contour")
        {
            contour.LoadFromString(n.second);
        }
        else if(n.first == "font_color")
        {
            font_color.LoadFromString(n.second);
        }
    }
}

/*******************************************************************************
 * axNumberBox::Builder.
 ******************************************************************************/
axNumberBox::Builder::Builder(axWindow* win):
_parent(win),
_past(nullptr)
{
    
}

axNumberBox* axNumberBox::Builder::Create(axVectorPairString attributes)
{
    std::string name;
    axPoint pos;
    axNumberBox::Events evts;
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
            
            pos = axPoint(stoi(strVec[0]),
                          stoi(strVec[1]));
            
            _size = axSize(stoi(strVec[2]),
                           stoi(strVec[3]));
        }
        else if(s.first == "info")
        {
            _info = axNumberBox::Info(s.second);
        }
        else if(s.first == "flags")
        {
            _flags = stoi(s.second);
        }
        else if(s.first == "img")
        {
            _img = s.second;
        }
        else if(s.first == std::string("event"))
        {
            evts.value_change = _parent->GetEventFunction(s.second);
        }
        
    }
    
    axNumberBox* box = new axNumberBox(_parent, axRect(pos, _size),
                                       evts, _info, _img);
    
    _parent->GetResourceManager()->Add(name, box);
    return box;
}

/*******************************************************************************
 * axNumberBox::axNumberBox.
 ******************************************************************************/
axNumberBox::axNumberBox(axWindow* parent,
						 const axRect& rect,
                         const axNumberBox::Events& events,
                         const axNumberBox::Info& info,
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
//    std::cout << v << std::endl;
    _value = axClamp<double>(value, _range.left, _range.right);
    
    
//    std::cout << _value << std::endl;
    
    _zeroToOneValue = _range.GetZeroToOneValue(_value);
    
//    std::cout << _zeroToOneValue << std::endl;
    
    if(_events.value_change)
    {
        AddConnection(Events::VALUE_CHANGE, _events.value_change);
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

        PushEvent(Events::VALUE_CHANGE, new Msg(_value));
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

    PushEvent(Events::VALUE_CHANGE, new Msg(_value));
    
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
        if (IsFlag(Flags::SINGLE_IMG, _flags))
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
//    gc->SetFontSize(10);
    axFont font("FreeSans.ttf");
    font.SetFontSize(10);
    std::string v = to_string(_value);
    v.resize(4);
    gc->DrawStringAlignedCenter(font, v, rect0);
    
    gc->SetColor(_info.contour);
    gc->DrawRectangleContour(rect0);
}
