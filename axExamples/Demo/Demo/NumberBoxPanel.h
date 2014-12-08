//
//  NumberBoxPanel.h
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-03.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __Demo__NumberBoxPanel__
#define __Demo__NumberBoxPanel__

#include "axLib.h"

class NumberBoxPanel : public axPanel
{
public:
    NumberBoxPanel(
              axWindow* parent,
              const axRect& rect);
    
    axEVENT_ACCESSOR(axNumberBoxMsg, OnNumberBoxRed);
    axEVENT_ACCESSOR(axNumberBoxMsg, OnNumberBoxGreen);
    axEVENT_ACCESSOR(axNumberBoxMsg, OnNumberBoxBlue);
    axEVENT_ACCESSOR(axNumberBoxMsg, OnNumberBoxAlpha);
    
    axEVENT_ACCESSOR(axNumberBoxMsg, OnNumberBoxRedBottom);
    axEVENT_ACCESSOR(axNumberBoxMsg, OnNumberBoxGreenBottom);
    axEVENT_ACCESSOR(axNumberBoxMsg, OnNumberBoxBlueBottom);
    axEVENT_ACCESSOR(axNumberBoxMsg, OnNumberBoxAlphaBottom);
    
private:
    virtual void OnPaint();
    
    void OnNumberBoxRed(const axNumberBoxMsg& msg);
    void OnNumberBoxGreen(const axNumberBoxMsg& msg);
    void OnNumberBoxBlue(const axNumberBoxMsg& msg);
    void OnNumberBoxAlpha(const axNumberBoxMsg& msg);
    
    void OnNumberBoxRedBottom(const axNumberBoxMsg& msg);
    void OnNumberBoxGreenBottom(const axNumberBoxMsg& msg);
    void OnNumberBoxBlueBottom(const axNumberBoxMsg& msg);
    void OnNumberBoxAlphaBottom(const axNumberBoxMsg& msg);
    
    axColor _squareColor, _squareContourColor, _squareColorBottom;
    axRect _squareRect;
};

#endif /* defined(__Demo__NumberBoxPanel__) */
