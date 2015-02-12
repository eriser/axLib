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

MidiNoteEditorScrollPanel::
MidiNoteEditorScrollPanel(axWindow* parent,
                          const axRect& rect,
                          const MidiNoteEditorScrollPanel::Events& events):
axPanel(parent, rect)
{
    _fixRect = rect;
    _fixRect.position = axPoint(0, 0);
    
    SetBlockDrawing(true);
    SetShownRect(_fixRect);
    
    if(events.size_change)
    {
        axObject::AddConnection(events.SIZE_CHANGE, events.size_change);
    }
    
    PianoRoll* _piano =
    new PianoRoll(this, axRect(0, 0, 40, rect.size.y),
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
        new MidiSingleNoteEditor(this, axRect(39, i * 10,
                                              rect.size.x - 39, 11),
                                 black_note);
        _notesEditor.push_back(note);
    }
}

void MidiNoteEditorScrollPanel::SetChoiceColor(const MidiColorChoice& choice)
{
    for(auto& n : _notesEditor)
    {
        n->SetColorSelection(choice);
    }
}

void MidiNoteEditorScrollPanel::OnNoteChangeSize(const PianoNote::Msg& msg)
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

    SetSize(axSize(GetSize().x,
                   _notesEditor[_notesEditor.size() - 1]->GetNextPosDown(0).y));
    SetShownRect(_fixRect);
    
    axObject::PushEvent(Events::SIZE_CHANGE, new  Msg(this, ""));
}

void MidiNoteEditorScrollPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    
    gc->SetColor(axColor(0.3, 0.3, 0.3), 1.0);
    gc->DrawRectangle(rect);
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect);
}


MidiNoteEditor::MidiNoteEditor(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    SetBlockDrawing(true);
    
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
        new LineSelection(this, axRect(39, 0, rect.size.x - 39 - 8, 30));
    
    
    axRect scrollPanelRect(0, 30, rect.size.x - 8, rect.size.y - 30);
    _scrollPanel = new MidiNoteEditorScrollPanel(this, scrollPanelRect,
                                                 MidiNoteEditorScrollPanel::Events(GetOnNotesScrollPanelResize()));

    
    // Create midi notes eidtor scroll bar.
    axRect notesEditorSBarRect(rect.size.x - 8, 30,
                               8, rect.size.y - 30);
    
    axScrollBarInfo notesSBarInfo(axColor(0.6, 0.6, 0.6),
                                  axColor(0.65, 0.65, 0.65),
                                  axColor(0.62, 0.62, 0.62),
                                  axColor(0.0, 0.0, 0.0),
                                  axColor(0.0, 0.0, 0.0),
                                  axColor(0.8, 0.8, 0.8),
                                  axColor(0.9, 0.9, 0.9));
    
    _notesEditorScrollBar = new axScrollBar(this, _scrollPanel,
                                            notesEditorSBarRect,
                                            axScrollBarEvents(),
                                            notesSBarInfo);
    
    // Assign main scroll panel to scroll bar.
    _notesEditorScrollBar->SetPanelSize(_scrollPanel->GetSize());
    
  
}

void MidiNoteEditor::OnRedChoice(const axButton::Msg& msg)
{
    _scrollPanel->SetChoiceColor(MIDI_CHOICE_RED);
}

void MidiNoteEditor::OnGreenChoice(const axButton::Msg& msg)
{
    _scrollPanel->SetChoiceColor(MIDI_CHOICE_GREEN);
}

void MidiNoteEditor::OnBlueChoice(const axButton::Msg& msg)
{
    _scrollPanel->SetChoiceColor(MIDI_CHOICE_BLUE);
}

void MidiNoteEditor::
OnNotesScrollPanelResize(const MidiNoteEditorScrollPanel::Msg& msg)
{
    axSize size(_scrollPanel->GetSize());
    std::cout << "Scroll resize : " << size.x << " " << size.y << std::endl;
    _notesEditorScrollBar->SetPanelSize(_scrollPanel->GetSize());
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