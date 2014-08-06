/// @defgroup Widgets 
/// @{

#ifndef __AX_SCROLL_BAR__
#define __AX_SCROLL_BAR__

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
	axScrollBar(axApp* app,
		axWindow* parent,
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

#endif //__AX_SCROLL_BAR__

