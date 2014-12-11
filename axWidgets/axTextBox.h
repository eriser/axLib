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
#include "axTimer.h"

/**************************************************************************//**
 * axTextBoxFlags.
******************************************************************************/
#define axTEXT_BOX_FLASHING_CURSOR	axFLAG_1

class axTextBox;

class axTextBoxMsg : public axMsg
{
public:
    axTextBoxMsg()
    {
        _sender = nullptr;
    }
    
    axTextBoxMsg(axTextBox* sender, const string& msg)
    {
        _sender = sender;
        _msg = msg;
    }
    
    axTextBox* GetSender() const
    {
        return _sender;
    }
    
    string GetMsg() const
    {
        return _msg;
    }
    
    axMsg* GetCopy()
    {
        return new axTextBoxMsg(*this);
    }
    
private:
    axTextBox* _sender;
    string _msg;
};

struct axTextBoxEvents
{
    enum : axEventId { BUTTON_CLICK };
    
	axTextBoxEvents(){}
    axTextBoxEvents(axEventFunction& fct){ button_click = fct; }
    
    axEventFunction button_click;
};

struct axTextBoxInfo
{
	axColor normal;
	axColor hover;
	axColor clicking;
	axColor cursor;
	axColor contour;
	axColor font_color;

	axTextBoxInfo(){}
	axTextBoxInfo(
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
    
    axTextBoxInfo(const std::string& path)
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

class axTextBox : public axPanel
{
public:
	axTextBox(axWindow* parent,
                  const axRect& rect,
                  const axTextBoxEvents& events,
                  const axTextBoxInfo& info,
                  string img_path = "",
                  string label = "",
                  axFlag flags = 0);

    void SetLabel(const std::string& label);

    
    axEVENT_ACCESSOR(axTimerMsg, OnFlashingCursorTimer);
    
protected:
    axTextBoxEvents _events;
    axTextBoxInfo _info;
    string _label;
    axImage* _btnImg;
    axFlag _flags;
        
    axColor* _currentColor;
    int _nCurrentImg;
    
    int _cursorBarXPosition;
    
    enum axTextBoxState
    {
        axBTN_NORMAL,
        axBTN_HOVER,
        axBTN_DOWN,
        axBTN_SELECTED
    };
    
    axTimer* _flashingCursor;
    
	virtual void OnPaint();
	virtual void OnMouseLeftDown(const axPoint& pos);
	virtual void OnMouseLeftUp(const axPoint& pos);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
    
    virtual void OnKeyDown(const char& key);
    virtual void OnBackSpaceDown();
    
    virtual void OnLeftArrowDown();
    virtual void OnRightArrowDown();
    virtual void OnWasKeyUnGrabbed();
    virtual void OnWasKeyGrabbed();
    
    void OnFlashingCursorTimer(const axTimerMsg& msg);
    
    bool _cursorFlashActive;
};

#endif //__AX_BUTTON__

/// @}


