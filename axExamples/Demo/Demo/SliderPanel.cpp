//
//  SliderPanel.cpp
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-03.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "SliderPanel.h"

SliderPanel::SliderPanel(axApp* app,
                         axWindow* parent,
                         const axRect& rect):
// Parent.
axPanel(app, parent, rect)
{
   // std::string app_path("/Users/alexarse/Project/axLib/axExamples/Demo/");
	std::string app_path("C:/Users/Alexandre Arsenault/Desktop/axLib/axExamples/Demo/VisualStudio2013/axGL/");

    axSliderInfo sld_info;
    sld_info.img_path = app_path + std::string("sliderPlain.png");
    sld_info.btn_size = axSize(12, 12);
    sld_info.slider_width = 4;
    
    // Doesn't seem to work for now.
    sld_info.bgColorNormal = axColor(0.7, 0.7, 0.7);
    sld_info.bgColorHover = axColor(0.9, 0.9, 0.9);
    sld_info.bgColorClicked = axColor(0.6, 0.6, 0.6);
    sld_info.contourColor = axColor(0.0, 0.0, 0.0);
    
    sld_info.sliderColorNormal = axColor(0.2, 0.2, 0.9);
    sld_info.sliderColorHover = axColor(0.2, 0.2, 1.0);
    sld_info.sliderColorClicked = axColor(0.2, 0.2, 0.6);
    sld_info.sliderContourColor = axColor(0.2, 0.2, 0.2);
    
    sld_info.backSliderColor = axColor(0.7, 0.7, 0.7);
    sld_info.backSliderContourColor = axColor(0.2, 0.2, 0.2);
    
    axSliderEvents sld_evts;
    sld_evts.slider_value_change = GetOnSlider1();
    
    axSlider* sld1 = new axSlider(app, this, axRect(40, 80, 300, 15),
                                  sld_evts, sld_info);
    
    sld1->SetBackgroundAlpha(0.3);
    
    axSlider* sld2 = new axSlider(app, this, axRect(40, 100, 300, 15),
                                  sld_evts, sld_info,
                                  axSLIDER_FLAG_BACK_SLIDER);
    
    sld2->SetBackgroundAlpha(0.3);
    
    axSlider* sld3 = new axSlider(app, this, axRect(40, 120, 300, 15),
                                  sld_evts, sld_info,
                                  axSLIDER_FLAG_NO_SLIDER_LINE);
    
    sld3->SetBackgroundAlpha(0.3);
    
    
    axSlider* sld4 = new axSlider(app, this, axRect(40, 140, 300, 15),
                                  sld_evts, sld_info,
                                  axSLIDER_FLAG_CLICK_ANYWHERE);
    
    sld4->SetBackgroundAlpha(0.3);
    
    axSlider* sld5 = new axSlider(app, this, axRect(40, 160, 300, 15),
                                  sld_evts, sld_info,
                                  axSLIDER_FLAG_CLICK_ANYWHERE |
                                  axSLIDER_FLAG_RIGHT_ALIGN);
    
    sld5->SetBackgroundAlpha(0.3);
    
    sld_info.img_path = "";
    sld_info.btn_size = axSize(12, 12);
    sld_info.slider_width = 13;
    axSlider* sld6 = new axSlider(app, this, axRect(40, 180, 300, 15),
                                  sld_evts, sld_info,
                                  axSLIDER_FLAG_CLICK_ANYWHERE);
    
    sld6->SetBackgroundAlpha(0.3);
    
    
    sld_info.img_path = "";
    sld_info.btn_size = axSize(12, 12);
    sld_info.slider_width = 6;
    axSlider* sld7 = new axSlider(app, this, axRect(40, 200, 300, 15),
                                  sld_evts, sld_info,
                                  axSLIDER_FLAG_CLICK_ANYWHERE|
                                  axSLIDER_FLAG_MIDDLE_ALIGN);
    
    sld7->SetBackgroundAlpha(0.3);
    
    
    axSlider* sld8 = new axSlider(app, this, axRect(40, 240, 15, 100),
                                  sld_evts, sld_info,
                                  axSLIDER_FLAG_CLICK_ANYWHERE|
                                  axSLIDER_FLAG_VERTICAL |
                                  axSLIDER_FLAG_RIGHT_ALIGN);
    
    sld8->SetBackgroundAlpha(0.3);
    
    axSlider* sld9 = new axSlider(app, this, axRect(60, 240, 15, 100),
                                  sld_evts, sld_info,
                                  axSLIDER_FLAG_CLICK_ANYWHERE |
                                  axSLIDER_FLAG_VERTICAL |
                                  axSLIDER_FLAG_RELEASE_ON_LEAVE);
    
    sld9->SetBackgroundAlpha(0.3);
    
    
    sld_info.img_path = app_path + std::string("sliderCtrl.png");
    sld_info.btn_size = axSize(12, 12);
    sld_info.slider_width = 5;
    int x = 80;
    for(int i = 0; i < 5; i++, x+= 15)
    {
        axSlider* sld = new axSlider(app, this, axRect(x, 240, 15, 100),
                                     sld_evts, sld_info,
                                     axSLIDER_FLAG_CLICK_ANYWHERE |
                                     axSLIDER_FLAG_VERTICAL |
                                     axSLIDER_FLAG_RIGHT_ALIGN);
        
        sld->SetBackgroundAlpha(0.3);
    }
    
    
}

void SliderPanel::OnSlider1(const axSliderMsg& msg)
{
    std::cout << msg.GetValue() << std::endl;
}

void SliderPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.4, 0.4, 0.4), 1.0);
    gc->DrawRectangle(rect0);
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->SetFontSize(12);
    //    gc->DrawStringAlignedCenter("test", rect0);
    gc->DrawString("regular", axPoint(350, 80));
    gc->DrawString("with back slider", axPoint(350, 100));
    gc->DrawString("no slider", axPoint(350, 120));
    gc->DrawString("click anywhere", axPoint(350, 140));
    gc->DrawString("right align", axPoint(350, 160));
    gc->DrawString("no button", axPoint(350, 180));
    gc->DrawString("center align", axPoint(350, 200));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
}