//
//  MidiNoteEditor.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-08.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "MidiNoteEditor.h"
#include "PianoRoll.h"
#include "MidiSingleNoteEditor.h"
#include "LineSelection.h"

MidiNoteEditor::MidiNoteEditor(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    axButton::Info redInfo(axColor(0.8, 0.0, 0.0, 0.3),
                           axColor(0.9, 0.0, 0.0),
                           axColor(0.8, 0.0, 0.0),
                           axColor(0.8, 0.0, 0.0),
                           axColor(0.0, 0.0, 0.0),
                           axColor(0.0, 0.0, 0.0));
    
    axButton::Info greenInfo(axColor(0.0, 0.8, 0.0, 0.3),
                             axColor(0.0, 0.9, 0.0),
                             axColor(0.0, 0.8, 0.0),
                             axColor(0.0, 0.8, 0.0),
                             axColor(0.0, 0.0, 0.0),
                             axColor(0.0, 0.0, 0.0));
    
    axButton::Info blueInfo(axColor(0.0, 0.0, 0.8, 0.3),
                            axColor(0.0, 0.0, 0.9),
                            axColor(0.0, 0.0, 0.8),
                            axColor(0.0, 0.0, 0.8),
                            axColor(0.0, 0.0, 0.0),
                            axColor(0.0, 0.0, 0.0));
    
    axButton* red = new axButton(this,
                                 axRect(0, 0, 13, 30),
                                 axButton::Events(GetOnRedChoice()),
                                 redInfo, "", "");
    
    axButton* green = new axButton(this,
                                   axRect(13, 0, 13, 30),
                                   axButton::Events(GetOnGreenChoice()),
                                   greenInfo, "", "");
    
    axButton* blue = new axButton(this,
                                  axRect(26, 0, 13, 30),
                                  axButton::Events(GetOnBlueChoice()),
                                  blueInfo, "", "");
    
    LineSelection* _lines =
        new LineSelection(this, axRect(39, 0, rect.size.x - 39, 30));
    
    PianoRoll* _piano =
        new PianoRoll(this, axRect(0, 30, 40, rect.size.y - 30),
                      PianoNote::Events(GetOnNoteChangeSize()));
    
    for(int i = 0; i * 10 < rect.size.y; i++)
    {
        bool black_note = false;
        
        int i_mod_12 = i % 12;
        switch(i_mod_12)
        {
            case 0:
            case 2:
            case 4:
            case 5:
            case 7:
            case 9:
            case 11:
            break;
                
            default:
                black_note = true;
        }

        MidiSingleNoteEditor* note =
        new MidiSingleNoteEditor(this, axRect(39, 30 + i * 10,
                                              rect.size.x - 39, 11),
                                 black_note);
        _notesEditor.push_back(note);
    }
    
}

void MidiNoteEditor::OnNoteChangeSize(const PianoNote::Msg& msg)
{
    // Switch velocity on/off.
    if(msg.GetOperation() == 3)
    {
        _notesEditor[msg.GetIndex()]->SwitchVelocity();
    }
    
    // Adding or removing track.
    else
    {
        if(msg.GetOperation() == 1)
        {
            _notesEditor[msg.GetIndex()]->AddTrack();
        }
        else
        {
            _notesEditor[msg.GetIndex()]->RemoveTrack();
        }
    }
    
    for(int i = 1; i < _notesEditor.size(); i++)
    {
        _notesEditor[i]->SetPosition(_notesEditor[i-1]->GetNextPosDown(-1));
    }
}

void MidiNoteEditor::OnRedChoice(const axButton::Msg& msg)
{
    for(auto& n : _notesEditor)
    {
        n->SetColorSelection(MIDI_CHOICE_RED);
    }
}

void MidiNoteEditor::OnGreenChoice(const axButton::Msg& msg)
{
    for(auto& n : _notesEditor)
    {
        n->SetColorSelection(MIDI_CHOICE_GREEN);
    }
}

void MidiNoteEditor::OnBlueChoice(const axButton::Msg& msg)
{
    for(auto& n : _notesEditor)
    {
        n->SetColorSelection(MIDI_CHOICE_BLUE);
    }
}

void MidiNoteEditor::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    
    gc->SetColor(axColor(0.3, 0.3, 0.3), 1.0);
    gc->DrawRectangle(rect);
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect);
}