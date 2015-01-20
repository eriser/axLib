//
//  NumberBoxPanel.cpp
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-03.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "NumberBoxPanel.h"


NumberBoxPanel::NumberBoxPanel(axWindow* parent,
                     const axRect& rect):
// Parent.
axPanel(parent, rect)
{
    std::string app_path(axApp::GetInstance()->GetAppDirectory());
    
    _squareColor = axColor(0.0, 0.0, 0.0, 1.0);
    _squareColorBottom = axColor(1.0, 0.0, 0.0, 1.0);
    _squareContourColor = axColor(0.0, 0.0, 0.0);
    
    axNumberBox::Info box_info(axColor(0.7, 0.7, 0.7, 0.0),
                               axColor(0.3, 0.3, 0.3, 0.0),
                               axColor(0.2, 0.2, 0.2, 0.0),
                               axColor(0.2, 0.2, 0.2, 0.0),
                               axColor(0.0, 0.0, 0.0, 0.0),
                               axColor(0.0, 0.0, 0.0, 1.0),
                               app_path + std::string("NumberBox.png"),
                               true);
    
    axNumberBox::Info box_info2(axColor(0.7, 0.7, 0.7, 0.0),
                                axColor(0.3, 0.3, 0.3, 0.0),
                                axColor(0.2, 0.2, 0.2, 0.0),
                                axColor(0.2, 0.2, 0.2, 0.0),
                                axColor(0.0, 0.0, 0.0, 0.0),
                                axColor(0.0, 0.0, 0.0, 1.0),
                                app_path + std::string("NumberBoxFull.png"),
                                false);
    
    axNumberBox* box1 = new axNumberBox(this,
                                        axRect(40, 40, 40, 20),
                                        axNumberBox::Events(),
                                        box_info,
										axNumberBox::Flags::SINGLE_IMG);
    
    axNumberBox* box2 = new axNumberBox(this,
                                        axRect(100, 40, 40, 20),
                                        axNumberBox::Events(),
                                        box_info2);
    
    axNumberBox::Info box_info3(axColor(0.7, 0.7, 0.7, 0.0),
                                axColor(0.8, 0.8, 0.8, 0.0),
                                axColor(0.6, 0.6, 0.6, 0.0),
                                axColor(0.2, 0.2, 0.2, 0.0),
                                axColor(0.0, 0.0, 0.0, 0.0),
                                axColor(0.0, 0.0, 0.0, 1.0),
                                app_path + std::string("NumberBox1.png"),
                                false);
    
    axNumberBox* box3 = new axNumberBox(this,
                                        axRect(200, 40, 40, 20),
                                        axNumberBox::Events(),
                                        box_info3);
    
    axNumberBox::Events box_evts;
    box_evts.value_change = GetOnNumberBoxRed();
    
    axNumberBox* box_red = new axNumberBox(this,
                                           axRect(40, 140, 40, 20),
                                           box_evts,
                                           box_info2);
    
    box_evts.value_change = GetOnNumberBoxGreen();
    
    axNumberBox* box_green = new axNumberBox(this,
                                           axRect(40, 165, 40, 20),
                                           box_evts,
                                           box_info2);
    
    box_evts.value_change = GetOnNumberBoxBlue();
    
    axNumberBox* box_blue = new axNumberBox(this,
                                           axRect(40, 190, 40, 20),
                                           box_evts,
                                           box_info2);
    
    box_evts.value_change = GetOnNumberBoxAlpha();
    
    axNumberBox* box_alpha = new axNumberBox(this,
                                            axRect(40, 215, 40, 20),
                                            box_evts,
                                            box_info2,
                                            axFLAG_NONE,
                                            1.0);
    
    ///---------------
    box_evts.value_change = GetOnNumberBoxRedBottom();
    
    axNumberBox* box_red_botom =
    new axNumberBox(this,
                    axRect(195, 140, 40, 20),
                    box_evts,
                    box_info2,
                    axFLAG_NONE,
                    1.0);
    
    box_evts.value_change = GetOnNumberBoxGreenBottom();
    
    axNumberBox* box_green_botom =
    new axNumberBox(this,
                    axRect(195, 165, 40, 20),
                    box_evts,
                    box_info2);
    
    box_evts.value_change = GetOnNumberBoxBlueBottom();
    
    axNumberBox* box_blue_botom =
    new axNumberBox(this,
                    axRect(195, 190, 40, 20),
                    box_evts,
                    box_info2);
    
    box_evts.value_change = GetOnNumberBoxAlphaBottom();
    axNumberBox* box_alpha_botom =
    new axNumberBox(this,
                    axRect(195, 215, 40, 20),
                    box_evts,
                    box_info2,
                    axFLAG_NONE,
                    1.0);
    
    _squareRect = axRect(90, 140, 100, 100);
}

void NumberBoxPanel::OnNumberBoxRed(const axNumberBox::Msg& msg)
{
    _squareColor.SetRed(msg.GetValue());
    Update();
}

void NumberBoxPanel::OnNumberBoxGreen(const axNumberBox::Msg& msg)
{
    _squareColor.SetGreen(msg.GetValue());
    Update();
}

void NumberBoxPanel::OnNumberBoxBlue(const axNumberBox::Msg& msg)
{
    _squareColor.SetBlue(msg.GetValue());
    Update();
}

void NumberBoxPanel::OnNumberBoxAlpha(const axNumberBox::Msg& msg)
{
    _squareColor.SetAlpha(msg.GetValue());
    Update();
}

void NumberBoxPanel::OnNumberBoxRedBottom(const axNumberBox::Msg& msg)
{
    _squareColorBottom.SetRed(msg.GetValue());
    Update();
}

void NumberBoxPanel::OnNumberBoxGreenBottom(const axNumberBox::Msg& msg)
{
    _squareColorBottom.SetGreen(msg.GetValue());
    Update();
}

void NumberBoxPanel::OnNumberBoxBlueBottom(const axNumberBox::Msg& msg)
{
    _squareColorBottom.SetBlue(msg.GetValue());
    Update();
}

void NumberBoxPanel::OnNumberBoxAlphaBottom(const axNumberBox::Msg& msg)
{
    _squareColorBottom.SetAlpha(msg.GetValue());
    Update();
}

void NumberBoxPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(1.0, 1.0, 1.0), 1.0);
    gc->DrawRectangle(rect0);
    
    gc->DrawRectangleColorFade(_squareRect, _squareColor, _squareColorBottom);
    
    gc->SetColor(_squareContourColor);
    gc->DrawRectangleContour(_squareRect);

    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
}