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

class MidiNoteEditorScrollPanel: public axPanel
{
public:
    /***************************************************************************
     * MidiNoteEditorScrollPanel::Msg.
     **************************************************************************/
    class Msg : public axMsg
    {
    public:
        Msg(MidiNoteEditorScrollPanel* sender, const std::string& msg)
        {
            _sender = sender;
            _msg = msg;
        }
        
        MidiNoteEditorScrollPanel* GetSender() const
        {
            return _sender;
        }
        
        std::string GetMsg() const
        {
            return _msg;
        }
        
        axMsg* GetCopy()
        {
            return new MidiNoteEditorScrollPanel::Msg(*this);
        }
        
    private:
        MidiNoteEditorScrollPanel* _sender;
        std::string _msg;
    };
    
    /***************************************************************************
     * MidiNoteEditorScrollPanel::Events.
     **************************************************************************/
    class Events
    {
    public:
        enum : axEventId { SIZE_CHANGE };
        
        Events(){}
        Events(const axEventFunction& fct){ size_change = fct; }
        
        axEventFunction size_change;
    };
    
    MidiNoteEditorScrollPanel(axWindow* parent, const axRect& rect,
                              const MidiNoteEditorScrollPanel::Events& events);
    
    
    void SetChoiceColor(const MidiColorChoice& choice);
    
private:
    axRect _fixRect;
    
    std::vector<MidiSingleNoteEditor*> _notesEditor;
    
    axEVENT_ACCESSOR(PianoNote::Msg, OnNoteChangeSize);
    void OnNoteChangeSize(const PianoNote::Msg& msg);
    
    // axEvents.
    virtual void OnPaint();
};

class MidiNoteEditor: public axPanel
{
public:
    MidiNoteEditor(axWindow* parent, const axRect& rect);

private:
    MidiNoteEditorScrollPanel* _scrollPanel;
    axScrollBar* _notesEditorScrollBar;
    
    // Events.
    axEVENT_ACCESSOR(axButton::Msg, OnRedChoice);
    void OnRedChoice(const axButton::Msg& msg);
    
    axEVENT_ACCESSOR(axButton::Msg, OnGreenChoice);
    void OnGreenChoice(const axButton::Msg& msg);
    
    axEVENT_ACCESSOR(axButton::Msg, OnBlueChoice);
    void OnBlueChoice(const axButton::Msg& msg);
    
    axEVENT_ACCESSOR(MidiNoteEditorScrollPanel::Msg, OnNotesScrollPanelResize);
    void OnNotesScrollPanelResize(const MidiNoteEditorScrollPanel::Msg& msg);
    
    // axEvents.
    virtual void OnPaint();
};


#endif /* defined(__DrumNBass__MidiNoteEditor__) */
