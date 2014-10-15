//
//  Main.h
//  ax
//
//  Created by Alexandre Arsenault on 2014-10-14.
//  Copyright (c) 2014 alexarse. All rights reserved.
//

#ifndef __ax__Main__
#define __ax__Main__

#include "axLib.h"


class Desktop : public axPanel
{
public:
    Desktop(axApp* app, axWindow* parent, const axRect& rect);
    
    axEVENT(axButtonMsg, OnBtn);
    axEVENT(axButtonMsg, OnBtn2);
    
private:
    void OnBtn(const axButtonMsg& msg);
    void OnBtn2(const axButtonMsg& msg);
    void OnPaint();
};


class axMain
{
public:
    axMain()
    {
    }
    
    static void MainEntryPoint(axApp* app)
    {
        Desktop* desktop = new Desktop(axApp::MainInstance,
                                       nullptr, axRect(0, 0, 200, 200));
    }
};

#endif /* defined(__ax__Main__) */
