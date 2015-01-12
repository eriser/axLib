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

#ifndef __AX_BUTTON__
#define __AX_BUTTON__

/*******************************************************************************
 * @file    axButton.h
 * @author  Alexandre Arsenault <alx.arsenault@gmail.com>
 * @brief   axButton.
 * @date    19/07/2013
 ******************************************************************************/

/// @defgroup Widgets
/// @{

/// @defgroup Button
/// @{

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include "axMsg.h"

/*******************************************************************************
 * axButon.
 ******************************************************************************/
class axButton : public axPanel
{
public:
    /***************************************************************************
     * axButton::Flags.
     **************************************************************************/
    class Flags
    {
    public:
        static const axFlag SINGLE_IMG;
        static const axFlag IMG_RESIZE;
        static const axFlag CAN_SELECTED;
    };
    
    /***************************************************************************
     * axButton::Msg.
     **************************************************************************/
    class Msg : public axMsg
    {
    public:
        Msg();
        
        Msg(axButton* sender, const std::string& msg);
        
        axButton* GetSender() const;
        
        std::string GetMsg() const;
        
        axMsg* GetCopy();
        
    private:
        axButton* _sender;
        std::string _msg;
    };
    
    /***************************************************************************
     * axButton::Events.
     **************************************************************************/
    class Events
    {
    public:
        enum : axEventId { BUTTON_CLICK };
        
        Events(){}
        Events(const axEventFunction& fct){ button_click = fct; }
        
        axEventFunction button_click;
    };
    
    /***************************************************************************
     * axButton::Info.
     **************************************************************************/
    class Info
    {
    public:
        Info();
        
        Info(const axColor& normal_color,
             const axColor& hover_color,
             const axColor& clicked_color,
             const axColor& selected_color,
             const axColor& contour_color,
             const axColor& font_color);
        
        Info(const std::string& path);
        
        void SerializeOutput(const std::string& path);
        
        void SerializeInput(const std::string& path);
        
        axColor normal;
        axColor hover;
        axColor clicking;
        axColor selected;
        axColor contour;
        axColor font_color;
    };
    
    /***************************************************************************
     * axButton::axButton.
     **************************************************************************/
	axButton(axWindow* parent,
             const axRect& rect,
             const axButton::Events& events,
             const axButton::Info& info,
             std::string img_path = "",
             std::string label = "",
             axFlag flags = 0,
             std::string msg = "");

	void SetMsg(const std::string& msg);
    
	void SetSelected(const bool& selected);
    
    void SetLabel(const std::string& label);
    
protected:
    axButton::Events _events;
    axButton::Info _info;
    std::string _label;
    axImage* _btnImg;
    axFlag _flags;
    std::string _msg;
    
    axColor* _currentColor;
    bool _selected;
    int _nCurrentImg;
    
    enum axButtonState
    {
        axBTN_NORMAL,
        axBTN_HOVER,
        axBTN_DOWN,
        axBTN_SELECTED
    };
    
	virtual void OnPaint();
	virtual void OnMouseLeftDown(const axPoint& pos);
	virtual void OnMouseLeftUp(const axPoint& pos);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
};

/*******************************************************************************
 * axButonInfo template.
 ******************************************************************************/
#define axSTANDARD_BUTTON 	axButton::Info( \
axColor(0.5, 0.5, 0.5),\
axColor(0.6, 0.6, 0.6),\
axColor(0.4, 0.4, 0.4),\
axColor(0.5, 0.5, 0.5),\
axColor(0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0))

#define axBUTTON_TRANSPARENT 	axButton::Info( \
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 1.0))

/// @}
/// @}
#endif //__AX_BUTTON__




