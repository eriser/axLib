//
//  DrumMidiVelocity.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-06.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "DrumMidiVelocity.h"

DrumMidiVelocity::DrumMidiVelocity(axWindow* parent,
                                   const axRect& rect,
                                   const DrumMidiVelocity::Events& events):
axPanel(parent, rect)
// Members.
//_velocity_fct(fct),
//_standard_deviation_fct(sfct)
{
    if(events.value_change)
    {
        axObject::AddConnection(events.VALUE_CHANGE, events.value_change);
    }
    
    for(int i = 0; i < 16; i++)
    {
        double ratio = (rect.size.x) / double(16);
        int left_x = i * ratio;
        int right_x = (i+1.0) * ratio;
        
        axRect r(left_x, 0, right_x - left_x, rect.size.y + 1);
        
        axColor color(0.75, 0.75, 0.75);
        if(i % 2)
        {
            color = axColor(0.8, 0.8, 0.8);
        }
        
        //axEvtFunction(MultipleSliderMsg) sldfct(GetOnChangeVelocity());
        _sliders.push_back(new axMultipleSlider(this, r, color, i));
    }
    
    
    // axKnobInfo knob_info(axColor(0.6, 0.6, 0.6),
    //                    axColor("#AAAAAA"),
    //                    axColor("#BBBBBB"),
    //                    128,
    //                    axSize( 32, 32 ),
    //                    "knob.png",
    //                    "knobSelected.png");
    
    // axEvtFunction(axKnobMsg) evt(GetOnStandardDeviation());
    // axKnob* knob = new axKnob(app, this, axRect(25, 15, 32, 32),
    //                         axKnobEvents(evt), knob_info);
    
//    axNumberBox::Info box_info(axColor(0.7, 0.7, 0.7), // Normal.
//                               axColor(0.75, 0.75, 0.75), // Hover.
//                               axColor(0.65, 0.65, 0.65), // Clicking.
//                               axColor(0.2, 0.2, 0.2), // Selected.
//                               axColor(0.0, 0.0, 0.0), // Contour.
//                               axColor(0.0, 0.0, 0.0)); // Font color.
//    
//    axNumberBox::Events numEvents;
//    numEvents.value_change = GetOnStandardDeviation();
    
//    axNumberBox* box1 = new axNumberBox(this,
//                                        axRect(15, 10, 40, 20),
//                                        numEvents,
//                                        box_info);
    
}

//void MidiVelocity::SetPreset(TrackInfo* info)
//{
//    SetNumberOfSlider(info->nSubTrack);
//    
//    for(int j = 0; j < 3; j++)
//    {
//        for(int i = 0; i < 16; i++)
//        {
////            _sliders[i]->SetValue(j, info->velocity[j][i]);
//        }
//    }
//}

void DrumMidiVelocity::SetNumberOfSlider(const int& nb)
{
    for(int i = 0; i < _sliders.size(); i++)
    {
        _sliders[i]->SetNumberOfSlider(nb);
    }
}

void DrumMidiVelocity::OnChangeVelocity(const axMultipleSlider::Msg& msg)
{
    axMultipleSlider* sld = msg.GetSender();
    int bar_index = -1;
    
    for(int i = 0; i < 16; i++)
    {
        if(_sliders[i] == sld)
        {
            bar_index = i;
        }
    }
    
    if(bar_index != -1)
    {
        axObject::PushEvent(DrumMidiVelocity::Events::VALUE_CHANGE,
                            new DrumMidiVelocity::Msg(this, bar_index,
                                                      msg.GetIndex(),
                                                      msg.GetValue()));
    }
    
//    int bar_index
//    DrumMidiVelocity::Msg* d_msg = new DrumMidiVelocit
    
}

//void MidiVelocity::OnChangeVelocity(const MultipleSliderMsg& msg)
//{
////    if(_velocity_fct)
////    {
////        _velocity_fct(msg);
////    }
//}

void DrumMidiVelocity::OnStandardDeviation(const axNumberBox::Msg& msg)
{
//    if(_standard_deviation_fct)
//    {
//        //------------------------------------------------------------------------------------------
//        //_standard_deviation_fct(new const axNumberBoxMsg(*msg));
//    }
}

void DrumMidiVelocity::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.6));
    gc->DrawRectangle(rect0);
    
    gc->SetColor(axColor(0.7));
    
    for(int i = 0; i < 20; i++)
    {
        int y_pos = (i / double(20)) * (rect0.size.y);
        gc->DrawLine(axPoint(60, y_pos), axPoint(rect.size.x-14, y_pos));
    }
    
    gc->SetColor(axColor::axBlackColor);
    
    for(int i = 0; i < 16+1; i++)
    {
        int x_pos = (i / double(16)) * (rect0.size.x - 60 - 14) + 60;
        gc->DrawLine(axPoint(x_pos, 0), axPoint(x_pos, rect0.size.y));
    }
    
    gc->DrawRectangleContour(rect0);
}