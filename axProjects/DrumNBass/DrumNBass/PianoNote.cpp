//
//  PianoNote.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-09.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "PianoNote.h"

/*******************************************************************************
 * PianoNote::Msg.
 ******************************************************************************/
PianoNote::Msg::Msg()
{
    _sender = nullptr;
}

PianoNote::Msg::Msg(PianoNote* sender,
                    const int& index,
                    const int& oper)
{
    _sender = sender;
    _index = index;
    _operation = oper;
}

PianoNote* PianoNote::Msg::GetSender() const
{
    return _sender;
}

int PianoNote::Msg::GetIndex() const
{
    return _index;
}

int PianoNote::Msg::GetOperation() const
{
    return _operation;
}

axMsg* PianoNote::Msg::GetCopy()
{
    return new PianoNote::Msg(*this);
}

PianoNote::PianoNote(axWindow* parent,
                     const axRect& rect,
                     const PianoNote::Events& events,
                     const int& index,
                     const bool& black_note):
axPanel(parent, rect),
_blackNote(black_note),
_index(index),
_hasVelocity(false),
_nTrack(1)
{
    if(events.size_change)
    {
        axObject::AddConnection(Events::SIZE_CHANGE, events.size_change);
    }
    
    axButton::Info btn_info_black(axColor(0.17, 0.17, 0.17, 0.0),
                                  axColor(0.3, 0.3, 0.3, 0.4),
                                  axColor(0.2, 0.2, 0.2, 0.4),
                                  axColor(0.2, 0.2, 0.2, 0.0),
                                  axColor(0.0, 0.0, 0.0, 0.0),
                                  axColor(0.6, 0.6, 0.6));
    
    axButton::Info btn_info_white(axColor(0.17, 0.17, 0.17, 0.0),
                                  axColor(0.3, 0.3, 0.3, 0.4),
                                  axColor(0.2, 0.2, 0.2, 0.4),
                                  axColor(0.2, 0.2, 0.2, 0.0),
                                  axColor(0.0, 0.0, 0.0, 0.0),
                                  axColor(0.4, 0.4, 0.4));
    
    axButton::Info btn_info = _blackNote ? btn_info_black : btn_info_white;
    
    int note_height = 10;
    axRect drawRect(GetDrawingRect());
    
    axButton* down = new axButton(this,
                                  axRect(0, 0,
                                         10, note_height),
                                  axButton::Events(GetOnVelocityDown()),
                                  btn_info, "", ">");
    
    axButton* plus = new axButton(this,
                                  axRect(10, 0,
                                         10, note_height),
                                  axButton::Events(GetOnAddTrack()),
                                  btn_info, "", "+");
    
    axButton* minus = new axButton(this,
                                   axRect(20, 0,
                                          10, note_height),
                                   axButton::Events(GetOnRemoveTrack()),
                                   btn_info, "", "-");
}

void PianoNote::OnAddTrack(const axButton::Msg& msg)
{
    if(_nTrack < 3)
    {
        axSize size(GetSize());
        size.y += 10;
        SetSize(size);
        GetParent()->OnResize();
        
        _nTrack++;
        
        axObject::PushEvent(Events::SIZE_CHANGE,
                            new PianoNote::Msg(this, _index, 1));
    }
}

void PianoNote::OnRemoveTrack(const axButton::Msg& msg)
{
    axSize size(GetSize());
    
//    if(size.y > 10)
    if(_nTrack > 1)
    {
        size.y -= 10;
        SetSize(size);
        GetParent()->OnResize();
        
        _nTrack--;
        
        axObject::PushEvent(Events::SIZE_CHANGE,
                            new PianoNote::Msg(this, _index, 0));
    }
    
}

void PianoNote::OnVelocityDown(const axButton::Msg& msg)
{
    if(_hasVelocity)
    {
        axSize size(GetSize());
        size.y -= 40;
        SetSize(size);
        
        GetParent()->OnResize();
        _hasVelocity = false;
    }
    else
    {
        axSize size(GetSize());
        size.y += 40;
        SetSize(size);
        
        GetParent()->OnResize();
        _hasVelocity = true;
    }
    
    axObject::PushEvent(Events::SIZE_CHANGE,
                        new PianoNote::Msg(this, _index, 3));
}

void PianoNote::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    
    _blackNote ? gc->SetColor(axColor(0.0)) : gc->SetColor(axColor(1.0));
    gc->DrawRectangle(rect);
    
   
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect);
}