//
//  PianoNote.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-09.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__PianoNote__
#define __DrumNBass__PianoNote__

#include "axLib.h"

class PianoNote: public axPanel
{
public:
    /***************************************************************************
     * PianoNote::Msg.
     **************************************************************************/
    class Msg : public axMsg
    {
    public:
        Msg();
        
        Msg(PianoNote* sender, const int& index, const int& oper);
        
        PianoNote* GetSender() const;
        
        int GetIndex() const;
        
        int GetOperation() const;
        
        axMsg* GetCopy();
        
    private:
        PianoNote* _sender;
        int _index;
        int _operation;
    };
    
    /***************************************************************************
     * PianoNote::Events.
     **************************************************************************/
    class Events
    {
    public:
        enum : axEventId { SIZE_CHANGE };
        
        Events(){}
        Events(const axEventFunction& fct){ size_change = fct; }
        
        axEventFunction size_change;
    };
    
    /***************************************************************************
     * PianoNote::PianoNote.
     **************************************************************************/
    PianoNote(axWindow* parent,
              const axRect& rect,
              const PianoNote::Events& events,
              const int& index,
              const bool& black_note);
    
private:
    bool _blackNote;
    int _index;
    bool _hasVelocity;
    int _nTrack;
    axFont* _font;
    
    // axEvents.
    virtual void OnPaint();
    
    axEVENT_ACCESSOR(axButton::Msg, OnAddTrack);
    void OnAddTrack(const axButton::Msg& msg);
    
    axEVENT_ACCESSOR(axButton::Msg, OnRemoveTrack);
    void OnRemoveTrack(const axButton::Msg& msg);
    
    axEVENT_ACCESSOR(axButton::Msg, OnVelocityDown);
    void OnVelocityDown(const axButton::Msg& msg);
};

#endif /* defined(__DrumNBass__PianoNote__) */
