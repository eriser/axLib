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
                                   const axMultipleSlider::Events& events,
                                   const axColor& bgColor):
axPanel(parent, rect),
// Members.
_bgColor(bgColor),
_nSlider(3)
{
    if(events.value_change)
    {
        axObject::AddConnection(events.VALUE_CHANGE, events.value_change);
    }
    
    axSliderInfo sld_info("", // Image path.
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
    
    double position_ratio = rect.size.x / double(_nSlider);
    int size_x = rect.size.x / double(_nSlider);
    
    axFlag slider_flags = axSLIDER_FLAG_VERTICAL |
                          axSLIDER_FLAG_CLICK_ANYWHERE |
                          axSLIDER_FLAG_RIGHT_ALIGN;
    
    double left = position_ratio * 0.0;
    double right = position_ratio * 1.0;
    sliders[0] = new axSlider(this,
                              axRect(left, 0, right - left, rect.size.y),
                              axSliderEvents(GetOnSlider1Move()),
                              sld_info,
                              slider_flags);

    left = position_ratio * 1.0;
    right = position_ratio * 2.0;
    sliders[1] = new axSlider(this,
                              axRect(left, 0, right - left, rect.size.y),
                              axSliderEvents(GetOnSlider2Move()),
                              sld_info,
                              slider_flags);
    
    left = position_ratio * 2.0;
    right = position_ratio * 3.0;
    sliders[2] = new axSlider(this,
                              axRect(left, 0, right - left, rect.size.y),
                              axSliderEvents(GetOnSlider3Move()),
                              sld_info,
                              slider_flags);
        
    sliders[0]->SetValue(0.3);
    sliders[1]->SetValue(0.3);
    sliders[2]->SetValue(0.3);
    
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
    axObject::PushEvent(axMultipleSlider::Events::VALUE_CHANGE,
                        new Msg(this, 0, 1.0 - msg.GetValue()));
}

void axMultipleSlider::OnSlider2Move(const axSliderMsg& msg)
{
    axObject::PushEvent(axMultipleSlider::Events::VALUE_CHANGE,
                        new Msg(this, 1, 1.0 - msg.GetValue()));
}

void axMultipleSlider::OnSlider3Move(const axSliderMsg& msg)
{
    axObject::PushEvent(axMultipleSlider::Events::VALUE_CHANGE,
                        new Msg(this, 2, 1.0 - msg.GetValue()));
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