//
//  DrumInstrumentGUI.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__DrumInstrumentGUI__
#define __DrumNBass__DrumInstrumentGUI__

#include "axLib.h"
#include "InstrumentGUI.h"
#include "DrumSampler.h"

//class DrumMachine: public axPanel
//{
//public:
//    DrumMachine(axWindow* parent,
//                const axRect& rect, axAudio* audio);
//    
//    axEVENT(int, OnChangeTrackHeight);
//    axEVENT_ACCESSOR(axButton::Msg, OnChangeTemplate);
//    
//    void ExecApplication(const string& app_name);
//    
//    void SetPreset(const string& file_path);
//    
//private:
//    
//    
//    SynthControl* _synth;
//    MidiSequencer* _midiSeq;
//    MidiPartition* _midiPartition;
//    DrumSampler* _drumSampler;
//    axAudio* _audio;
//    //	axMidi* _midi;
//    axImage* _topBg;
//    
//    void OnChangeTrackHeight(const int& msg);
//    void OnChangeTemplate(const axButton::Msg& msg);
//    
//    
//    // axEvtFunction(int) _evt_track_size;
//    
//    
//    // Events.
//    virtual void OnPaint();
//};

//class DrumMidiSequencer;

class DrumInstrumentGUI : public axPanel, public InstrumentGUI
{
public:
    DrumInstrumentGUI(axWindow* parent, const axRect& rect);
    
private:
//    DrumMidiSequencer* _drumMidiSequencer;
    DrumSampler* _drumSampler;
    
    // axEvents.
    virtual void OnPaint();
    virtual void OnResize();
};

#endif /* defined(__DrumNBass__DrumInstrumentGUI__) */
