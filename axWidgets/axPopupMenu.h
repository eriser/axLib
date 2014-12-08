#ifndef __AX_POPUP_MENU__
#define __AX_POPUP_MENU__

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include "axMsg.h"
#include "axToggle.h"

/**************************************************************************//**
* axPopupMenuMsg
******************************************************************************/
class axPopupMenuMsg : public axMsg
{
public:
	axPopupMenuMsg(const string& msg) :
		_msg(msg)
	{
	}

	string GetMsg() const
	{
		return _msg;
	}
    
    axMsg* GetCopy()
    {
        return new axPopupMenuMsg(*this);
    }

private:
	string _msg;
};

/**************************************************************************//**
* axPopupMenuEvents.
******************************************************************************/
struct axPopupMenuEvents
{
    enum : axEventId { SELECTION_CHANGE };

	axPopupMenuEvents(){}
	axPopupMenuEvents(axEventFunction& fct){ selection_change = fct; }
    
    axEventFunction selection_change;
};

/**************************************************************************//**
* axPopupMenuInfo.
******************************************************************************/
struct axPopupMenuInfo
{
	axColor bgColorNormal,
			bgColorHover,
			bgColorSelected,
			lineColor,
			contourColor,
			fontColor;

	axPopupMenuInfo(){}
	axPopupMenuInfo(const axColor& bg_normal,
		const axColor& bg_hover,
		const axColor& bg_selected,
		const axColor& lineColor,
		const axColor& contour,
		const axColor& font_color) :
		// Members.
		bgColorNormal(bg_normal),
		bgColorHover(bg_hover),
		bgColorSelected(bg_selected),
		lineColor(lineColor),
		contourColor(contour),
		fontColor(font_color){}
};



/*******************************************************************************//**
* axPopupMenu.
***********************************************************************************/
class axPopupMenu : public axPanel
{
public:
	axPopupMenu(axWindow* parent,
				const axRect& rect,
				const axPopupMenuEvents& events,
				const axPopupMenuInfo& info,
                const std::vector<std::string>& labels,
                std::string bgImg = "",
				axFlag flags = 0);

	//~axPopupMenu();

	int GetIndexValue() { return _index; }
    void SetSelectedIndex(const int& index);
	

private:
	axPopupMenuEvents _events;
	axPopupMenuInfo _info;
	axFlag _flag;
	vector<string> _labels;
	vector<axToggle*> _btns;
	int _index;
	axToggle* _lastSelected;

	axEVENT_ACCESSOR(axToggleMsg, OnButtonClick);
	void OnButtonClick(const axToggleMsg& msg);

	virtual void OnPaint();
	virtual void OnMouseMotion(const axPoint& pos);
	virtual void OnMouseLeftUp(const axPoint& pos);

