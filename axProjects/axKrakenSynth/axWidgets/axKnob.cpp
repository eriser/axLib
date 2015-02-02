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
#include "axObjectLoader.h"

/*******************************************************************************
 * axKnob::Msg.
 ******************************************************************************/
axKnob::Msg::Msg(const double& value):
_value(value)
{
}

double axKnob::Msg::GetValue() const
{
    return _value;
}

axMsg* axKnob::Msg::GetCopy()
{
    return new Msg(*this);
}

/*******************************************************************************
 * axKnob::Info.
 ******************************************************************************/
axKnob::Info::Info( const axColor& bg_normalColor,
                   const axColor& bg_hoverColor,
                   const axColor& bg_clickingColor,
                   const unsigned int& numberKnob,
                   const axSize& size,
                   const string& imgPath,
                   const string& sImgPath ):
// Members.
img_path( imgPath ),
selected_img_path( sImgPath ),
knob_size( size ),
n_knob( numberKnob ),
bgColorNormal( bg_normalColor ),
bgColorHover( bg_hoverColor ),
bgColorClicked( bg_clickingColor )
{
    
}

axKnob::Info::Info()
{
    
}

//axKnob::Info::Info(const std::string& path)
//{
//    axWidgetLoader loader;
//    axVectorPairString att = loader.GetAttributes(path);
//    
//    for(auto& n : att)
//    {
//        if(n.first == "normal")
//        {
//            bgColorNormal.LoadFromString(n.second);
//        }
//        else if(n.first == "hover")
//        {
//            bgColorHover.LoadFromString(n.second);
//        }
//        else if(n.first == "clicking")
//        {
//            bgColorClicked.LoadFromString(n.second);
//        }
//        else if(n.first == "nknob")
//        {
//            n_knob = stoi(n.second);
//        }
//        else if(n.first == "knob_size")
//        {
//            axStringVector strVec;
//            strVec = GetVectorFromStringDelimiter(n.second, ",");
//            knob_size = axSize(stoi(strVec[0]), stoi(strVec[1]));
//        }
//        else if(n.first == "img")
//        {
//            img_path = n.second;
//        }
//        else if(n.first == "selected_img")
//        {
//            selected_img_path = n.second;
//        }
//    }
//}

axKnob::Info::Info(const std::string& path):
// Heritage.
axInfo(path)
{
    axWidgetLoader loader;
    axVectorPairString att = loader.GetAttributes(path);
    
    SetAttributes(att);
}

axKnob::Info::Info(const axVectorPairString& attributes)
{
    SetAttributes(attributes);
}

axStringVector axKnob::Info::GetParamNameList() const
{
    return axStringVector{"normal", "hover", "clicking",
        "knob_size", "nknob", "img", "selected_img"};
}

std::string axKnob::Info::GetAttributeValue(const std::string& name)
{
    if(name == "normal")
    {
        return bgColorNormal.ToString();
    }
    else if(name == "hover")
    {
        return bgColorHover.ToString();
    }
    else if(name == "clicking")
    {
        return bgColorClicked.ToString();
    }
    else if(name == "nknob")
    {
        return to_string(n_knob);
    }
    else if(name == "knob_size")
    {
        return to_string(knob_size.x) + "," + to_string(knob_size.y);
    }
    else if(name == "img")
    {
        return img_path;
    }
    else if(name == "selected_img")
    {
        return selected_img_path;
    }
    
    return "";
}

void axKnob::Info::SetAttribute(const axStringPair& attribute)
{
    if(attribute.first == "normal")
    {
        bgColorNormal.LoadFromString(attribute.second);
    }
    else if(attribute.first == "hover")
    {
        bgColorHover.LoadFromString(attribute.second);
    }
    else if(attribute.first == "clicking")
    {
        bgColorClicked.LoadFromString(attribute.second);
    }
    else if(attribute.first == "nknob")
    {
        n_knob = stoi(attribute.second);
    }
    else if(attribute.first == "knob_size")
    {
        axStringVector strVec;
        strVec = GetVectorFromStringDelimiter(attribute.second, ",");
        knob_size = axSize(stoi(strVec[0]), stoi(strVec[1]));
    }
    else if(attribute.first == "img")
    {
        img_path = attribute.second;
    }
    else if(attribute.first == "selected_img")
    {
        selected_img_path = attribute.second;
    }
}

/*******************************************************************************
 * axKnob::Builder.
 ******************************************************************************/
axKnob::Builder::Builder(axWindow* parent,
                         const axSize& size,
                         const axKnob::Info& info,
                         axFlag flags,
                         int nextPositionDelta,
                         axDirection direction):
_parent(parent),
_size(size),
_info(info),
_flags(flags),
_direction(direction),
_nextPositionDelta(nextPositionDelta),
_pastKnob(nullptr)

{
    
}

axKnob::Builder::Builder(axWindow* parent,
                         axFlag flags,
                         int nextPositionDelta,
                         axDirection direction):
_parent(parent),
_pastKnob(nullptr)

{
    
}

axKnob* axKnob::Builder::Create(const axPoint& pos, const axEventFunction& evt)
{
    return _pastKnob = new axKnob(_parent, axRect(pos, _size), evt,
                                  _info, _flags);
}

axKnob* axKnob::Builder::Create(const axPoint& pos)
{
    axKnob::Events evts;
    return _pastKnob = new axKnob(_parent, axRect(pos, _size), evts,
                                  _info, _flags);
}

