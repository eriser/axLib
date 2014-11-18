//
//  axVolumeMeter.h
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-17.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __MidiSequencer__axVolumeMeter__
#define __MidiSequencer__axVolumeMeter__

#include "axEvent.h"
#include "axPanel.h"
#include "axColor.h"
#include "axGC.h"
#include "axImage.h"

struct axVolumeMeterInfo
{
    axColor bgColorNormal,
    bgColorHover,
    contourColor;
    
    axVolumeMeterInfo(const axColor& bg_normalColor,
                      const axColor& bg_hoverColor,
                      const axColor& contour_color):
    bgColorNormal(bg_normalColor),
    bgColorHover(bg_hoverColor),
    contourColor(contour_color)
    {
        
    }
};


class axVolumeMeter: public axPanel
{
public:
    axVolumeMeter(axApp* app,
              axWindow* parent,
              const axRect& rect,
              const axVolumeMeterInfo& info,
              axFlag flags = 0);
    
    void SetBackgroundAlpha(const double& alpha)
    {
        _bgAlpha = alpha;
        Update();
    }
    
    void SetValue(const double& value);
    
private:
//    axToolBarEvents _events;
    axVolumeMeterInfo _info;
    axFlag _flags;
    
    double _bgAlpha;
    axColor* _currentColor;
    
    double _value;
    
    virtual void OnPaint();
//    virtual void OnMouseEnter();
//    virtual void OnMouseLeave();
};
#endif /* defined(__MidiSequencer__axVolumeMeter__) */
