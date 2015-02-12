//
//  MidiTrack.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-08.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "MidiInstrument.h"

MidiInstrument::MidiInstrument(axWindow* parent,
                               const axRect& rect,
                               const MidiInstrument::Events& events,
                               const std::string& name):
axPanel(parent, rect),
_nTrack(1)
{
    if(events.size_change)
    {
        axObject::AddConnection(events.SIZE_CHANGE, events.size_change);
    }
    
    _font = new axFont(0);
    axButton::Info btn_info(axColor(0.17, 0.17, 0.17, 0.0),
                            axColor(0.3, 0.3, 0.3, 0.4),
                            axColor(0.2, 0.2, 0.2, 0.4),
                            axColor(0.2, 0.2, 0.2, 0.0),
                            axColor(0.0, 0.0, 0.0, 0.0),
                            axColor(0.8, 0.8, 0.8));
    
    axButton* nameBtn = new axButton(this,
                                  axRect(60, 20, 20, 20),
                                  axButton::Events(GetOnExpand()),
                                  btn_info,
                                  "",
                                  "+",
                                  axButton::Flags::SINGLE_IMG);
}

void MidiInstrument::OnExpand(const axButton::Msg& msg)
{
    axSize size(GetSize());
    
    SetSize(axSize(size.x, size.y + 40));
    
    axObject::PushEvent(MidiInstrument::Events::SIZE_CHANGE,
                        new MidiInstrument::Msg(this));
    
    _nTrack++;
}

void MidiInstrument::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    
    gc->SetColor(axColor(0.9, 0.9, 0.9));
    gc->DrawRectangle(rect);
    
    gc->SetColor(axColor(0.17, 0.17, 0.17));
    
    axRect nameRect(0, 0, 80, rect.size.y);
    gc->DrawRectangle(nameRect);
    
    gc->SetColor(axColor(0.8, 0.8, 0.8));
    gc->DrawStringAlignedCenter(*_font, "TrackName", axRect(0, 0, 80, 20));
    
    gc->SetColor(axColor::axBlackColor);
    gc->DrawRectangleContour(nameRect);
    
    gc->SetColor(axColor(0.4));
    for(int i = 0; i < _nTrack; i++)
    {
        gc->DrawLine(axPoint(0, i * 40), axPoint(rect.size.x, i * 40));
    }
    
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect);
}