axKnob* axKnob::Builder::Create(const axEventFunction& evt)
{
    if(_pastKnob != nullptr)
    {
        if(_direction == axDIRECTION_RIGHT)
        {
            axPoint pos(_pastKnob->GetNextPosRight(_nextPositionDelta));
            return _pastKnob = new axKnob(_parent, axRect(pos, _size), evt,
                                          _info, _flags);
        }
        else if(_direction == axDIRECTION_DOWN)
        {
            
        }
        else if(_direction == axDIRECTION_LEFT)
        {
            
        }
        
        else //axDIRECTION_UP
        {
            
        }
        
    }
    
    return nullptr;
}

axKnob* axKnob::Builder::Create()
{
    if(_pastKnob != nullptr)
    {
        axKnob::Events evts;
        axPoint pos(_pastKnob->GetNextPosRight(_nextPositionDelta));
        return _pastKnob = new axKnob(_parent, axRect(pos, _size),
                                      evts, _info, _flags);
    }
    
    return nullptr;
}

axKnob* axKnob::Builder::Create(axVectorPairString attributes)
{
    std::string name;
    axPoint pos;
    axKnob::Events evts;
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
            _info = axKnob::Info(s.second);
        }
        else if(s.first == "flags")
        {
            _flags = stoi(s.second);
        }
        else if(s.first == std::string("event"))
        {
            evts.value_change = _parent->GetEventFunction(s.second);
        }
        
    }
    
    axKnob* knob = new axKnob(_parent, axRect(pos, _size),
                              evts, _info);
    
    _parent->GetResourceManager()->Add(name, knob);
    return knob;
}

/*******************************************************************************
 * axKnob.
 ******************************************************************************/
axKnob::axKnob(axWindow* parent,
               const axRect& rect,
               const axKnob::Events& events,
               const axKnob::Info& info,
               axFlag flags,
               double value):
// Heritage.
axWidget(parent, rect, new axKnob::Info(info)),
// Members.
_events(events),
//_info(info),
m_currentBgColor(&static_cast<Info*>(_info)->bgColorNormal),
//m_nCurrentImg(0),
m_knobValue(0.0),
_zeroToOneValue(0.0),
_range(0.0, 1.0)
{
    m_knobImg = new axImage(static_cast<Info*>(_info)->img_path);
    _bgAlpha = 1.0;
    
    m_nCurrentImg = m_knobValue * (static_cast<Info*>(_info)->n_knob - 1);
    
    if(_events.value_change)
    {
        AddConnection(0, _events.value_change);
    }
}

void axKnob::SetInfo(const axVectorPairString& attributes)
{
    _info->SetAttributes(attributes);

    /// @todo Major leak.
    std::string path = _info->GetAttributeValue("img");
    if(m_knobImg->GetImagePath() != path)
    {
        m_knobImg = new axImage(path);
    }

    Update();
}

void axKnob::OnMouseLeftDown(const axPoint& pos)
{
    _clickPosY = (pos - GetAbsoluteRect().position).y;

    GrabMouse();
    Update();
    
    HideMouse();

    PushEvent(0, new Msg(m_knobValue));
}

void axKnob::OnMouseLeftUp(const axPoint& pos)
{
    if( IsGrabbed() )
    {
        ShowMouse();
        UnGrabMouse();
        Update();

        PushEvent(0, new Msg(m_knobValue));
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

    m_nCurrentImg = m_knobValue * ( static_cast<Info*>(_info)->n_knob - 1 ) ;
    
    if( m_nCurrentImg != cur_img )
    {
        Update();
    }

    PushEvent(0, new Msg(m_knobValue));
}

void axKnob::SetValue(const axFloat& value, bool callValueChangeEvent)
{
	int cur_img = m_nCurrentImg;
	_zeroToOneValue = axClamp<double>(value, 0.0, 1.0);
    m_knobValue = _zeroToOneValue;
	m_nCurrentImg = m_knobValue * (static_cast<Info*>(_info)->n_knob - 1);

	if (m_nCurrentImg != cur_img)
	{
		Update();
	}

    if(callValueChangeEvent)
    {
        PushEvent(0, new Msg(m_knobValue));
    }
}

void axKnob::OnPaint()
{
    axGC* gc = GetGC();
    axSize size = GetSize();
    axRect rect0(0, 0, size.x, size.y);
	
	gc->SetColor(*m_currentBgColor);

    gc->DrawRectangle(rect0);

    gc->DrawPartOfImage(m_knobImg,
                        axPoint( m_nCurrentImg * static_cast<Info*>(_info)->knob_size.x, 0),
                        static_cast<Info*>(_info)->knob_size,
                        axPoint(0, 0));
}

/*******************************************************************************
 * axKnobControl.
 ******************************************************************************/
axKnobControl::axKnobControl(axWindow* parent,
                             const axRect& rect,
                             const axKnob::Events& events,
                             const axKnob::Info& info,
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
    axKnob::Events evts;
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

void axKnobControl::OnKnobValueChange(const axKnob::Msg& msg)
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
    
    axFont font("FreeSans.ttf");
//    gc->SetFontSize(12);
    gc->SetColor(axColor(0.0, 0.0, 0.0));
    gc->DrawStringAlignedCenter(font, _label, labelRect);
    
//    gc->SetFontSize(10);
    font.SetFontSize(10);
    gc->DrawStringAlignedCenter(font, _value, axRect(0, rect0.size.y - 20,
                                               rect0.size.x, 20));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0, 0.3));
    gc->DrawRectangleContour(rect0);

}
