//
//  MainMidiSequencer.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-08.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "MainMidiSequencer.h"
#include "LineSelection.h"

MainMidiSequencer::MainMidiSequencer(axWindow* parent,
                                     const axRect& rect):
axPanel(parent, rect)
{
    SetBlockDrawing(true);
    
    std::string app_path(axApp::GetAppPath());
    
    
    LineSelection* line = new LineSelection(this,
                                            axRect(80, 0, rect.size.x - 80, 30));
    
    
    _midiInstruments.push_back(new MidiInstrument(this,
                                                  axRect(0, 30, rect.size.x, 40),
                                                  MidiInstrument::Events(GetOnMidiInstrumentResize()),
                                                  "TName"));

    _midiInstruments.push_back(new MidiInstrument(this,
                                                  axRect(0, 70, rect.size.x, 40),
                                                  MidiInstrument::Events(GetOnMidiInstrumentResize()),
                                                  "TName"));

    
//    MidiNoteEditor* _midiNoteEditor = new MidiNoteEditor(this,
//                                                         axRect(0, rect.size.y - 300,
//                                                                rect.size.x, 300));
    

}

void MainMidiSequencer::OnMidiInstrumentResize(const MidiInstrument::Msg& msg)
{
    for(int i = 1; i < _midiInstruments.size(); i++)
    {
        _midiInstruments[i]->SetPosition(_midiInstruments[i-1]->GetNextPosDown(0));
    }
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
    
    gc->SetColor(axColor(0.0, 0.0, 0.0));
    gc->DrawRectangleContour(rect);
}