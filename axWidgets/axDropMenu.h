/// @defgroup Widgets
/// @{

#ifndef __AX_DROP_MENU__
#define __AX_DROP_MENU__

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"
#include "axMsg.h"
#include "axPopupMenu.h"

#define axDROP_MENU_SINGLE_IMG	axFLAG_1

class axDropMenu;

class axDropMenuMsg : public axMsg
{
public:
    axDropMenuMsg()
    {
        _sender = nullptr;
    }
    
    axDropMenuMsg(axDropMenu* sender, const string& msg)
    {
        _sender = sender;
        _msg = msg;
    }
    
    axDropMenu* GetSender() const
    {
        return _sender;
    }
    
    string GetMsg() const
    {
        return _msg;
    }
    
    axMsg* GetCopy()
    {
        return new axDropMenuMsg(*this);
    }
    
private:
    axDropMenu* _sender;
    string _msg;
};

struct axDropMenuEvents
{
    enum : axEventId { SELECTION_CHANGE };
    
    axDropMenuEvents(){}
    axDropMenuEvents(axEventFunction& fct){ selection_change = fct; }
    
    axEventFunction selection_change;
};

struct axDropMenuInfo
{
    axColor normal;
    axColor hover;
    axColor clicking;
    axColor selected;
    axColor contour;
    axColor font_color;
    
    axDropMenuInfo(){}
    axDropMenuInfo(
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

class axDropMenu : public axPanel
{
public:
    axDropMenu(axWindow* parent,
               const axRect& rect,
               const axDropMenuEvents& events,
               const axDropMenuInfo& info,
               string img_path = "",
               string bg_img_path = "",
               axFlag flags = 0,
               string msg = "");

    void SetMsg(const string& msg);
    
    void SetSelected(const bool& selected);
        
    axEVENT_ACCESSOR(axPopupMenuMsg, OnPopupMenuChoice);
    
private:
    axDropMenuEvents _events;
    axDropMenuInfo _info;
    
    axColor* _currentColor;
    axImage* _btnImg;
    axImage* _bgImg;
    string _label;
    string _msg;
    
    bool _selected;
    axFlag _flags;
    axColor test;
//    axFloat _bgAlpha;
    
    axPopupMenu* _popMenu;
    
    enum axDropMenuState
    {
        axDROP_NORMAL,
        axDROP_DOWN,
        axDROP_HOVER,
        axDROP_SELECTED
    };
    
    int _nCurrentImg;
    
    virtual void OnPaint();
    virtual void OnMouseLeftDown(const axPoint& pos);
    virtual void OnMouseLeftUp(const axPoint& pos);
    virtual void OnMouseEnter();
    virtual void OnMouseLeave();
    
    void OnPopupMenuChoice(const axPopupMenuMsg& msg);
};

#endif //__AX_DROP_MENU__

/// @}
