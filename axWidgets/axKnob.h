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
#ifndef DEF_AX_KNOB_H
#define DEF_AX_KNOB_H

/// @defgroup Widgets
/// @{

/// @defgroup Knob
/// @{

#include "axWindow.h"
#include "axImage.h"
#include "axWidget.h"
#include "axObjectLoader.h"

/**************************************************************************//**
 * axKnobMsg
******************************************************************************/
class axKnobMsg : public axMsg
{
public:
    axKnobMsg(const double& value):
        _value(value)
    {
    }

    double GetValue() const
    {
        return _value;
    }
    
    axMsg* GetCopy()
    {
        return new axKnobMsg(*this);
    }

private:
    double _value;
};

/**************************************************************************//**
 * axKnobEvents.
******************************************************************************/
struct axKnobEvents
{
    axEventFunction value_change;

    axKnobEvents(){}
    axKnobEvents(const axEventFunction& fct){ value_change = fct; }
};

/********************************************************************************//**
 * axKnobInfo.
 ***********************************************************************************/
struct axKnobInfo
{
    string img_path, selected_img_path;

    axSize knob_size;

    unsigned int n_knob;

    axColor bgColorNormal,
            bgColorHover,
            bgColorClicked;

    axKnobInfo( const axColor& bg_normalColor,
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
                bgColorClicked( bg_clickingColor ){}
    
    axKnobInfo(){}

    axKnobInfo(const std::string& path)
    {
        axWidgetLoader loader;
        axVectorPairString att = loader.GetAttributes(path);

        for(auto& n : att)
        {
            if(n.first == "normal")
            {
                bgColorNormal.LoadFromString(n.second);
            }
            else if(n.first == "hover")
            {
                bgColorHover.LoadFromString(n.second);
            }
            else if(n.first == "clicking")
            {
                bgColorClicked.LoadFromString(n.second);
            }
            else if(n.first == "nknob")
            {
                n_knob = stoi(n.second);
            }
            else if(n.first == "knob_size")
            {
                axStringVector strVec;
                strVec = GetVectorFromStringDelimiter(n.second, ",");
                knob_size = axSize(stoi(strVec[0]), stoi(strVec[1]));
            }
            else if(n.first == "img")
            {
                img_path = n.second;
            }
            else if(n.first == "selected_img")
            {
                selected_img_path = n.second;
            }
        }
    }
};

/********************************************************************************//**
 * axKnob.
 ***********************************************************************************/
class axKnob: public axPanel
{
public:
    axKnob(axWindow* parent,
           const axRect& rect,
           const axKnobEvents& events,
           const axKnobInfo& info,
           axFlag flags = 0,
           double value = 0.0);

    double GetValue() { return m_knobValue; }

	void SetValue(const axFloat& value, bool callValueChangeEvent = true);

	void SetBackgroundAlpha(const float& alpha)
	{
		_bgAlpha = alpha;
		Update();
	}
    
    class axKnobBuilder
    {
    public:
        axKnobBuilder(axWindow* parent,
                      const axSize& size,
                      const axKnobInfo& info,
                      axFlag flags = 0,
                      int nextPositionDelta = 5,
                      axDirection direction = axDIRECTION_RIGHT):
        _parent(parent),
        _size(size),
        _info(info),
        _flags(flags),
        _direction(direction),
        _nextPositionDelta(nextPositionDelta),
        _pastKnob(nullptr)
        
        {
            
        }
        
        axKnobBuilder(axWindow* parent,
                      axFlag flags = 0,
                      int nextPositionDelta = 5,
                      axDirection direction = axDIRECTION_RIGHT):
        _parent(parent),
        _pastKnob(nullptr)
        
        {
            
        }
        
        axKnob* Create(const axPoint& pos, const axEventFunction& evt)
        {
            return _pastKnob = new axKnob(_parent, axRect(pos, _size), evt,
                                          _info, _flags);
        }
        
        axKnob* Create(const axPoint& pos)
        {
            axKnobEvents evts;
            return _pastKnob = new axKnob(_parent, axRect(pos, _size), evts,
                                          _info, _flags);
        }
        
        axKnob* Create(const axEventFunction& evt)
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
        
        axKnob* Create()
        {
            if(_pastKnob != nullptr)
            {
                axKnobEvents evts;
                axPoint pos(_pastKnob->GetNextPosRight(_nextPositionDelta));
                return _pastKnob = new axKnob(_parent, axRect(pos, _size),
                                              evts, _info, _flags);
            }
            
            return nullptr;
        }
        
        axKnob* Create(axVectorPairString attributes)
        {
            std::string name;
            axPoint pos;
            axKnobEvents evts;
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
                    _info = axKnobInfo(s.second);
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

        
    private:
        axWindow* _parent;
        axKnobInfo _info;
        axFlag _flags;
        axSize _size;
        int _nextPositionDelta;
        axKnob* _pastKnob;
        axDirection _direction;
    };

private:
    axKnobEvents _events;
    axKnobInfo _info;
    axFlag _flags;
    axFloatRange _range;
    

    axImage* m_knobImg;
    axColor m_currentBgColor;
    unsigned int m_nCurrentImg;
    double m_knobValue;
    int _clickPosY;
	float _bgAlpha;
    double _zeroToOneValue;

    void OnPaint();
    void OnMouseLeftDragging(const axPoint& pos);
    void OnMouseLeftDown(const axPoint& pos);
    void OnMouseLeftUp(const axPoint& pos);
};

/********************************************************************************//**
 * axKnobControl.
 ***********************************************************************************/
 class axKnobControl: public axPanel
 {
 public:
     axKnobControl(axWindow* parent,
                   const axRect& rect,
                   const axKnobEvents& events,
                   const axKnobInfo& info,
                   const std::string& label,
                   axFlag flags = 0,
                   double value = 0.0);
     
     axEVENT_ACCESSOR(axKnobMsg, OnKnobValueChange);
     
     void SetValue(const double& value);
     
 private:
     axKnob* _knob;
     std::string _label;
     std::string _value;
     
     void OnKnobValueChange(const axKnobMsg& msg);
     
     void OnPaint();
 };

/********************************************************************************//**
 * Knob Templates.
 ***********************************************************************************/
#define axKNOB_INFO_SMALL_RED axKnobInfo( axColor("#777777"),            \
                                          axColor("#999999"),            \
                                          axColor("#999999"),            \
                                          128,                           \
                                          axSize( 32, 32 ),              \
                                          "knob.png",         \
                                          "knobSelected.png")

#define axKNOB_INFO_SIMPLE_BLUE axKnobInfo( axColor("#888888"),            \
                                            axColor("#AAAAAA"),            \
                                            axColor("#BBBBBB"),            \
                                            128,                           \
                                            axSize( 32, 32 ),              \
                                            "ressources/images/knobs/simple_blue_knob.png", \
                                            "ressources/images/knobs/simple_blue_knob.png")

/// @}
/// @}
#endif // DEF_AX_KNOB_H.
