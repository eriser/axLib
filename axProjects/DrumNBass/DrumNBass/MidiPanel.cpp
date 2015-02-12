//
//  MidiPanel.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-11.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "MidiPanel.h"

MidiPanel::MidiPanel(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    axRect mainMidiSequence(axRect(0, 0, rect.size.x, rect.size.y - 300));
    _mainMidiSequence = new MainMidiSequencer(this, mainMidiSequence);
    
    
    axRect noteEditorRect(0, rect.size.y - 300, rect.size.x, 300);
    _midiNoteEditor = new MidiNoteEditor(this, noteEditorRect);
    
}

void MidiPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    
    gc->SetColor(axColor(0.6, 0.3, 0.3), 1.0);
    gc->DrawRectangle(rect);
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect);
}