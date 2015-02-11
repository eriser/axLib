//
//  DrumSampler.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-06.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__DrumSampler__
#define __DrumNBass__DrumSampler__

#include "axLib.h"
#include <vector>
#include <string>
#include <cmath>

//#include "AudioDrumMachine.h"
//#include "AudioWaveform.h"
#include "axWaveform.h"

class SynthControl : public axPanel
{
public:
    SynthControl(axWindow* parent,
                 const axRect& rect,
                 const string& snd_path,
                 const int& track_num);
private:
    // Events.
    axImage* _bgImg;
//    AudioMidiSeq* _audio;
    int _trackNum;
    axWaveform* _wave;
    axFont* _font;
    
    axEVENT_ACCESSOR(axButton::Msg, OnOpenFile);
    void OnOpenFile(const axButton::Msg& msg);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnFilterFrequency);
    void OnFilterFrequency(const axKnob::Msg& msg);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnFilterRes);
    void OnFilterRes(const axKnob::Msg& msg);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnFilterEnvelope);
    void OnFilterEnvelope(const axKnob::Msg& msg);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnAttack);
    void OnAttack(const axKnob::Msg& msg);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnDecay);
    void OnDecay(const axKnob::Msg& msg);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnGain);
    void OnGain(const axKnob::Msg& msg);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnTuning);
    void OnTuning(const axKnob::Msg& msg);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnTuningEnv);
    void OnTuningEnv(const axKnob::Msg& msg);
    
    
    virtual void OnPaint();
    
    
};

class DrumPad;

class DrumPadMsg
{
public:
    DrumPadMsg()
    {
        _sender = nullptr;
    }
    
    DrumPadMsg(DrumPad* sender, const int& msg)
    {
        _sender = sender;
        _msg = msg;
    }
    
    DrumPad* GetSender() const
    {
        return _sender;
    }
    
    int GetMsg() const
    {
        return _msg;
    }
    
private:
    DrumPad* _sender;
    int _msg;
};

class DrumPad : public axPanel
{
public:
    DrumPad(axWindow* parent,
            const axRect& rect,
            const int& track_id);
    
    void SetTrackName(const string& name);
    void SetSelected();
    void SetUnselected();
    
    //axEVENT(axButtonMsg, OnOpenFile);
    
private:
    axImage* _bgImg;
    int _trackId;
    string _trackName;
    bool _highlight, _selected;
    axFont* _font;
    
//    axEvtFunction(DrumPadMsg) _clickEvent;
    // Events.
    
    //void OnOpenFile(const axButtonMsg& msg);
    virtual void OnMouseLeftDown(const axPoint& pos);
    virtual void OnMouseEnter();
    virtual void OnMouseLeave();
    virtual void OnPaint();
};

class DrumSampler : public axPanel
{
public:
    DrumSampler(axWindow* parent,
                const axRect& rect);
    
    //axEVENT(axButtonMsg, OnOpenFile);
    
    axEVENT(DrumPadMsg, OnDrumPadClick);
    
private:
//    AudioMidiSeq* _audio;
    vector<SynthControl*> _synths;
    vector<DrumPad*> _pads;
    
    axImage* _topBar;
    
    // Events.
    
    void OnDrumPadClick(const DrumPadMsg& msg);
    virtual void OnPaint();
};

#endif /* defined(__DrumNBass__DrumSampler__) */
