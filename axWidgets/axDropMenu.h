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

#ifndef __AX_DROP_MENU__
#define __AX_DROP_MENU__

/*******************************************************************************
 * @file    axDropMenu.h
 * @author  Alexandre Arsenault <alx.arsenault@gmail.com>
 * @brief   axDropMenu.
 * @date    12/12/2014
 ******************************************************************************/

/// @defgroup Widgets
/// @{

/// @defgroup DropMenu
/// @{

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include "axMsg.h"
#include "axPopupMenu.h"

/*******************************************************************************
 * axDropMenuFlags.
 ******************************************************************************/
#define axDROP_MENU_SINGLE_IMG	axFLAG_1
#define axDROP_MENU_SINGLE_BACKGROUND_IMG axFLAG_2

class axDropMenu;

/*******************************************************************************
 * axDropMenuMsg.
 ******************************************************************************/
class axDropMenuMsg : public axMsg
{
public:
    axDropMenuMsg()
    {
        _sender = nullptr;
    }
    
    axDropMenuMsg(axDropMenu* sender, const string& msg)
    {
        _sender = sender;
        _msg = msg;
    }
    
    axDropMenu* GetSender() const
    {
        return _sender;
    }
    
    string GetMsg() const
    {
        return _msg;
    }
    
    axMsg* GetCopy()
    {
        return new axDropMenuMsg(*this);
    }
    
private:
    axDropMenu* _sender;
    string _msg;
};

/*******************************************************************************
 * axDropMenuEvents.
 ******************************************************************************/
struct axDropMenuEvents
{
    enum : axEventId { SELECTION_CHANGE };
    
    axDropMenuEvents(){}
    axDropMenuEvents(const axEventFunction& fct){ selection_change = fct; }
    
    axEventFunction selection_change;
};

/*******************************************************************************
 * axDropMenuInfo.
 ******************************************************************************/
struct axDropMenuInfo
{
    axColor normal;
    axColor hover;
    axColor clicking;
    axColor selected;
    axColor contour;
    axColor font_color;
    
    axDropMenuInfo(){}
    axDropMenuInfo(const axColor& normal_color,
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
    font_color(font_color_){}
    
};

/*******************************************************************************
 * axDropMenu.
 ******************************************************************************/
class axDropMenu : public axPanel
{
public:
    axDropMenu(axWindow* parent,
               const axRect& rect,
               const axDropMenuEvents& events,
               const axDropMenuInfo& info,
               std::string img_path = "",
               std::string bg_img_path = "",
               std::string toogle_img_path = "",
               const std::vector<std::string>& menu_str = {},
               axFlag flags = 0,
               std::string msg = "");

    void SetMsg(const string& msg);
    
    void SetSelected(const bool& selected);
        
    axEVENT_ACCESSOR(axPopupMenuMsg, OnPopupMenuChoice);
    
private:
    axDropMenuEvents _events;
    axDropMenuInfo _info;
    
    axColor* _currentColor;
    axImage* _btnImg;
    axImage* _bgImg;
    string _label;
    string _msg;
    
    bool _selected;
    axFlag _flags;
    axColor test;
//    axFloat _bgAlpha;
    
    axPopupMenu* _popMenu;
    
    enum axDropMenuState
    {
        axDROP_NORMAL,
        axDROP_HOVER,
        axDROP_DOWN,
        axDROP_SELECTED
    };
    
    int _nCurrentImg;
    
    virtual void OnPaint();
    virtual void OnMouseLeftDown(const axPoint& pos);
    virtual void OnMouseLeftUp(const axPoint& pos);
    virtual void OnMouseEnter();
    virtual void OnMouseLeave();
    
    void OnPopupMenuChoice(const axPopupMenuMsg& msg);
};

/*******************************************************************************
 * axDropMenuInfo template.
 ******************************************************************************/
#define axDROP_MENU_TRANSPARENT 	axDropMenuInfo( \
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 0.0),\
axColor(0.0, 0.0, 0.0, 1.0))

/// @}
/// @}
#endif //__AX_DROP_MENU__

