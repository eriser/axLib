//
//  KnobPanel.cpp
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-03.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "KnobPanel.h"

KnobPanel::KnobPanel(axApp* app,
                         axWindow* parent,
                         const axRect& rect):
// Parent.
axPanel(app, parent, rect)
{
    std::string app_path("/Users/alexarse/Project/axLib/axExamples/Demo/");
    axKnobInfo knob_info(axColor(0.3, 0.3, 0.3),
                         axColor("#AAAAAA"),
                         axColor("#BBBBBB"),
                         128,
                         axSize(46, 46),
                         app_path + std::string("knob_dark.png"),
                         app_path + std::string("knob_dark.png"));
    
    axKnob* knob = new axKnob(app, this, axRect(40, 40, 46, 46),
                              axKnobEvents(), knob_info);
    
    knob->SetBackgroundAlpha(0.0);
    knob->SetValue(1.0);
}

void KnobPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.4, 0.4, 0.4), 1.0);
    gc->DrawRectangle(rect0);
    
    //    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    //    gc->SetFontSize(12);
    ////    gc->DrawStringAlignedCenter("test", rect0);
    //    gc->DrawString("regular", axPoint(350, 80));
    //    gc->DrawString("with back slider", axPoint(350, 100));
    //    gc->DrawString("no slider", axPoint(350, 120));
    //    gc->DrawString("click anywhere", axPoint(350, 140));
    //    gc->DrawString("right align", axPoint(350, 160));
    //    gc->DrawString("no button", axPoint(350, 180));
    //    gc->DrawString("center align", axPoint(350, 200));
    //
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
}