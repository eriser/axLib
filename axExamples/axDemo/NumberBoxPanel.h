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
    
    axEVENT_ACCESSOR(axNumberBox::Msg, OnNumberBoxRed);
    axEVENT_ACCESSOR(axNumberBox::Msg, OnNumberBoxGreen);
    axEVENT_ACCESSOR(axNumberBox::Msg, OnNumberBoxBlue);
    axEVENT_ACCESSOR(axNumberBox::Msg, OnNumberBoxAlpha);
    
    axEVENT_ACCESSOR(axNumberBox::Msg, OnNumberBoxRedBottom);
    axEVENT_ACCESSOR(axNumberBox::Msg, OnNumberBoxGreenBottom);
    axEVENT_ACCESSOR(axNumberBox::Msg, OnNumberBoxBlueBottom);
    axEVENT_ACCESSOR(axNumberBox::Msg, OnNumberBoxAlphaBottom);
    
private:
    virtual void OnPaint();
    
    void OnNumberBoxRed(const axNumberBox::Msg& msg);
    void OnNumberBoxGreen(const axNumberBox::Msg& msg);
    void OnNumberBoxBlue(const axNumberBox::Msg& msg);
    void OnNumberBoxAlpha(const axNumberBox::Msg& msg);
    
    void OnNumberBoxRedBottom(const axNumberBox::Msg& msg);
    void OnNumberBoxGreenBottom(const axNumberBox::Msg& msg);
    void OnNumberBoxBlueBottom(const axNumberBox::Msg& msg);
    void OnNumberBoxAlphaBottom(const axNumberBox::Msg& msg);
    
    axColor _squareColor, _squareContourColor, _squareColorBottom;
    axRect _squareRect;
};

#endif /* defined(__Demo__NumberBoxPanel__) */
