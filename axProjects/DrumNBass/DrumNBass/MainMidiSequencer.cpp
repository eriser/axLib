//
//  MainMidiSequencer.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-08.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "MainMidiSequencer.h"


MainMidiSequencer::MainMidiSequencer(axWindow* parent,
                                     const axRect& rect):
axPanel(parent, rect)
{
    SetBlockDrawing(true);
    
    std::string app_path(axApp::GetAppPath());
    
    MidiInstrument* _midiInstrument = new MidiInstrument(this,
                                                         axRect(0, 30,
                                                                rect.size.x, 40),
                                                         "TName");
    
    MidiNoteEditor* _midiNoteEditor = new MidiNoteEditor(this,
                                                         axRect(0, rect.size.y - 300,
                                                                rect.size.x, 300));
            
}

void MainMidiSequencer::OnResize()
{

}

void MainMidiSequencer::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    
    gc->SetColor(1.0, 1.0, 1.0);
    gc->DrawRectangle(rect);
    
    gc->SetColor(0.4, 0.4, 0.4);
    gc->DrawRectangle(axRect(80, 0, rect.size.x, 30));
    gc->SetColor(axColor(0.0, 0.0, 0.0));
    gc->DrawRectangleContour(axRect(80, 0, rect.size.x, 30));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0));
    gc->DrawRectangleContour(rect);
}