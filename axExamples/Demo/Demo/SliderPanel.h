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

class SliderPanel : public axPanel
{
public:
    SliderPanel(axApp* app,
                axWindow* parent,
                const axRect& rect);
    
    axEVENT(axSliderMsg, OnSlider1);
private:
    virtual void OnPaint();
    
    void OnSlider1(const axSliderMsg& msg);
};

#endif /* defined(__Demo__SliderPanel__) */
