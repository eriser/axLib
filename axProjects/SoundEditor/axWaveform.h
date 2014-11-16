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
    
private:
    axAudioBuffer* _audioBuffer;
    
    double _zoom, _leftPos;
    
    // Events.
    virtual void OnPaint();
};

#endif /* defined(__MidiSequencer__axWaveform__) */
