//
//  axToolBar.h
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-16.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __MidiSequencer__axToolBar__
#define __MidiSequencer__axToolBar__

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"

class axToolBarMsg
{
public:
    axToolBarMsg(const double& value):
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


struct axToolBarEvents
{
    axEvtFunction(axToolBarMsg) value_change;
    
    axToolBarEvents(){}
    axToolBarEvents(axEvtFunction(axToolBarMsg)& fct){ value_change = fct; }
};


struct axToolBarInfo
{
    axColor bgColorNormal,
            bgColorHover,
            contourColor;

    axToolBarInfo(const axColor& bg_normalColor,
                  const axColor& bg_hoverColor,
                  const axColor& contour_color):
    bgColorNormal(bg_normalColor),
    bgColorHover(bg_hoverColor),
    contourColor(contour_color)
    {
        
    }
};


class axToolBar: public axPanel
{
public:
    axToolBar(axApp* app,
              axWindow* parent,
              const axRect& rect,
              const axToolBarEvents& events,
              const axToolBarInfo& info,
              axFlag flags = 0);
    
    void SetBackgroundAlpha(const double& alpha)
    {
        _bgAlpha = alpha;
        Update();
    }
    
private:
    axToolBarEvents _events;
    axToolBarInfo _info;
    axFlag _flags;
    
    double _bgAlpha;
    axColor* _currentColor;
    
    virtual void OnPaint();
    virtual void OnMouseEnter();
    virtual void OnMouseLeave();
};

#endif /* defined(__MidiSequencer__axToolBar__) */
