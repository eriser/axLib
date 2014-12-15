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

/// @defgroup Widgets
/// @{

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include "axMsg.h"
#include <fstream>


/**************************************************************************//**
 * axToggleFlags.
******************************************************************************/
#define axTOGGLE_SINGLE_IMG	axFLAG_1
#define axTOGGLE_IMG_RESIZE	axFLAG_2
#define axTOGGLE_CLICK_ON_LEFT_DOWN axFLAG_3
#define axTOGGLE_CANT_UNSELECT_WITH_MOUSE axFLAG_4

class axToggle;

class axToggleMsg : public axMsg
{
public:
	axToggleMsg()
	{
		_sender = nullptr;
	}

	axToggleMsg(axToggle* sender, const bool& selected)
	{
		_sender = sender;
		_selected = selected;
        _msg = std::string("");
	}
    
    axToggleMsg(axToggle* sender, const bool& selected, const std::string& msg)
    {
        _sender = sender;
        _selected = selected;
        _msg = msg;
    }

	axToggle* GetSender() const
	{
		return _sender;
	}
    
    bool GetSelected() const
    {
        return _selected;
    }
    
    string GetMsg() const
    {
        return _msg;
    }
    
    axMsg* GetCopy()
    {
        return new axToggleMsg(*this);
    }

private:
	axToggle* _sender;
    bool _selected;
    std::string _msg;
};

struct axToggleEvents
{
    enum : axEventId { BUTTON_CLICK };
    
	axEventFunction button_click;
	
	axToggleEvents(){}
	axToggleEvents(axEventFunction& fct){ button_click = fct; }
};

struct axToggleInfo
{
	axColor normal;
	axColor hover;
	axColor clicking;
    
	axColor selected;
    axColor selected_hover;
    axColor selected_clicking;
    
	axColor contour;
	axColor font_color;

	axToggleInfo(){}
	axToggleInfo(
		const axColor& normal_color,
		const axColor& hover_color,
		const axColor& clicked_color,
		const axColor& selected_color,
        const axColor& selectedHover_color,
        const axColor& selectedClicking_color,
		const axColor& contour_color,
		const axColor& font_color_) :
		normal(normal_color),
		hover(hover_color),
		clicking(clicked_color),
		selected(selected_color),
        selected_hover(selectedHover_color),
        selected_clicking(selectedClicking_color),
		contour(contour_color),
		font_color(font_color_){}

	axToggleInfo(const string& info_path)
	{
		ifstream file;
		file.open(info_path);

		if (file.fail())
		{
			cerr << "Problem opening file " << info_path << endl;
		}
		else
		{
			string line;

			axColor* ptr = &normal;
			while (file.good())
			{
				getline(file, line);
				*ptr++ = axColor(line);
			}
		}
	}
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

class axToggle : public axPanel
{
public:
	axToggle(axWindow* parent,
             const axRect& rect,
             const axToggleEvents& events,
             const axToggleInfo& info,
             string img_path = "",
             string label = "",
             axFlag flags = 0,
             string msg = "");

//	axToggle(axWindow* parent,
//			 const axToggleEvents& events,
//			 const string& path);

	void SetBackgroundAlpha(const float& alpha);

	void SetMsg(const string& msg);
	void SetSelected(const bool& selected);

private:
	axToggleEvents _events;
	axToggleInfo _info;
	axColor* _currentColor;
	axImage* _btnImg;
	string _label;
	string _msg;
	
	bool _selected;
	axFlag _flags;
	axColor test;
	axFloat _bgAlpha;

	enum axToggleState
	{
		axTOG_NORMAL,
		axTOG_DOWN,
		axTOG_HOVER,
		axTOG_SELECTED
	};

	int _nCurrentImg;

	axEvent OnPaint();
	axEvent OnMouseLeftDown(const axPoint& pos);
	axEvent OnMouseLeftUp(const axPoint& pos);
	axEvent OnMouseEnter();
	axEvent OnMouseLeave();
};


/// @}
#endif //__AX_TOGGLE__

