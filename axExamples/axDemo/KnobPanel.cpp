//
//  KnobPanel.cpp
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-03.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "KnobPanel.h"

KnobPanel::KnobPanel(axWindow* parent,
                         const axRect& rect):
// Parent.
axPanel(parent, rect)
{
    std::string app_path(axApp::GetInstance()->GetAppDirectory());
	//std::string app_path("C:/Users/Alexandre Arsenault/Desktop/axLib/axExamples/Demo/VisualStudio2013/axGL/");

    axKnob::Info knob_info(axColor(0.3, 0.3, 0.3),
                         axColor("#AAAAAA"),
                         axColor("#BBBBBB"),
                         128,
                         axSize(46, 46),
                         app_path + std::string("knob_dark.png"),
                         app_path + std::string("knob_dark.png"));
    
    axKnob* knob = new axKnob(this, axRect(40, 40, 46, 46),
                              axKnob::Events(), knob_info);
    
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
 
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
}