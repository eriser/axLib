/// @defgroup Widgets 
/// @{

#ifndef __AX_GRID__
#define __AX_GRID__

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"

#include <fstream>

typedef axTemplate2DPoint<int> axArrayIndex;

class axGrid;

class axGridMsg
{
public:
	axGridMsg()
	{
		_sender = nullptr;
	}

	axGridMsg(axGridMsg* sender, const string& msg)
	{
		_sender = sender;
		_msg = msg;
	}

	axGridMsg* GetSender() const
	{
		return _sender;
	}

	string GetMsg() const
	{
		return _msg;
	}

private:
	axGridMsg* _sender;
	string _msg;
};

struct axGridEvents
{
	std::function<void (axGridMsg)> move_element;
	std::function<void (axGridMsg)> new_element;
	
	axGridEvents(){}
	axGridEvents(std::function<void (axGridMsg)>& move, 
				   std::function<void (axGridMsg)>& new_elem)
	{ 
		move_element = move;
		new_elem = new_elem;
	}
};

struct axGridInfo
{
	axColor normal;
	axColor hover;
	axColor clicking;
	axColor selected;
	axColor contour;
	axColor font_color;

	axGridInfo(){}
	axGridInfo(
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

	axGridInfo(const string& info_path)
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

class axGrid : public axPanel
{
public:
	axGrid(axWindow* parent,
		const axRect& rect,
		const axGridEvents& events,
		const axGridInfo& info,
		axSize grid_dimension,
		//string label = "",
		axFlag flags = 0,
		string msg = "");

	void SetBackgroundAlpha(const float& alpha);
	void SetElementColor(const axColor& color);
	void SetMsg(const string& msg);
	void SetSelected(const bool& selected);

private:
	axGridEvents _events;
	axGridInfo _info;
	//string _label;
	string _msg;
	
	axFlag _flags;
	axSize _dimension;

	int _nCurrentImg;

	int nGridElement;

	struct axGridElement
	{
		bool on;
		axPoint position;
		axColor color;
	};

	axColor _currentElementColor;
	vector<vector<axGridElement>> _gridElements;
	
	void AddElement(const int& row, const int& col);
	axArrayIndex GetElementIndexFromMouse(const axPoint& pos);
	axPoint GetPositionOfElement(const axArrayIndex& index);

	axArrayIndex _selectedElement;

	virtual void OnPaint();
	//virtual void OnMouseMotion(){}
	virtual void OnMouseLeftDown(const axPoint& pos);
	virtual void OnMouseLeftUp(const axPoint& pos);
	// virtual void OnMouseRightDown();
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
};

#endif //__AX_GRID__

/// @}