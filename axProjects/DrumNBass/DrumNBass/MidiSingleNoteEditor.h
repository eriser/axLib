//
//  MidiSingleNoteEditor.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-09.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__MidiSingleNoteEditor__
#define __DrumNBass__MidiSingleNoteEditor__

#include "axLib.h"
#include "DrumMidiVelocity.h"

enum MidiColorChoice
{
    MIDI_CHOICE_RED,
    MIDI_CHOICE_GREEN,
    MIDI_CHOICE_BLUE,
    MIDI_CHOICE_NUM
};

struct MidiNoteParameter
{
    MidiNoteParameter():
    velocities(3, 0.7),
    actives(3, false),
    colors(3, axColor(0.8, 0.0, 0.0))
    {

    }
    
    std::vector<bool> actives;
    std::vector<double> velocities;
    std::vector<axColor> colors;
};

class MidiSingleNoteEditor: public axPanel
{
public:
    MidiSingleNoteEditor(axWindow* parent,
                   const axRect& rect, const bool& black_note);
    
    void AddTrack();
    void RemoveTrack();
    void SwitchVelocity();
    void SetColorSelection(const MidiColorChoice& choice);
    
private:
    bool _blackNote;
    int _nTrack;
    int _highLightIndex;
    int _selectedSeparationNumber;
    
    MidiColorChoice _colorChoice;
    axColor _choiceColors[MIDI_CHOICE_NUM];
    
    DrumMidiVelocity* _velocity;
    
    std::vector<MidiNoteParameter> _notes;

    // axEvents.
    virtual void OnPaint();
    virtual void OnMouseMotion(const axPoint& mousePos);
    virtual void OnMouseLeftDown(const axPoint& mousePos);
    virtual void OnMouseLeave();
};

#endif /* defined(__DrumNBass__MidiSingleNoteEditor__) */
