//
//  MidiTrack.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-08.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__MidiTrack__
#define __DrumNBass__MidiTrack__

#include "axLib.h"

class MidiInstrument: public axPanel
{
public:
    /***************************************************************************
     * MidiInstrument.
     **************************************************************************/
    class Msg : public axMsg
    {
    public:
        Msg();
        
        Msg(MidiInstrument* sender)
        {
            _sender = sender;
        }
        
        MidiInstrument* GetSender() const
        {
            return _sender;
        }
        
        axMsg* GetCopy()
        {
            return new MidiInstrument::Msg(*this);
        }
        
    private:
        MidiInstrument* _sender;
    };
    
    /***************************************************************************
     * MidiInstrument::Events.
     **************************************************************************/
    class Events
    {
    public:
        enum : axEventId { SIZE_CHANGE };
        
        Events(){}
        Events(const axEventFunction& fct){ size_change = fct; }
        
        axEventFunction size_change;
    };
    
    MidiInstrument(axWindow* parent,
                   const axRect& rect,
                   const MidiInstrument::Events& events,
                   const std::string& trackName);
    
private:
    int _nTrack;
    axFont* _font;
    
    axEVENT_ACCESSOR(axButton::Msg, OnExpand);
    
    void OnExpand(const axButton::Msg& msg);
    
    // axEvents.
    virtual void OnPaint();
};

#endif /* defined(__DrumNBass__MidiTrack__) */
