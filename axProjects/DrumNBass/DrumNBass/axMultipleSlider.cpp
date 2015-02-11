//
//  axMultipleSlider.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-06.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "axMultipleSlider.h"

/*******************************************************************************
 * axMultipleSlider::Msg.
 ******************************************************************************/
axMultipleSlider::Msg::Msg()
{
    _sender = nullptr;
}

axMultipleSlider::Msg::Msg(axMultipleSlider* sender,
                           const int& index, const double& value)
{
    _sender = sender;
    _index = index;
    _value = value;
}

axMultipleSlider* axMultipleSlider::Msg::GetSender() const
{
    return _sender;
}


double axMultipleSlider::Msg::GetValue() const
{
    return _value;
}

int axMultipleSlider::Msg::GetIndex() const
{
    return _index;
}

axMsg* axMultipleSlider::Msg::GetCopy()
{
    return new axMultipleSlider::Msg(*this);
}


axMultipleSlider::axMultipleSlider(axWindow* parent,
                               const axRect& rect,
                               const axColor& bgColor,
                               const int& barIndex):
axPanel(parent, rect),
// Members.
_barIndex(barIndex)
{
    _bgColor = bgColor;
    
    axSliderInfo sld_info("",//"sliderPlain.png",
                          axSize(8, 8),
                          axColor(0.9, 0.9, 0.9, 0.2), // Bg
                          axColor(1.0, 1.0, 1.0, 0.1), // bg hover.
                          axColor(0.9, 0.9, 0.9), // Bg clicking.
                          axColor(0.3, 0.3, 0.3), // Slider normal
                          axColor(0.3, 0.3, 0.3), // Slider hover
                          axColor(0.4, 0.4, 0.4), // Slider clicking.
                          axColor(0.3, 0.3, 0.3), // Slider contour.
                          axColor(0.0, 0.0, 0.0), // Contour
                          axColor(0.9, 0.9, 0.9), // Back Slider
                          axColor(0.9, 0.9, 0.9), // Back Slider contour.
                          3);
    
    _nSlider = 3;
    double position_ratio = rect.size.x / double(_nSlider);
    int size_x = rect.size.x / double(_nSlider);
    
    /*double t = position_ratio * 1.0;*/
    
    axFlag slider_flags = axSLIDER_FLAG_VERTICAL |
    axSLIDER_FLAG_CLICK_ANYWHERE |
    axSLIDER_FLAG_RIGHT_ALIGN;
    
    
    //    axEvtFunction(axSliderMsg) sld_fct(GetOnSlider1Move());
    
    axSliderEvents sldEvents;
    sldEvents.slider_value_change = GetOnSlider1Move();
    // function<void(const axSliderMsg&)> sld_fct(GetOnSliderMove());
    
    double left = position_ratio * 0.0;
    double right = position_ratio * 1.0;
    sliders[0] = new axSlider(this,
                              axRect(left, 0, right - left, rect.size.y),
                              sldEvents, sld_info, slider_flags);

    left = position_ratio * 1.0;
    right = position_ratio * 2.0;
    sldEvents.slider_value_change = GetOnSlider2Move();
    sliders[1] = new axSlider(this,
                              axRect(left, 0, right - left, rect.size.y),
                              sldEvents, sld_info, slider_flags);
    
    left = position_ratio * 2.0;
    right = position_ratio * 3.0;
    sldEvents.slider_value_change = GetOnSlider3Move();
    sliders[2] = new axSlider(this,
                              axRect(left, 0, right - left, rect.size.y),
                              sldEvents, sld_info, slider_flags);
        
    sliders[0]->SetValue(0.7);
    sliders[1]->SetValue(0.7);
    sliders[2]->SetValue(0.7);
    // sliders[2]->Hide();
    
    SetNumberOfSlider(1);
    
}

void axMultipleSlider::SetValue(const int& index,
                              const double& value)
{
    if(index >= 0 && index < 3)
    {
        sliders[index]->SetValue(value);
    }
}


void axMultipleSlider::OnSlider1Move(const axSliderMsg& msg)
{
//    if (_slider_fct)
//    {
//        axMultipleSliderMsg m;
//        m.index = 0;
//        m.value = 1.0 - msg.GetValue();
//        m.bar_index = _barIndex;
//        
//        _slider_fct(m);
//    }
}

void axMultipleSlider::OnSlider2Move(const axSliderMsg& msg)
{
//    if (_slider_fct)
//    {
//        axMultipleSliderMsg m;
//        m.index = 1;
//        m.value = 1.0 - msg.GetValue();
//        m.bar_index = _barIndex;
//        
//        _slider_fct(m);
//    }
}

void axMultipleSlider::OnSlider3Move(const axSliderMsg& msg)
{
//    if (_slider_fct)
//    {
//        axMultipleSliderMsg m;
//        m.index = 2;
//        m.value = 1.0 - msg.GetValue();
//        m.bar_index = _barIndex;
//        
//        _slider_fct(m);
//    }
}

void axMultipleSlider::SetNumberOfSlider(const int& nb)
{
    _nSlider = nb;
    
    if(_nSlider == 1)
    {
        axSize rSize(GetRect().size);
        int size_x = rSize.x / double(_nSlider);
        sliders[0]->ResizeSlider(axSize(size_x, rSize.y));
        
        sliders[1]->Hide();
        sliders[2]->Hide();
    }
    else if(_nSlider == 2)
    {
        axSize rSize(GetRect().size);
        int size_x = rSize.x / double(_nSlider);
        sliders[0]->ResizeSlider(axSize(size_x, rSize.y));
        sliders[1]->ResizeSlider(axSize(size_x, rSize.y));
        sliders[1]->SetPosition(axPoint(size_x, 0));
        sliders[1]->Show();
        sliders[2]->Hide();
    }
    else if(_nSlider == 3)
    {
        axSize rSize(GetRect().size);
        int size_x = rSize.x / double(_nSlider);
        sliders[0]->ResizeSlider(axSize(size_x, rSize.y));
        sliders[1]->ResizeSlider(axSize(size_x, rSize.y));
        sliders[1]->SetPosition(axPoint(size_x, 0));
        
        sliders[1]->Show();
        sliders[2]->Show();
    }
    Update();
}

void axMultipleSlider::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    // Draw background.
    gc->SetColor(_bgColor, 1.0);
    gc->DrawRectangle(rect0);
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
}