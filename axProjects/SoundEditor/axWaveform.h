//
//  axWaveform.h
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-14.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __MidiSequencer__axWaveform__
#define __MidiSequencer__axWaveform__

#include "axLib.h"

class axAudioBuffer;

class axWaveform : public axPanel
{
public:
    axWaveform(axApp* app,
                axWindow* parent,
                const axRect& rect);
    
    void SetAudioBuffer(axAudioBuffer* buffer);
    
    void SetZoom(const double& zoom);
    
    void SetLeftPosition(const double& pos);
    
    axFloatRange GetBorders() const;
    
    std::vector<float>* _envBuffer;
    std::vector<axFloatPoint>* _envPoints;
    
    void ShowEnv(const bool& show);
    
private:
    axAudioBuffer* _audioBuffer;
    
    double _zoom, _leftPos;
    bool _showEnv;
    
    // Events.
    virtual void OnPaint();
};

#endif /* defined(__MidiSequencer__axWaveform__) */
