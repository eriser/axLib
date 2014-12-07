//
//  ButtonPanel.cpp
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-02.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "ButtonPanel.h"

ButtonPanel::ButtonPanel(axWindow* parent,
                         const axRect& rect):
// Parent.
axPanel(parent, rect)
{
    std::string app_path(axApp::GetInstance()->GetAppDirectory());
	//std::string app_path("C:/Users/Alexandre Arsenault/Desktop/axLib/axExamples/Demo/VisualStudio2013/axGL/");
    
    axButtonInfo btn1_info;
    btn1_info.normal = axColor(0.8, 0.8, 0.8);
    btn1_info.hover = axColor(0.9, 0.9, 0.9);
    btn1_info.clicking = axColor(0.7, 0.7, 0.7);
    btn1_info.contour = axColor(0.0, 0.0, 0.0);
    btn1_info.selected = btn1_info.normal;
    
    axButtonEvents btn1_evts;
    
    axButton* btn1 = new axButton(this,
                                  axRect(40, 40, 60, 25),
                                  btn1_evts,
                                  btn1_info,
                                  "", "Btn1");
    
    btn1->AddConnection(axButtonEvents::BUTTON_CLICK,
                        GetOnButtonWithEvtManager());
    
    
    axButton* btn2 = new axButton(this,
                                  axRect(110, 40, 60, 25),
                                  btn1_evts,
                                  btn1_info,
                                  app_path + std::string("btn.png"),
                                  "Btn2");
    btn2->SetBackgroundAlpha(0.0);
    
    axButton* btn3 = new axButton(this,
                                  axRect(180, 40, 60, 25),
                                  btn1_evts,
                                  btn1_info,
                                  app_path + std::string("btn2.png"),
                                  "Btn3");
    btn3->SetBackgroundAlpha(0.0);
    
    std::string img_path = app_path + std::string("button.png");
    axButton* btn4 = new axButton(this,
                                  axRect(250, 40, 60, 25),
                                  btn1_evts,
                                  btn1_info,
                                  img_path,
                                  "Btn4",
                                  axBUTTON_SINGLE_IMG |
                                  axBUTTON_IMG_RESIZE);
    
    axButtonInfo btn2_info;
    btn2_info.normal = axColor(0.2, 0.8, 0.8);
    btn2_info.hover = axColor(0.3, 0.9, 0.9);
    btn2_info.clicking = axColor(0.1, 0.7, 0.7);
    btn2_info.contour = axColor(0.0, 0.0, 0.0);
    btn2_info.selected = btn2_info.normal;

    axButton* btn5 = new axButton(this,
                                  axRect(320, 40, 60, 25),
                                  btn1_evts,
                                  btn2_info,
                                  img_path,
                                  "Btn5",
                                  axBUTTON_SINGLE_IMG |
                                  axBUTTON_IMG_RESIZE);
    
    axButton* btn6 = new axButton(this,
                                  axRect(390, 40, 25, 25),
                                  btn1_evts,
                                  btn1_info,
                                  app_path + std::string("playTest.png"),
                                  "",
                                  axBUTTON_IMG_RESIZE);
    
    btn6->SetBackgroundAlpha(0.0);
    
    axTimer* timer1 = new axTimer(200);
    timer1->AddConnection(0, GetOnTimerEvent());
    
}

void ButtonPanel::OnButtonWithEvtManager(const axButtonMsg& msg)
{
    std::cout << "Button event." << std::endl;
}

void ButtonPanel::OnTimerEvent(const axTimerMsg& msg)
{
    std::cout << "Timer : " << msg.GetTime() << std::endl;
}

void ButtonPanel::OnPaint()
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