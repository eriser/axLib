#ifndef __AX_SLIDER__
#define __AX_SLIDER__

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"

/**************************************************************************//**
 * axSliderFlags.
******************************************************************************/
#define axSLIDER_FLAG_VERTICAL			axFLAG_1
#define axSLIDER_FLAG_CLICK_ANYWHERE    axFLAG_2
#define axSLIDER_FLAG_RELEASE_ON_LEAVE  axFLAG_3
#define axSLIDER_FLAG_LEFT_CLICK_ENTER  axFLAG_4
#define axSLIDER_FLAG_RIGHT_ALIGN       axFLAG_5
#define axSLIDER_FLAG_MIDDLE_ALIGN      axFLAG_6
#define axSLIDER_FLAG_BACK_SLIDER       axFLAG_7
#define axSLIDER_FLAG_NO_SLIDER_LINE    axFLAG_8

/**************************************************************************//**
 * axSliderMsg
******************************************************************************/
class axSliderMsg
{
public:
	axSliderMsg(const double& value):
		_value(value)
	{
	}

	double GetValue() const
	{
		return _value;
	}

private:
	double _value;
};

/**************************************************************************//**
 * axSliderEvents.
******************************************************************************/
struct axSliderEvents
{
	axEvtFunction(axSliderMsg) slider_value_change;

	axSliderEvents(){}
	axSliderEvents(axEvtFunction(axSliderMsg)& fct){ slider_value_change = fct; }
};

/**************************************************************************//**
 * axSliderInfo.
******************************************************************************/
struct axSliderInfo
{
	string img_path;
	axSize btn_size;
	unsigned int slider_width;

	axColor bgColorNormal,
			bgColorHover,
			bgColorClicked,
			sliderColorNormal,
			sliderColorHover,
			sliderColorClicked,
			sliderContourColor,
			contourColor,
			backSliderColor,
			backSliderContourColor;

    axSliderInfo(){}
	axSliderInfo(const string& imgPath,
				 const axSize& size,
				 const axColor& bg_normal,
				 const axColor& bg_hover,
				 const axColor& bg_clicking,
				 const axColor& slider_normal,
				 const axColor& slider_hover,
				 const axColor& slider_clicking,
				 const axColor& slider_contour,
				 const axColor& contour,
				 const axColor& back_slider,
				 const axColor& back_slider_contour,
				 const unsigned int width) :
				 // Members.
				 img_path(imgPath),
				 btn_size(size),
				 bgColorNormal(bg_normal),
				 bgColorHover(bg_hover),
				 bgColorClicked(bg_clicking),
				 sliderColorNormal(slider_normal),
				 sliderColorHover(slider_hover),
				 sliderColorClicked(slider_clicking),
				 sliderContourColor(slider_contour),
				 contourColor(contour),
				 backSliderColor(back_slider),
				 backSliderContourColor(back_slider_contour),
				 slider_width(width){}

	axSliderInfo(const string& info_path)
	{
	// 	ifstream file;
	// 	file.open(info_path);

	// 	if (file.fail())
	// 	{
	// 		cerr << "Problem opening file " << info_path << endl;
	// 	}
	// 	else
	// 	{
	// 		string line;

	// 		if (file.good())
	// 		{
	// 			getline(file, line);
	// 			img_path = line;
	// 		}

	// 		if (file.good())
	// 		{
	// 			getline(file, line);
	// 			btn_size.x = stoi(line);
	// 		}

	// 		if (file.good())
	// 		{
	// 			getline(file, line);
	// 			btn_size.y = stoi(line);
	// 		}

	// 		if (file.good())
	// 		{
	// 			getline(file, line);
	// 			slider_width = stoi(line);
	// 		}
			

	// 		axColor* ptr = &bgColorNormal;
	// 		while (file.good())
	// 		{
	// 			getline(file, line);
	// 			*ptr++ = axColor(line);
	// 		}
	// 	}
	}
};