	static const int LABEL_HEIGHT = 20;

};
///**************************************************************************//**
// * axSliderFlags.
//******************************************************************************/
//#define axSLIDER_FLAG_VERTICAL			axFLAG_1
//#define axSLIDER_FLAG_CLICK_ANYWHERE    axFLAG_2
//#define axSLIDER_FLAG_RELEASE_ON_LEAVE  axFLAG_3
//#define axSLIDER_FLAG_LEFT_CLICK_ENTER  axFLAG_4
//#define axSLIDER_FLAG_RIGHT_ALIGN       axFLAG_5
//#define axSLIDER_FLAG_MIDDLE_ALIGN      axFLAG_6
//#define axSLIDER_FLAG_BACK_SLIDER       axFLAG_7
//#define axSLIDER_FLAG_NO_SLIDER_LINE    axFLAG_8
//
///**************************************************************************//**
// * axSliderMsg
//******************************************************************************/
//class axSliderMsg
//{
//public:
//	axSliderMsg(const double& value):
//		_value(value)
//	{
//	}
//
//	double GetValue() const
//	{
//		return _value;
//	}
//
//private:
//	double _value;
//};
//
///**************************************************************************//**
// * axSliderEvents.
//******************************************************************************/
//struct axSliderEvents
//{
//	axEvtFunction(axSliderMsg) slider_value_change;
//
//	axSliderEvents(){}
//	axSliderEvents(axEvtFunction(axSliderMsg)& fct){ slider_value_change = fct; }
//};
//
///**************************************************************************//**
// * axSliderInfo.
//******************************************************************************/
//struct axSliderInfo
//{
//	string img_path;
//	axSize btn_size;
//	unsigned int slider_width;
//
//	axColor bgColorNormal,
//			bgColorHover,
//			bgColorClicked,
//			sliderColorNormal,
//			sliderColorHover,
//			sliderColorClicked,
//			sliderContourColor,
//			contourColor,
//			backSliderColor,
//			backSliderContourColor;
//
//	axSliderInfo(const string& imgPath,
//				 const axSize& size,
//				 const axColor& bg_normal,
//				 const axColor& bg_hover,
//				 const axColor& bg_clicking,
//				 const axColor& slider_normal,
//				 const axColor& slider_hover,
//				 const axColor& slider_clicking,
//				 const axColor& slider_contour,
//				 const axColor& contour,
//				 const axColor& back_slider,
//				 const axColor& back_slider_contour,
//				 const unsigned int width) :
//				 // Members.
//				 img_path(imgPath),
//				 btn_size(size),
//				 bgColorNormal(bg_normal),
//				 bgColorHover(bg_hover),
//				 bgColorClicked(bg_clicking),
//				 sliderColorNormal(slider_normal),
//				 sliderColorHover(slider_hover),
//				 sliderColorClicked(slider_clicking),
//				 sliderContourColor(slider_contour),
//				 contourColor(contour),
//				 backSliderColor(back_slider),
//				 backSliderContourColor(back_slider_contour),
//				 slider_width(width){}
//
//	axSliderInfo(const string& info_path)
//	{
//		ifstream file;
//		file.open(info_path);
//
//		if (file.fail())
//		{
//			cerr << "Problem opening file " << info_path << endl;
//		}
//		else
//		{
//			string line;
//
//			if (file.good())
//			{
//				getline(file, line);
//				img_path = line;
//			}
//
//			if (file.good())
//			{
//				getline(file, line);
//				btn_size.x = stoi(line);
//			}
//
//			if (file.good())
//			{
//				getline(file, line);
//				btn_size.y = stoi(line);
//			}
//
//			if (file.good())
//			{
//				getline(file, line);
//				slider_width = stoi(line);
//			}
//			
//
//			axColor* ptr = &bgColorNormal;
//			while (file.good())
//			{
//				getline(file, line);
//				*ptr++ = axColor(line);
//			}
//		}
//	}
//};
//
///**************************************************************************//**
// * axSlider.
//******************************************************************************/
//class axSlider : public axPanel
//{
//public:
//	axSlider(axApp* app,
//			 axWindow* parent,
//			 const axRect& rect,
//			 const axSliderEvents& events,
//			 const axSliderInfo& info,
//			 //string img_path = "",
//			 //string label = "",
//			 axFlag flags = 0);
//
//protected:
//	//axSliderMembers _members;
//	//axSliderOrientationStrategy* _sliderOrientationStrategy;
//	axSliderEvents _events;
//	axSliderInfo _info;
//	axFlag _flag;
//	axColor _currentBgColor, 
//			_currentSliderColor;
//
//	axImage _btnImg;
//	axFlag _flags;
//	int _nCurrentImg,
//		_sliderPosition,
//		_btnYPos,
//		_sliderYPos,
//		_delta_click;
//
//	double _sliderValue;
//
//	enum axButtonState 
//	{
//		axBTN_NORMAL,
//		axBTN_HOVER,
//		axBTN_DOWN
//	};
//
//	void blockSliderPosition(const axPoint& pos);
//	void updateSliderValue();
//
//	void OnPaint();
//	void OnMouseEnter(const axPoint& pos);
//	void OnMouseLeftDown(const axPoint& pos);
//	void OnMouseLeftUp(const axPoint& pos);
//	void OnMouseMotion(const axPoint& pos);
//	void OnMouseLeftDragging(const axPoint& pos);
//	void OnMouseLeave(const axPoint& pos);
//	//void OnFocusIn();
//};

//#define axSLIDER_PLAIN_GREY     axSliderInfo( "sliderPlain.png",          \
//	axSize(12, 12), \
//	axColor(0.1, 0.1, 0.1), \
//	axColor("#888888"), \
//	axColor("#888888"), \
//	axColor("#3333FF"), \
//	axColor("#4444FF"), \
//	axColor("#5555FF"), \
//	axColor("#000000"), \
//	axColor(0.2, 0.2, 0.2), \
//	axColor("#444444"), \
//	axColor("#000000"), \
//	5)

#endif // __AX_POPUP_MENU__
