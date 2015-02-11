//
//  DrumMidiTrack.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__DrumMidiTrack__
#define __DrumNBass__DrumMidiTrack__

#include "axLib.h"
#include "DrumUtils.h"
#include "DrumMidiVelocity.h"
#include "DrumMidiTrackSequence.h"

class DrumMidiTrack: public axPanel
{
public:
    /***************************************************************************
     * DrumMidiTrack::Msg.
     **************************************************************************/
    class Msg : public axMsg
    {
    public:
        Msg();
        
        Msg(DrumMidiTrack* sender, const int& msg);
        
        DrumMidiTrack* GetSender() const;
        
        int GetMsg() const;
        
        axMsg* GetCopy();
        
    private:
        DrumMidiTrack* _sender;
        int _msg;
    };
    
    /***************************************************************************
     * DrumMidiTrack::Events.
     **************************************************************************/
    class Events
    {
    public:
        enum : axEventId { SIZE_CHANGE };
        
        Events(){}
        Events(const axEventFunction& fct){ size_change = fct; }
        
        axEventFunction size_change;
    };
    
    DrumMidiTrack(axWindow* parent,
                  const axRect& rect,
                  const DrumMidiTrack::Events& events,
                  const std::string& trackName,
                  int track_number);
    
//    void SetEvent(MidiTrackEvents& evt)
//    {
//        _evt = evt;
//    }
//    
//    void SetBtnClickEvent(axEvtFunction(int) fct)
//    {
//        _btn_evt_fct = fct;
//    }
//    
//    void SetPreset(TrackInfo* info);
//    
//    
    void SetColorSelection(const ColorChoice& choice);
//
//    axEVENT_ACCESSOR(axButton::Msg, OnAddSeparation);
//    axEVENT_ACCESSOR(axButton::Msg, OnRemoveSeparation);
//    axEVENT_ACCESSOR(axButton::Msg, OnMinimize);
//    axEVENT(MultipleSliderMsg, OnVelocity);
//    axEVENT_ACCESSOR(axNumberBox::Msg, OnStandardDeviation);
    
    
private:
    DrumMidiTrack::Events _events;
//    MidiTrackName* _trackName;
    DrumMidiTrackSequence* _trackSeq;
    DrumMidiVelocity* _velocity;
//    MidiTrackEvents _evt;
//    AudioMidiSeq* _audio;
    
//    axEvtFunction(int) _btn_evt_fct;
    
    axButton *_addBtn, *_removeBtn;
    
    int _nSubTrack;
    
    int _track_number;
    
    // Events.
    virtual void OnPaint();

    axEVENT_ACCESSOR(axButton::Msg, OnAddSeparation);
    void OnAddSeparation(const axButton::Msg& msg);
    
    axEVENT_ACCESSOR(axButton::Msg, OnRemoveSeparation);
    void OnRemoveSeparation(const axButton::Msg& msg);
    
    axEVENT_ACCESSOR(axButton::Msg, OnMinimize);
    void OnMinimize(const axButton::Msg& msg);
    
//    void OnVelocity(const MultipleSliderMsg& msg);
    void OnStandardDeviation(const axNumberBox::Msg& msg);
    
};

#endif /* defined(__DrumNBass__DrumMidiTrack__) */