/**************************************************************************//**
 * axSlider.
******************************************************************************/
class axSlider : public axPanel
{
public:
	axSlider(axWindow* parent,
			 const axRect& rect,
			 const axSliderEvents& events,
			 const axSliderInfo& info,
			 axFlag flags = 0);

	void SetBackgroundAlpha(const float& alpha)
	{
		_bg_alpha = alpha;
	}

	void ResizeSlider(const axSize& size)
	{
		SetSize(size);
		_sliderYPos = (GetSize().x - _info.slider_width) * 0.5;
		_btnYPos = (GetSize().x - _info.btn_size.x) * 0.5;
	}

	double GetValue() const
	{
		return _sliderValue;
	}

	void SetValue(const double& value);
	
protected:
	//axSliderMembers _members;
	//axSliderOrientationStrategy* _sliderOrientationStrategy;
	axSliderEvents _events;
	axSliderInfo _info;
	axFlag _flag;
	axColor _currentBgColor, 
			_currentSliderColor;



	axImage _btnImg;
	axFlag _flags;
	int _nCurrentImg,
		_sliderPosition,
		_btnYPos,
		_sliderYPos,
		_delta_click;

		float _bg_alpha;

	double _sliderValue;

	enum axButtonState 
	{
		axBTN_NORMAL,
		axBTN_HOVER,
		axBTN_DOWN
	};

	void blockSliderPosition(const axPoint& pos);
	void updateSliderValue();

	void DrawVerticalSlider(axGC* gc, const axRect& rect0);
	void DrawLineBehindSlider_Vertical(axGC* gc, const axRect& rect0);



	void OnPaint();
	void OnMouseEnter(const axPoint& pos);
	void OnMouseLeftDown(const axPoint& pos);
	void OnMouseLeftUp(const axPoint& pos);
	void OnMouseMotion(const axPoint& pos);
	void OnMouseLeftDragging(const axPoint& pos);
	void OnMouseLeave(const axPoint& pos);
	//void OnFocusIn();
};

/**************************************************************************//**
 * axSliderOrientation.
******************************************************************************/
//class axSliderOrientationStrategy : public axSlider
//{
//public:
//	axSliderOrientationStrategy(bool vertical)
//	{
//
//	}
//};
//
//
//class axSliderVertical : public axSliderOrientation
//{
//public:
//	axSliderVertical()
//	{
//
//	}
//}

//#define axSLIDER_STANDARD_BLUE axSliderInfo( "ressources/images/bouttons/sliderCtrl.png",   \
//											 axSize(12, 12),								\
//											 axColor("#444444"),							\
//											 axColor("#444444"),							\
//											 axColor("#444444"),							\
//											 axColor("#3333FF"),							\
//											 axColor("#4444FF"),							\
//											 axColor("#5555FF"),							\
//											 axColor("#000000"),							\
//											 axColor("#000000"),							\
//											 axColor("#000000"),							\
//											 axColor("#000000"),							\
//											 5)
//
//#define axSLIDER_STANDARD_BLUE_NO_IMG axSliderInfo( "",										\
//													axSize(0, 0),							\
//													axColor("#AAAAAA"),						\
//													axColor("#BBBBBB"),						\
//													axColor("#CCCCCC"),						\
//													axColor("#3333FF"),						\
//													axColor("#4444FF"),						\
//													axColor("#5555FF"),						\
//													axColor("#000000"),						\
//													axColor("#000000"),						\
//													axColor("#000000"),						\
//													axColor("#000000"),						\
//													5)
//

#define axSLIDER_PLAIN_GREY     axSliderInfo( "/Users/alexarse/Project/axLib/ressources/sliderPlain.png",          \
	axSize(12, 12), \
	axColor(0.1, 0.1, 0.1), \
	axColor("#888888"), \
	axColor("#888888"), \
	axColor("#3333FF"), \
	axColor("#4444FF"), \
	axColor("#5555FF"), \
	axColor("#000000"), \
	axColor(0.2, 0.2, 0.2), \
	axColor("#444444"), \
	axColor("#000000"), \
	5)

#endif // __AX_SLIDER__
