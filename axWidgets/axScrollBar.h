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
#ifndef __AX_SCROLL_BAR__
#define __AX_SCROLL_BAR__

/// @defgroup Widgets
/// @{

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include <fstream>

#include "axButton.h"

/**************************************************************************//**
 * axButtonFlags.
******************************************************************************/
// #define axBUTTON_SINGLE_IMG	axFLAG_1
// #define axBUTTON_IMG_RESIZE	axFLAG_2

class axScrollBar;

class axScrollBarMsg
{
public:
	axScrollBarMsg()
	{
		_sender = nullptr;
	}

	axScrollBarMsg(axScrollBar* sender, const string& msg)
	{
		_sender = sender;
		_msg = msg;
	}

	axScrollBar* GetSender() const
	{
		return _sender;
	}

	string GetMsg() const
	{
		return _msg;
	}

private:
	axScrollBar* _sender;
	string _msg;
};

struct axScrollBarEvents
{
	//axEvtFunction(axButtonMsg) button_click;
	std::function<void (axScrollBarMsg)> value_change;
	
	axScrollBarEvents(){}
	axScrollBarEvents(std::function<void (axScrollBarMsg)>& fct){ value_change = fct; }
};

struct axScrollBarInfo
{
	axColor normal;
	axColor hover;
	axColor clicking;
	axColor selected;
	axColor contour;
	axColor font_color;

	axScrollBarInfo(){}
	axScrollBarInfo(
		const axColor& normal_color,
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


class axScrollBar : public axPanel
{
public:
	axScrollBar(axWindow* parent,
		const axRect& rect,
		const axScrollBarEvents& events,
		const axScrollBarInfo& info,
		axFlag flags = 0);

	double getZeroToOneValue();

	// void setBackgroundImage(wxBitmap& bmp);
	void setSliderPositionZeroToOne(const double& value);
	void setInputTopDelta(int y);
	void setInputFrameHeight(int y);
	void setInputPanelHeight(int y);
	void setInputInfo(int frameSize, int panelSize, int topDelta);
	double getSliderValue() const;
	inline void recalculateInputValue();

private:
	axScrollBarEvents _events;
	axScrollBarInfo _info;
	axFlag _flags;

	axButton* _btn[2];

	//----------------------
	int m_imgHeight, m_sliderHeight, m_sliderMaxHeight;
	int m_inputFrameSize, m_inputPanelSize, m_inputTopDecay;
	int m_yClickDelta;
	
	double m_value;
	double m_sliderPosition, m_sliderValue;
	int m_sliderPositionPixel;


	virtual void OnPaint();
	virtual void OnMouseLeftDragging(const axPoint& pos);
	virtual void OnMouseLeftDown(const axPoint& pos);
	virtual void OnMouseLeftUp(const axPoint& pos);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
};

/// @}
#endif //__AX_SCROLL_BAR__

