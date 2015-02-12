//
//  DrumNBassGUI.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__DrumNBassGUI__
#define __DrumNBass__DrumNBassGUI__

#include "axLib.h"

// Main panel define in DrumNBassScrollPanel.h
class DrumNBassScrollPanel;
//class MainMidiSequencer;
class MidiPanel;

class DrumNBassGUI : public axPanel
{
public:
    DrumNBassGUI();
    
private:
    axImage* _woodSideImg;
    axScrollBar* _scrollBar;
    DrumNBassScrollPanel* _mainScrollPanel;
//    MainMidiSequencer* _mainMidiSequencer;
    MidiPanel* _midiPanel;
    
    // axEvents.
    void OnPaint();
    virtual void OnResize();
    
    axEVENT_ACCESSOR(axButton::Msg, OnMidiPartClick);
    void OnMidiPartClick(const axButton::Msg& msg);
    
    axEVENT_ACCESSOR(axButton::Msg, OnSynthPartClick);
    void OnSynthPartClick(const axButton::Msg& msg);
};

#endif /* defined(__DrumNBass__DrumNBassGUI__) */
