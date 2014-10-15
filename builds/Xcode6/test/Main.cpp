//
//  Main.cpp
//  ax
//
//  Created by Alexandre Arsenault on 2014-10-14.
//  Copyright (c) 2014 alexarse. All rights reserved.
//

#include "Main.h"

Desktop::Desktop(axApp* app, axWindow* parent, const axRect& rect):
axPanel(app, parent, rect)
{
    axButtonInfo btn_info(axColor(0.8, 0.8, 0.8),
                          axColor(0.9, 0.9, 0.9),
                          axColor(1.0, 1.0, 1.0),
                          axColor(0.8, 0.8, 0.8),
                          axColor(0.0, 0.0, 0.0),
                          axColor(0.8, 0.8, 0.8));
    
    axEvtFunction(axButtonMsg) evt(GetOnBtn());
    axButton* btn = new axButton(app, this, axRect(30, 30, 48, 48),
                                 axButtonEvents(evt),
                                 btn_info,"/Users/alexarse/Project/axLib/ressources/axImages/calc.png", "");
    
    axEvtFunction(axButtonMsg) evt2(GetOnBtn2());
    axButton* btn2 = new axButton(app, this, axRect(100, 30, 48, 48),
                                  axButtonEvents(evt2),
                                  btn_info);
    
    axSlider* slider = new axSlider(app, this, axRect(30, 80, 70, 20),
                                    axSliderEvents(),
                                    axSLIDER_PLAIN_GREY);
    
    slider->SetBackgroundAlpha(0.0);
    
}

void Desktop::OnBtn(const axButtonMsg& msg)
{
    cout << "Btn 1 msg." << endl;
}


void Desktop::OnBtn2(const axButtonMsg& msg)
{
    cout << "Btn 2 msg." << endl;
}

void Desktop::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.0, 1.0, 0.0), 1.0);
    gc->DrawRectangle(rect0);
}


