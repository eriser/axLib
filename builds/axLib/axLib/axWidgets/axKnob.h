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

/*******************************************************************************
 * @file    axKnob
 * @author  Alexandre Arsenault <alx.arsenault@gmail.com>
 * @brief   axKnob.
 * @date    12/01/2015
 ******************************************************************************/

/// @defgroup Widgets
/// @{

/// @defgroup Knob
/// @{

#include "axWindow.h"
#include "axImage.h"
#include "axWidget.h"

/*******************************************************************************
 * axKnob.
 ******************************************************************************/
class axKnob: public axWidget
{
public:
    /***************************************************************************
     * axKnob::Msg.
     **************************************************************************/
    class Msg : public axMsg
    {
    public:
        Msg(const double& value);
        
        double GetValue() const;
        
        axMsg* GetCopy();
        
    private:
        double _value;
    };
    
    /***************************************************************************
     * axKnob::Events.
     **************************************************************************/
    class Events
    {
    public:
        axEventFunction value_change;
        
        Events(){}
        Events(const axEventFunction& fct){ value_change = fct; }
    };
    
    /***************************************************************************
     * axKnob::Info.
     **************************************************************************/
    class Info : public axInfo
    {
    public:
        Info();
        
        Info(const std::string& path);
        
        Info(const axVectorPairString& attributes);
        
        Info(const axColor& bg_normalColor,
             const axColor& bg_hoverColor,
             const axColor& bg_clickingColor,
             const unsigned int& numberKnob,
             const axSize& size,
             const string& imgPath,
             const string& sImgPath);
        
        // Info needed for debug editor. Derived from axInfo.
        virtual axStringVector GetParamNameList() const;
        virtual std::string GetAttributeValue(const std::string& name);
        virtual void SetAttribute(const axStringPair& attribute);
        
        std::string img_path, selected_img_path;
        axSize knob_size;
        unsigned int n_knob;
        axColor bgColorNormal, bgColorHover, bgColorClicked;
    };
    
    /***************************************************************************
     * axKnob::Builder.
     **************************************************************************/
    class Builder
    {
    public:
        Builder(axWindow* parent,
                const axSize& size,
                const axKnob::Info& info,
                axFlag flags = 0,
                int nextPositionDelta = 5,
                axDirection direction = axDIRECTION_RIGHT);
        
        Builder(axWindow* parent,
                axFlag flags = 0,
                int nextPositionDelta = 5,
                axDirection direction = axDIRECTION_RIGHT);
        
        axKnob* Create(const axPoint& pos, const axEventFunction& evt);
        
        axKnob* Create(const axPoint& pos);
        
        axKnob* Create(const axEventFunction& evt);
        
        axKnob* Create();
        
        axKnob* Create(axVectorPairString attributes);
        
    private:
        axWindow* _parent;
        axKnob::Info _info;
        axFlag _flags;
        axSize _size;
        int _nextPositionDelta;
        axKnob* _pastKnob;
        axDirection _direction;
    };
    
    /***************************************************************************
     * axKnob::axKnob.
     **************************************************************************/
    axKnob(axWindow* parent,
           const axRect& rect,
           const axKnob::Events& events,
           const axKnob::Info& info,
           axFlag flags = 0,
           double value = 0.0);

    double GetValue() { return m_knobValue; }

	void SetValue(const axFloat& value, bool callValueChangeEvent = true);

	void SetBackgroundAlpha(const float& alpha)
	{
		_bgAlpha = alpha;
		Update();
	}
    
    virtual void SetInfo(const axVectorPairString& attributes);

private:
    axKnob::Events _events;
//    axKnob::Info _info;
    axFlag _flags;
    axFloatRange _range;

    axImage* m_knobImg;
    axColor* m_currentBgColor;
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
                   const axKnob::Events& events,
                   const axKnob::Info& info,
                   const std::string& label,
                   axFlag flags = 0,
                   double value = 0.0);
     
     axEVENT_ACCESSOR(axKnob::Msg, OnKnobValueChange);
     
     void SetValue(const double& value);
     
 private:
     axKnob* _knob;
     std::string _label;
     std::string _value;
     
     void OnKnobValueChange(const axKnob::Msg& msg);
     
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
