//
//  MidiNoteEditor.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-08.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__MidiNoteEditor__
#define __DrumNBass__MidiNoteEditor__

#include "axLib.h"
#include "MidiSingleNoteEditor.h"
#include "PianoNote.h"

class MidiNoteEditor: public axPanel
{
public:
    MidiNoteEditor(axWindow* parent,
                   const axRect& rect);
    
private:
    std::vector<MidiSingleNoteEditor*> _notesEditor;
    //    axFont* _font;
    
    
    axEVENT_ACCESSOR(PianoNote::Msg, OnNoteChangeSize);
    void OnNoteChangeSize(const PianoNote::Msg& msg);
    
    axEVENT_ACCESSOR(axButton::Msg, OnRedChoice);
    void OnRedChoice(const axButton::Msg& msg);
    
    axEVENT_ACCESSOR(axButton::Msg, OnGreenChoice);
    void OnGreenChoice(const axButton::Msg& msg);
    
    axEVENT_ACCESSOR(axButton::Msg, OnBlueChoice);
    void OnBlueChoice(const axButton::Msg& msg);
    
    // axEvents.
    virtual void OnPaint();
};


#endif /* defined(__DrumNBass__MidiNoteEditor__) */
