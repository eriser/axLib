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

#ifndef __AX_NUMBER_BOX__
#define __AX_NUMBER_BOX__

/*******************************************************************************
 * @file    axNumberBox.h
 * @author  Alexandre Arsenault <alx.arsenault@gmail.com>
 * @brief   axNumberBox.
 * @date    19/07/2013
 ******************************************************************************/

/// @defgroup Widgets
/// @{

/// @defgroup NumberBox
/// @{

#include "axEvent.h"
#include "axWidget.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include "axMsg.h"

/***************************************************************************
 * axNumberBox.
 **************************************************************************/
class axNumberBox: public axWidget
{
public:
    /***************************************************************************
     * axNumberBox::Flags.
     **************************************************************************/
    class Flags
    {
    public:
        static const axFlag SINGLE_IMG;
        static const axFlag NO_IMG_RESIZE;
        static const axFlag LABEL;
        static const axFlag SLIDER;
        static const axFlag NO_MOUSE;
    };
    
    /***************************************************************************
     * axNumberBox::Msg.
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
     * axNumberBox::Events.
     **************************************************************************/
    class Events
    {
    public:
        enum : axEventId { VALUE_CHANGE };
        
        Events(){}
        Events(axEventFunction& fct){ value_change = fct; }
        
        axEventFunction value_change;
    };
    
    /***************************************************************************
     * axNumberBox::Info.
     **************************************************************************/
    class Info : public axInfo
    {
    public:
        Info();
        
        Info(const std::string& path);
        
        Info(const axVectorPairString& attributes);
        
        Info(const axColor& normal_color,
             const axColor& hover_color,
             const axColor& clicked_color,
             const axColor& selected_color,
             const axColor& contour_color,
             const axColor& fontColor,
             const std::string& img = "",
             const bool& singleImg = false);
  
        // Info needed for debug editor. Derived from axInfo.
        virtual axStringVector GetParamNameList() const;
        virtual std::string GetAttributeValue(const std::string& name);
        virtual void SetAttribute(const axStringPair& attribute);

        axColor normal;
        axColor hover;
        axColor clicking;
        axColor selected;
        axColor contour;
        axColor font_color;
        
        std::string img;
        bool single_img;
    };
    
    /***************************************************************************
     * axNumberBox::Builder.
     **************************************************************************/
    class Builder
    {
    public:
        Builder(axWindow* win);
        
        axNumberBox* Create(axVectorPairString attributes);
        
    private:
        axWindow* _parent;
        axNumberBox::Info _info;
        axFlag _flags;
        axSize _size;
        int _nextPositionDelta;
        axNumberBox* _past;
        axDirection _direction;
    };
    
    /***************************************************************************
     * axNumberBox::axNumberBox.
     **************************************************************************/
    axNumberBox(axWindow* parent,
                const axRect& rect,
                const axNumberBox::Events& events,
                const axNumberBox::Info& info,
                axFlag flags = 0,
                double value = 0.0,
                axFloatRange range = axFloatRange(0.0, 1.0),
                axControlType type = axCTRL_FLOAT,
                axControlUnit m_unit = axCTRL_NO_UNIT,
                axControlInterpolation interpolation = axCTRL_LINEAR,
                std::string label = "");

    double GetValue();
    
    virtual void SetInfo(const axVectorPairString& attributes);

	void SetValue(const double& value)
	{
		_value = value;
		Update();
	}
    
private:
    axNumberBox::Events _events;
    axFlag _flags;
    axImage* _bgImg;
    axFont* _font;

    axControlType _type;
    axControlUnit _unit;
    axFloatRange _range;
    axControlInterpolation _interpolation;

    axColor* _currentColor;
    int _nCurrentImg;
    double _value;
    double _zeroToOneValue;
    std::string _label;

    int _clickPosY;

    enum axNumberBoxState
    {
        axNUM_BOX_NORMAL,
        axNUM_BOX_HOVER,
        axNUM_BOX_DOWN
    };

    // Events.
    virtual void OnPaint();
    virtual void OnMouseEnter();
    virtual void OnMouseLeftDown(const axPoint& pos);
    virtual void OnMouseLeftUp(const axPoint& pos);
    virtual void OnMouseLeftDragging(const axPoint& pos);
    virtual void OnMouseLeave();

};

#define axNUMBER_BOX_STANDARD axNumberBox::Info( axColor("#AA1111"),          \
                                               axColor("#CC1111"),          \
                                               axColor("#FF1111"),          \
                                               axColor("#FF0000"),          \
                                               axColor("#000000") )

#define axNUMBER_BOX_WHITE axNumberBox::Info( axColor("#FFFFFF"),          \
                                            axColor("#DDDDDD"),          \
                                            axColor("#AAAAAA"),          \
                                            axColor("#FF0000"),          \
                                            axColor("#000000") )

/// @}
/// @}
#endif // __AX_NUMBER_BOX__
