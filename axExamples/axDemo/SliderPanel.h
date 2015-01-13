//
//  SliderPanel.h
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-03.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __Demo__SliderPanel__
#define __Demo__SliderPanel__

#include "axLib.h"

class MyScrollPanel : public axPanel
{
public:
    MyScrollPanel(axWindow* parent,
                const axRect& rect,
                  const axSize& seenSize);
    
    axEVENT_ACCESSOR(axButton::Msg, OnBtn);
    
private:
    virtual void OnPaint();
    virtual void OnPaintStatic();
    
    void OnBtn(const axButton::Msg& msg);
};

class SliderPanel : public axPanel
{
public:
    SliderPanel(axWindow* parent,
                const axRect& rect);
    
    axEVENT_ACCESSOR(axSliderMsg, OnSlider1);
    
private:
    virtual void OnPaint();
    
    void OnSlider1(const axSliderMsg& msg);
    
    MyScrollPanel* _scrollPanel;
};

#endif /* defined(__Demo__SliderPanel__) */
