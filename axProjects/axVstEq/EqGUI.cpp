//
//  EqGUI.cpp
//  AudioApp
//
//  Created by Alexandre Arsenault on 2015-02-27.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "EqGUI.h"
#include <axAudio/axAudioUtils.h>

struct BandControl
{
    BandControl()
    {
    }
    BandControl(const int& n, const bool& as_amp_,
                const axEventFunction& f_evt,
                const axEventFunction& q_evt,
                const axEventFunction& a_evt,
                const axEventFunction& active):
    freq(f_evt), q(q_evt), amp(a_evt), on(active), num(n), as_amp(as_amp_)
    {
        
    }
    axEventFunction freq, q, amp, on;
    int num;
    bool as_amp;
};

axPoint CreateFilterBandControls(axWindow* parent,
                                 const axPoint& pos,
                                 const BandControl& ctrl_info,
                                 const axSize& knobSize,
                                 const axKnob::Info& knob_info,
                                 const axToggle::Info& togInfoOnOff,
                                 const axSize& toggleSizeOnOff,
                                 const axLabel::Info& labelInfo,
                                 const axSize& labelSize)
{
    std::string msg = to_string(ctrl_info.num);
    
    axKnob* p1_freq = new axKnob(parent, axRect(pos, knobSize),
                                 axKnob::Events(ctrl_info.freq),
                                 knob_info, 0, 0.0, msg);
    
    p1_freq->SetValue(ctrl_info.num / 8.0);
    
    axToggle* tog = new axToggle(parent,
                                 axRect(p1_freq->GetNextPosUp(40) + axPoint(3, 0),
                                        toggleSizeOnOff),
                                 axToggle::Events(ctrl_info.on),
                                 togInfoOnOff, "",
                                 to_string(ctrl_info.num), 0,
                                 to_string(ctrl_info.num));
    
    tog->SetSelected(true);
    
    
    new axLabel(parent,
                axRect(p1_freq->GetNextPosUp(20), labelSize),
                labelInfo, "Freq");
    
    axKnob* p1_q = new axKnob(parent,
                              axRect(p1_freq->GetNextPosDown(20), knobSize),
                              axKnob::Events(ctrl_info.q),
                              knob_info, 0, 0.0, msg);
    
    axRange<double> qRange(0.1, 5.0);
    p1_q->SetValue(qRange.GetZeroToOneValue(0.707));
    
    new axLabel(parent, axRect(p1_q->GetNextPosUp(20), labelSize),
                labelInfo, "Q");
    
    if(ctrl_info.as_amp)
    {
        axKnob* p1_amp = new axKnob(parent,
                                    axRect(p1_q->GetNextPosDown(20), knobSize),
                                    axKnob::Events(ctrl_info.amp),
                                    knob_info, 0, 0.0, msg);
        
        p1_amp->SetValue(0.5);
        
        new axLabel(parent, axRect(p1_amp->GetNextPosUp(20), labelSize),
                    labelInfo, "Gain");
    }
    
    
    return p1_freq->GetNextPosRight(25);
}

EqGUI::EqGUI(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    
    _filter = new FilterParametric(this,
                                   axRect(axPoint(10, 10), axSize(480, 200)));
                                          
    
    axLabel::Info labelInfo;
    labelInfo.normal = axColor::axTransparentColor;
    labelInfo.contour = axColor::axTransparentColor;
    labelInfo.font_size = 12;
    labelInfo.font_color = axColor(0.4);
    labelInfo.font_name = "";
    labelInfo._alignement = axALIGN_CENTER;
    
    axToggle::Info togInfoOnOff;
    togInfoOnOff.normal = axColor(0.6);
    togInfoOnOff.hover = axColor(0.7);
    togInfoOnOff.clicking = axColor(0.65);
    togInfoOnOff.selected = axColor(0.9, 0.4, 0.0);
    togInfoOnOff.selected_hover = axColor(0.95, 0.4, 0.0);
    togInfoOnOff.selected_clicking = axColor(0.85, 0.4, 0.0);
    togInfoOnOff.contour = axColor(0.0);
    togInfoOnOff.img = "";
    togInfoOnOff.single_img = false;
    togInfoOnOff.font_color = axColor(0.0);
    togInfoOnOff.selected_font_color = axColor(1.0);
    togInfoOnOff.font_size = 10;
    
    axSize toggleSizeOnOff(30, 14);
    axSize knobSize(36, 36);
    axSize labelSize(36, 20);
    
    axKnob::Info knob_info(axColor::axTransparentColor,
                           axColor::axTransparentColor,
                           axColor::axTransparentColor,
                           128,
                           knobSize,
                           std::string("greyKnob.png"),
                           std::string("greyKnob.png"));
    
    // Create all 8 bands.
    BandControl band_ctrl(0, false, GetOnP1FreqChange(),
                          GetOnP1QChange(),
                          GetOnP1AmpChange(),
                          GetOnToggleF1());
    
    axPoint next_pos(axPoint(18, 430));
    for(int i = 0; i < 8; i++)
    {
        if(i > 0 && i < 7)
        {
            band_ctrl.as_amp = true;
        }
        else
        {
            band_ctrl.as_amp = false;
        }
        
        band_ctrl.num = i + 1;
        next_pos = CreateFilterBandControls(this,
                                            next_pos,
                                            band_ctrl, knobSize, knob_info,
                                            togInfoOnOff, toggleSizeOnOff,
                                            labelInfo, labelSize);
    }
}

void EqGUI::OnToggleF1(const axToggle::Msg& msg)
{
    int index = stoi(msg.GetMsg()) - 1;
    _filter->SetActive(index, msg.GetSelected());
    
//    Audio* audio = GetApp()->GetResourceManager()->GetResource("Audio");
//    audio->SetActive(index, msg.GetSelected());
}

void EqGUI::OnP1FreqChange(const axKnob::Msg& msg)
{
    axPrint("Freq value change");
    double f = axAudio::LogarithmicInterpole<double>(20.0,
                                                     20000.0,
                                                     msg.GetValue());
    
    int index = stoi(msg.GetMsg()) - 1;
    _filter->SetFrequency(index, f);
    
//    Audio* audio = GetApp()->GetResourceManager()->GetResource("Audio");
//    audio->SetFreq(index, f);
}

void EqGUI::OnP1QChange(const axKnob::Msg& msg)
{
    double q = axAudio::LineairInterpole(0.1, 5.0, msg.GetValue());
    
    int index = stoi(msg.GetMsg()) - 1;
    _filter->SetQ(index, q);
    
//    Audio* audio = GetApp()->GetResourceManager()->GetResource("Audio");
//    audio->SetQ(index, q);
}

void EqGUI::OnP1AmpChange(const axKnob::Msg& msg)
{
    double amp = axAudio::LineairInterpole(0.1, 1.9, msg.GetValue());
    
    int index = stoi(msg.GetMsg()) - 1;
    _filter->SetAmp(index, amp);
    
//    Audio* audio = GetApp()->GetResourceManager()->GetResource("Audio");
//    audio->SetAmp(index, amp);
}

void EqGUI::OnPaint()
{
    axGC gc(this);
    axRect rect(GetRect());
    
    gc.DrawRectangleColorFade(rect, axColor(0.8), axColor(0.9));
    
    gc.SetColor(axColor(0.45));
    gc.DrawRectangle(axRect(0, 0, rect.size.x, 30));
}
