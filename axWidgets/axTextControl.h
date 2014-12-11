/// @defgroup Widgets 
/// @{

#ifndef __AX_TEXT_CONTROL__
#define __AX_TEXT_CONTROL__

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include "axMsg.h"

/**************************************************************************//**
 * axTextControlFlags.
******************************************************************************/
#define axBUTTON_SINGLE_IMG	axFLAG_1
#define axBUTTON_IMG_RESIZE	axFLAG_2
#define axBUTTON_CAN_SELECTED axFLAG_3 // Not implemented yet.

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
	axColor clicking;
	axColor cursor;
	axColor contour;
	axColor font_color;

	axTextControlInfo(){}
	axTextControlInfo(
		const axColor& normal_color,
		const axColor& hover_color,
		const axColor& clicked_color,
		const axColor& cursor_color,
		const axColor& contour_color,
		const axColor& font_color_) :
		normal(normal_color),
		hover(hover_color),
		clicking(clicked_color),
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
            clicking.SerializeOutput(file);
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
            clicking.SerializeInput(file);
            cursor.SerializeInput(file);
            contour.SerializeInput(file);
            font_color.SerializeInput(file);
        }
    }
};

//#define axSTANDARD_BUTTON 	axButtonInfo( \
//							axColor(0.5, 0.5, 0.5),\
//							axColor(0.6, 0.6, 0.6),\
//							axColor(0.4, 0.4, 0.4),\
//							axColor(0.5, 0.5, 0.5),\
//							axColor(0.0, 0.0, 0.0),\
//							axColor(0.0, 0.0, 0.0)) 
//
//#define axBUTTON_TRANSPARENT 	axButtonInfo( \
//axColor(0.0, 0.0, 0.0, 0.0),\
//axColor(0.0, 0.0, 0.0, 0.0),\
//axColor(0.0, 0.0, 0.0, 0.0),\
//axColor(0.0, 0.0, 0.0, 0.0),\
//axColor(0.0, 0.0, 0.0, 0.0),\
//axColor(0.0, 0.0, 0.0, 1.0))

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

    
protected:
    axTextControlEvents _events;
    axTextControlInfo _info;
    string _label;
    axImage* _btnImg;
    axFlag _flags;
        
    axColor* _currentColor;
    int _nCurrentImg;
    
    int _cursorBarXPosition;
    
    enum axTextControlState
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
    
    virtual void OnKeyDown(const char& key);
    virtual void OnBackSpaceDown();
};

#endif //__AX_BUTTON__

/// @}


