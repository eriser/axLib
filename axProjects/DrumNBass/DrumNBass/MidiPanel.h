//
//  MidiPanel.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-11.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__MidiPanel__
#define __DrumNBass__MidiPanel__

#include "axLib.h"
#include "MainMidiSequencer.h"
#include "MidiNoteEditor.h"

class MidiPanel: public axPanel
{
public:
    MidiPanel(axWindow* parent, const axRect& rect);
    
private:
    MainMidiSequencer* _mainMidiSequence;
    MidiNoteEditor* _midiNoteEditor;
//    axScrollBar* _notesEditorScrollBar;
    
    // axEvents.
    virtual void OnPaint();
};

#endif /* defined(__DrumNBass__MidiPanel__) */
