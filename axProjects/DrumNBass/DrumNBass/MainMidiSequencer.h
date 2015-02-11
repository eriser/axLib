//
//  MainMidiSequencer.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-08.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__MainMidiSequencer__
#define __DrumNBass__MainMidiSequencer__

#include "axLib.h"
#include "MidiInstrument.h"
#include "MidiNoteEditor.h"

class MainMidiSequencer : public axPanel
{
public:
    MainMidiSequencer(axWindow* parent, const axRect& rect);
    
    virtual void OnResize();
    
private:    
    // axEvents.
    void OnPaint();
    
};

#endif /* defined(__DrumNBass__MainMidiSequencer__) */
