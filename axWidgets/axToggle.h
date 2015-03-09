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

#ifndef __AX_TOGGLE__
#define __AX_TOGGLE__

/*******************************************************************************
 * @file    axToggle
 * @author  Alexandre Arsenault <alx.arsenault@gmail.com>
 * @brief   axToggle.
 * @date    19/07/2013
 ******************************************************************************/

/// @defgroup Widgets
/// @{

/// @defgroup Toggle
/// @{

#include "axWidget.h"
#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include "axMsg.h"
#include <fstream>

/*******************************************************************************
 * axToggle.
 ******************************************************************************/
class axToggle : public axWidget
{
public:
    /***************************************************************************
     * axToggle::Flags.
     **************************************************************************/
    class Flags
    {
    public:
        static const axFlag SINGLE_IMG;
        static const axFlag IMG_RESIZE;
        static const axFlag CLICK_ON_LEFT_DOWN;
        static const axFlag CANT_UNSELECT_WITH_MOUSE;
    };
    
    /***************************************************************************
     * axToggle::Msg.
     **************************************************************************/
    class Msg : public axMsg
    {
    public:
        Msg();
        
        Msg(axToggle* sender, const bool& selected);
        
        Msg(axToggle* sender, const bool& selected, const std::string& msg);
        
        axToggle* GetSender() const;
        
        bool GetSelected() const;
        
        string GetMsg() const;
        
        axMsg* GetCopy();
        
    private:
        axToggle* _sender;
        bool _selected;
        std::string _msg;
    };
    
    /***************************************************************************
     * axToggle::Events.
     **************************************************************************/
    class Events
    {
    public:
        enum : axEventId { BUTTON_CLICK };
        
        axEventFunction button_click;
        
        Events(){}
        Events(const axEventFunction& fct){ button_click = fct; }
    };
    
    /***************************************************************************
     * axToggle::Info.
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
             const axColor& selectedHover_color,
             const axColor& selectedClicking_color,
             const axColor& contour_color,
             const axColor& font_color_,
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
        axColor selected_hover;
        axColor selected_clicking;
        
        /// @todo Add select font color to axToggle.
        axColor selected_font_color;
        
        axColor contour;
        axColor font_color;
        int font_size = 12;
        
        std::string img;
        bool single_img;
    };
    
    /***************************************************************************
     * axToggle::Builder.
     **************************************************************************/
    class Builder
    {
    public:
        Builder(axPanel* parent,
                const axSize& size,
                const axToggle::Info& info,
                string img_path = "",
                string label = "",
                axFlag flags = 0,
                int nextPositionDelta = 5,
                axDirection direction = axDIRECTION_RIGHT);
        
        Builder(axWindow* win);
        
        axToggle* Create(axVectorPairString attributes);
    
    private:
        axWindow* _parent;
        axToggle::Info _info;
        std::string _label;
        std::string _img;
        axFlag _flags;
        axSize _size;
        int _nextPositionDelta;
        axToggle* _past;
        axDirection _direction;
    };
    
    /***************************************************************************
     * axToggle::axToggle.
     **************************************************************************/
    axToggle(axWindow* parent,
             const axRect& rect,
             const axToggle::Events& events,
             const axToggle::Info& info,
             string img_path = "",
             string label = "",
             axFlag flags = 0,
             string msg = "");
        
    void SetMsg(const string& msg);
    void SetSelected(const bool& selected);

protected:
	enum axToggleState
	{
		axTOG_NORMAL,
		axTOG_HOVER,
		axTOG_CLICK,
		axTOG_SEL_NORMAL,
		axTOG_SEL_HOVER,
		axTOG_SEL_CLICK
	};

	int _nCurrentImg;
private:
    axToggle::Events _events;
	axColor* _currentColor;
	axImage* _bgImg;
    std::string _label;
    std::string _msg;
    axFont* _font;
	
	bool _selected;
	axFlag _flags;
	axColor test;
	axFloat _bgAlpha;



    // Events.
	virtual void OnPaint();
	virtual void OnMouseLeftDown(const axPoint& pos);
	virtual void OnMouseLeftUp(const axPoint& pos);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
};

#define axSTANDARD_TOGGLE 	axToggleInfo( \
axColor(0.5, 0.5, 0.5),\
axColor(0.6, 0.6, 0.6),\
axColor(0.4, 0.4, 0.4),\
axColor(0.5, 0.5, 0.5),\
axColor(0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0))

#define axTOGGLE_TRANSPARENT 	axToggleInfo( \
axColor(0.5, 0.5, 0.5, 0.0),\
axColor(0.6, 0.6, 0.6, 0.0),\
axColor(0.4, 0.4, 0.4, 0.0),\
axColor(0.5, 0.5, 0.5, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 1.0))

/// @}
/// @}
#endif //__AX_TOGGLE__

