//
//  EnvelopeEditor.h
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-18.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __MidiSequencer__EnvelopeEditor__
#define __MidiSequencer__EnvelopeEditor__

#include "axLib.h"
#include "SoundEditorAudio.h"

//class axWaveform;
//class axWaveformNavigator;
//class axToolBar;
//class axVolumeMeter;

class EnvelopeEditorMsg
{
public:
    EnvelopeEditorMsg(std::vector<float>* env, std::vector<axFloatPoint>* points)
    {
        _envBuffer = env;
        _envPoints = points;
    }
    
    std::vector<float>* GetBuffer() const
    {
        return _envBuffer;
    }
    
    std::vector<axFloatPoint>* GetPoints() const
    {
        return _envPoints;
    }
    
private:
    std::vector<float>* _envBuffer;
    std::vector<axFloatPoint>* _envPoints;
};

class EnvelopeEditor: public axPanel
{
public:
    EnvelopeEditor(axApp* app,
                   axWindow* parent,
                   const axRect& rect);
    
    void SetEnvChangeEvent(axEvtFunction(EnvelopeEditorMsg) fct);
    
    void ProcessEnvelope(const double& freq,
                         const double& q,
                         const double& gain);
    
    axEVENT(axSliderMsg, OnFilterValue);
    axEVENT(axSliderMsg, OnFilterQValue);
    axEVENT(axSliderMsg, OnFilterGainValue);
    
private:
    std::vector<float> _envBuffer;
    std::vector<axFloatPoint> _envPoints;
    
    

    // Events.
    virtual void OnPaint();
    void OnFilterValue(const axSliderMsg& msg);
    void OnFilterQValue(const axSliderMsg& msg);
    void OnFilterGainValue(const axSliderMsg& msg);
    
    axEvtFunction(EnvelopeEditorMsg) _envChangeEvt;
    
};

#endif /* defined(__MidiSequencer__EnvelopeEditor__) */
