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
    
    axButton::Info btn1_info;
    btn1_info.normal = axColor(0.8, 0.8, 0.8);
    btn1_info.hover = axColor(0.9, 0.9, 0.9);
    btn1_info.clicking = axColor(0.7, 0.7, 0.7);
    btn1_info.contour = axColor(0.0, 0.0, 0.0);
    btn1_info.selected = btn1_info.normal;
    
    axButton::Events btn1_evts;
    btn1_evts.button_click = GetOnButtonWithEvtManager();
    
    axButton* btn1 = new axButton(this,
                                  axRect(40, 40, 60, 25),
                                  btn1_evts,
                                  btn1_info,
                                  "", "Btn1");
    
//    axButton* btn2 = new axButton(this,
//                                  axRect(110, 40, 60, 25),
//                                  axButton::Events(),
//                                  axBUTTON_TRANSPARENT,
//                                  app_path + std::string("GrayButton.png"),
//                                  "Btn2",
//                                  axButton::Flags::SINGLE_IMG);
//    
//    axButton* btn3 = new axButton(this,
//                                  axRect(180, 40, 60, 25),
//                                  axButton::Events(),
//                                  axBUTTON_TRANSPARENT,
//                                  app_path + std::string("btn2.png"),
//                                  "Btn3");
//    
//    std::string img_path = app_path + std::string("button.png");
//    axButton* btn4 = new axButton(this,
//                                  axRect(250, 40, 60, 25),
//                                  axButton::Events(),
//                                  btn1_info,
//                                  img_path,
//                                  "Btn4",
//								  axButton::Flags::SINGLE_IMG |
//								  axButton::Flags::IMG_RESIZE);
//    
//    axButton* btn5 = new axButton(this,
//                                  axRect(320, 40, 60, 25),
//                                  axButton::Events(),
//								  axButton::Info(app_path + std::string("axButtonBlue.axobj")),
//                                  img_path,
//                                  "Btn5",
//								  axButton::Flags::SINGLE_IMG |
//								  axButton::Flags::IMG_RESIZE);
    
    axButton* btn6 = new axButton(this,
                                  axRect(390, 40, 25, 25),
                                  axButton::Events(),
                                  axBUTTON_TRANSPARENT,
                                  app_path + std::string("playTest.png"),
                                  "",
								  axButton::Flags::IMG_RESIZE);
    
    _timer = new axTimer();
    _timer->AddConnection(0, GetOnTimerEvent());
    
    _colorTimer = axColor(1.0, 1.0, 1.0);
    
}

void ButtonPanel::OnButtonWithEvtManager(const axButton::Msg& msg)
{
    std::cout << "Button event." << std::endl;
    _timer->StartTimer(20, 800);
}

void ButtonPanel::OnTimerEvent(const axTimerMsg& msg)
{
//    std::cout << "Timer : " << msg.GetTime() << std::endl;
    double timer_ratio = msg.GetTime() / double(800 - 20) * 0.4;
    _colorTimer = axColor(timer_ratio, timer_ratio, 0.4);
    Update();
}

void ButtonPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(_colorTimer, 1.0);
    gc->DrawRectangle(rect0);

   
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
}