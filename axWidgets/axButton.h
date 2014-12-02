/// @defgroup Widgets 
/// @{

#ifndef __AX_BUTTON__
#define __AX_BUTTON__

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include <fstream>

// enum axType
// {
// 	axCOLOR,
// 	axINT,
// 	axFLOAT
// };

//typedef unsigned int axType;

// class axInfo
// {
// public:
// 	axInfo()
// 	{
// 	}

// 	bool Load()
// 	{
// 		Fill();
// 		return true;
// 	}

// 	bool Load(const string& info_path)
// 	{
// 		Fill();
// 		cout << "Size : " << _colors.size() << endl;
// 		return true;
// 	}

// 	virtual void Fill() = 0;

// 	axColor& operator [](pair<axColor, std::string>& col)
// 	{
// 		return _colors[col.second];
// 	}

// private:
// 	map<string, axColor> _colors;
// };

// #define axAddInfo(type, label) (*this)[pair<type, string>(type(), label)]

// class axInfoTest : public axInfo
// {
// public:
// 	axInfoTest()
// 	{
// 		Load();
// 	}

// 	axInfoTest(const string& path)
// 	{	
// 		Load(path);
// 	}

// 	virtual void Fill()
// 	{
// 		axAddInfo(axColor, "normal");
// 		axAddInfo(axColor, "hover");
// 		axAddInfo(axColor, "clicking");
// 		axAddInfo(axColor, "selected");
// 		axAddInfo(axColor, "contour");
// 		axAddInfo(axColor, "font");
// 	}
// };

/**************************************************************************//**
 * axButtonFlags.
******************************************************************************/
#define axBUTTON_SINGLE_IMG	axFLAG_1
#define axBUTTON_IMG_RESIZE	axFLAG_2
#define axBUTTON_CANT_BE_SELECTED axFLAG_3 // Not implemented yet.

class axButton;

class axButtonMsg
{
public:
	axButtonMsg()
	{
		_sender = nullptr;
	}

	axButtonMsg(axButton* sender, const string& msg)
	{
		_sender = sender;
		_msg = msg;
	}

	axButton* GetSender() const
	{
		return _sender;
	}

	string GetMsg() const
	{
		return _msg;
	}

private:
	axButton* _sender;
	string _msg;
};

struct axButtonEvents
{
//	axEvtFunction(axButtonMsg) button_click;
	std::function<void (axButtonMsg)> button_click;
	
	axButtonEvents(){}
	axButtonEvents(std::function<void (axButtonMsg)>& fct){ button_click = fct; }
};

struct axButtonInfo
{
	axColor normal;
	axColor hover;
	axColor clicking;
	axColor selected;
	axColor contour;
	axColor font_color;

	axButtonInfo(){}
	axButtonInfo(
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

	axButtonInfo(const string& info_path)
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

#define axSTANDARD_BUTTON 	axButtonInfo( \
							axColor(0.5, 0.5, 0.5),\
							axColor(0.6, 0.6, 0.6),\
							axColor(0.4, 0.4, 0.4),\
							axColor(0.5, 0.5, 0.5),\
							axColor(0.0, 0.0, 0.0),\
							axColor(0.0, 0.0, 0.0)) 


class axButton : public axPanel
{
public:
	axButton(axWindow* parent,
		const axRect& rect,
		const axButtonEvents& events,
		const axButtonInfo& info,
		string img_path = "",
		string label = "",
		axFlag flags = 0,
		string msg = "");

	axButton(axWindow* parent,
			 const axButtonEvents& events,
			 const string& path);

    // Should be there since update axColor with alpha component.
	void SetBackgroundAlpha(const float& alpha);

	void SetMsg(const string& msg);
    
	void SetSelected(const bool& selected);
    
    void SetLabel(const std::string& label);

private:
	axButtonEvents _events;
	axButtonInfo _info;
	axColor* _currentColor;
	axImage* _btnImg;
	string _label;
	string _msg;
	
	bool _selected;
	axFlag _flags;
	axColor test;
	axFloat _bgAlpha;

	enum axButtonState 
	{
		axBTN_NORMAL,
		axBTN_DOWN,
		axBTN_HOVER,
		axBTN_SELECTED
	};

	int _nCurrentImg;

	axEvent OnPaint();
	axEvent OnMouseLeftDown(const axPoint& pos);
	axEvent OnMouseLeftUp(const axPoint& pos);
	axEvent OnMouseEnter();
	axEvent OnMouseLeave();
};

#endif //__AX_BUTTON__

/// @}
