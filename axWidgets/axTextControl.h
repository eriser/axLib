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
#ifndef __AX_TEXT_CONTROL__
#define __AX_TEXT_CONTROL__

/// @defgroup Widgets
/// @{

/// @defgroup TextControl
/// @{

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include "axMsg.h"
#include "axTimer.h"

/**************************************************************************//**
 * axTextControlFlags.
******************************************************************************/
#define axTEXT_CTRL_FLASHING_CURSOR      axFLAG_1
#define axTEXT_CTRL_CONTOUR_HIGHLIGHT    axFLAG_2
#define axTEXT_CTRL_CONOUR_NO_FADE       axFLAG_3

class axTextControl;

class axTextControlMsg : public axMsg
{
public:
    axTextControlMsg()
    {
        _sender = nullptr;
    }
    
    axTextControlMsg(axTextControl* sender, const string& msg)
    {
        _sender = sender;
        _msg = msg;
    }
    
    axTextControl* GetSender() const
    {
        return _sender;
    }
    
    string GetMsg() const
    {
        return _msg;
    }
    
    axMsg* GetCopy()
    {
        return new axTextControlMsg(*this);
    }
    
private:
    axTextControl* _sender;
    string _msg;
};

struct axTextControlEvents
{
    enum : axEventId { BUTTON_CLICK };
    
	axTextControlEvents(){}
    axTextControlEvents(axEventFunction& fct){ button_click = fct; }
    
    axEventFunction button_click;
};

struct axTextControlInfo
{
	axColor normal;
	axColor hover;
    axColor hightlight; // This needs to be transparent.
	axColor selected;
    axColor selected_shadow;
	axColor cursor;
	axColor contour;
	axColor font_color;

	axTextControlInfo(){}
	axTextControlInfo(const axColor& normal_color,
                  const axColor& hover_color,
                  const axColor& hightlight_color,
                  const axColor& selected_color,
                  const axColor& selected_shadow_color,
                  const axColor& cursor_color,
                  const axColor& contour_color,
                  const axColor& font_color_) :
    normal(normal_color),
    hover(hover_color),
    hightlight(hightlight_color),
    selected(selected_color),
    selected_shadow(selected_shadow_color),
    cursor(cursor_color),
    contour(contour_color),
    font_color(font_color_){}
    
    axTextControlInfo(const std::string& path)
    {
        SerializeInput(path);
    }

    void SerializeOutput(const std::string& path)
    {
        fstream file;
        file.open(path, std::fstream::out | std::fstream::binary);

        if (file.fail())
        {
            std::cerr << "Problem opening file " << path << std::endl;
        }
        else
        {
            normal.SerializeOutput(file);
            hover.SerializeOutput(file);
            selected.SerializeOutput(file);
            cursor.SerializeOutput(file);
            contour.SerializeOutput(file);
            font_color.SerializeOutput(file);
        }

    }
    
    void SerializeInput(const std::string& path)
    {
        fstream file;
        file.open(path, std::fstream::in | std::fstream::binary);
        
        if (file.fail())
        {
            std::cerr << "Problem opening file " << path << std::endl;
        }
        else
        {
            normal.SerializeInput(file);
            hover.SerializeInput(file);
            selected.SerializeInput(file);
            cursor.SerializeInput(file);
            contour.SerializeInput(file);
            font_color.SerializeInput(file);
        }
    }
};

class axTextControl : public axPanel
{
public:
	axTextControl(axWindow* parent,
                  const axRect& rect,
                  const axTextControlEvents& events,
                  const axTextControlInfo& info,
                  string img_path = "",
                  string label = "",
                  axFlag flags = 0);

    void SetLabel(const std::string& label);

    
    axEVENT_ACCESSOR(axTimerMsg, OnFlashingCursorTimer);
    
protected:
    axTextControlEvents _events;
    axTextControlInfo _info;
    string _label;
    axImage* _btnImg;
    axFlag _flags;
        
    axColor* _currentColor;
    int _nCurrentImg;
    int _cursorIndex;
    bool _isHightlight;
    bool _findClickCursorIndex;
    axPoint _clickPosition;
    
    int _cursorBarXPosition, _lastCharXPosition;
    
    enum axTextControlState
    {
        axBTN_NORMAL,
        axBTN_HOVER,
        axBTN_DOWN,
        axBTN_SELECTED
    };
    
    axTimer* _flashingCursor;
    
	virtual void OnPaint();
	virtual void OnMouseLeftDown(const axPoint& pos);
    virtual void OnMouseLeftDragging(const axPoint& pos);
	virtual void OnMouseLeftUp(const axPoint& pos);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
    virtual void OnMouseLeftDoubleClick(const axPoint& pos);
    virtual void OnKeyDown(const char& key);
    virtual void OnBackSpaceDown();
    
    virtual void OnLeftArrowDown();
    virtual void OnRightArrowDown();
    virtual void OnWasKeyUnGrabbed();
    virtual void OnWasKeyGrabbed();
    virtual void OnKeyDeleteDown();
    
    //
    virtual void DrawContourRectangle(axGC* gc);
    
    void OnFlashingCursorTimer(const axTimerMsg& msg);
    
    bool _cursorFlashActive;
};

/// @}
/// @}
#endif // __AX_TEXT_CONTROL__

