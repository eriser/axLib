//
//  MidiSingleNoteEditor.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-09.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "MidiSingleNoteEditor.h"

MidiSingleNoteEditor::MidiSingleNoteEditor(axWindow* parent,
                                           const axRect& rect,
                                           const bool& black_note):
axPanel(parent, rect),
_blackNote(black_note),
_nTrack(1),
_highLightIndex(-1),
_selectedSeparationNumber(0),
_colorChoice(MIDI_CHOICE_RED)
{
    _choiceColors[MIDI_CHOICE_RED] = axColor(0.8, 0.0, 0.0);
    _choiceColors[MIDI_CHOICE_GREEN] = axColor(0.0, 0.8, 0.0);
    _choiceColors[MIDI_CHOICE_BLUE] = axColor(0.0, 0.0, 0.8);
    
     _notes.resize(16);
    _velocity = new DrumMidiVelocity(this, axRect(0, rect.size.y,
                                                  rect.size.x, 40),
                                     DrumMidiVelocity::Events());
    _velocity->Hide();
}

void MidiSingleNoteEditor::AddTrack()
{
    axSize size(GetSize());
    size.y += 10;
    SetSize(size);
    
    _nTrack++;
    
    _velocity->SetNumberOfSlider(_nTrack);
    
    if(_velocity->IsShown())
    {
        axSize size(GetSize());
        _velocity->SetPosition(axPoint(0, size.y - 40));
        
    }
}

void MidiSingleNoteEditor::SwitchVelocity()
{
    if(_velocity->IsShown())
    {
        axSize size(GetSize());
        size.y -= 40;
        SetSize(size);
        
        _velocity->Hide();
    }
    else
    {
        axSize size(GetSize());
        size.y += 40;
        SetSize(size);
        
        _velocity->SetPosition(axPoint(0, size.y - 40));
        _velocity->Show();
    }
}

void MidiSingleNoteEditor::RemoveTrack()
{
    axSize size(GetSize());
    
    if(_nTrack > 1)
    {
        size.y -= 10;
        SetSize(size);
        _nTrack--;
        _velocity->SetNumberOfSlider(_nTrack);
    }
    
    if(_velocity->IsShown())
    {
        axSize size(GetSize());
        _velocity->SetPosition(axPoint(0, size.y - 40));
    }
}

void MidiSingleNoteEditor::SetColorSelection(const MidiColorChoice& choice)
{
    _colorChoice = choice;
}

void MidiSingleNoteEditor::OnMouseMotion(const axPoint& mousePos)
{
    axPoint pos = mousePos - GetAbsoluteRect().position;
    axSize size(GetSize());
    
    if(_velocity->IsShown())
    {
        size.y -= 40;
    }
    
    int index = pos.x / double(size.x) * 16;
    int selected = (pos.y / double(size.y)) * _nTrack;
    
    
    if(index != _highLightIndex || selected != _selectedSeparationNumber)
    {
        _highLightIndex = index;
        _selectedSeparationNumber = selected;
        
        Update();
    }
}

void MidiSingleNoteEditor::OnMouseLeftDown(const axPoint& mousePos)
{
    if(_highLightIndex != -1)
    {
        // Note is already active, needs to be turn off.
        if(_notes[_highLightIndex].actives[_selectedSeparationNumber] == true)
        {
            _notes[_highLightIndex].actives[_selectedSeparationNumber] = false;
        }
        // Note needs to be turn on.
        else
        {
            MidiNoteParameter& note_param = _notes[_highLightIndex];
            note_param.actives[_selectedSeparationNumber] = true;
            note_param.colors[_selectedSeparationNumber] = _choiceColors[_colorChoice];
        }
        Update();
    }
}

void MidiSingleNoteEditor::OnMouseLeave()
{
    if(_highLightIndex != -1)
    {
        _highLightIndex = -1;
        Update();
    }
}

void MidiSingleNoteEditor::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    axColor hori_line_color;
    
    // Draw background rectangle.
    if(_blackNote)
    {
        gc->SetColor(axColor(0.75, 0.75, 0.75));
        hori_line_color = axColor(0.5, 0.5, 0.5);
    }
    else
    {
        gc->SetColor(axColor(0.85, 0.85, 0.85));
        hori_line_color = axColor(0.6, 0.6, 0.6);
    }
    gc->DrawRectangle(rect);
    
    
    // Draw horizontal lines.
    gc->SetColor(hori_line_color);
    for(int i = 0; i * 10 < rect.size.y; i++)
    {
        gc->DrawLine(axPoint(0, i * 10), axPoint(rect.size.x, i * 10));
    }
    
    
    // Draw vertical lines.
    gc->SetColor(axColor::axBlackColor);
    
    for(int i = 0; i < 16; i++)
    {
        int x_pos = ((i) / double(16)) * (rect.size.x);
        
        gc->DrawLine(axPoint(x_pos, 0), axPoint(x_pos, rect.size.y));
    }
    
    // Draw notes.
    axSize size(GetSize());
    if(_velocity->IsShown())
    {
        size.y -= 40;
    }
    
    for(int i = 0; i < 16; i++)
    {
        for (int k = 0; k < _nTrack; k++)
        {
            if (_notes[i].actives[k] == true)
            {
                float alpha = _notes[i].velocities[k] * 0.8 + 0.2;
                gc->SetColor(_notes[i].colors[k], alpha);
                
                int x_left = ((i) / double(16)) * (rect.size.x);
                int x_right = ((i+1) / double(16)) * (rect.size.x);
                int size_x = x_right - x_left -1;
                
                axRect selRect(x_left, 10 * k,
                               size_x, 10 - 1);

                gc->DrawRectangle(selRect);
            }
        }
    }

    
    // Draw highlight note.
    if(_highLightIndex != -1)
    {
        int x_left = ((_highLightIndex) / double(16)) * (rect.size.x);
        int x_right = ((_highLightIndex+1) / double(16)) * (rect.size.x);
        int size_x = x_right - x_left -1;

        axRect selRect(x_left, 10 * _selectedSeparationNumber,
                       size_x, 10 - 1);
        gc->SetColor(1.0, 1.0, 1.0, 0.2);
        gc->DrawRectangle(selRect);
    }

    
    // Draw main contour.
    gc->SetColor(axColor::axBlackColor);
    gc->DrawRectangleContour(rect);
}