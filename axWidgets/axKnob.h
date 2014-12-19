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

/// @defgroup Grid
/// @{

#include "axWindow.h"
#include "axImage.h"
#include "axWidget.h"

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

	void SetValue(const axFloat& value);

	void SetBackgroundAlpha(const float& alpha)
	{
		_bgAlpha = alpha;
		Update();
	}

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
