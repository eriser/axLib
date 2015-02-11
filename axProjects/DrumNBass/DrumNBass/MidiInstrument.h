//
//  MidiTrack.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-08.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__MidiTrack__
#define __DrumNBass__MidiTrack__

#include "axLib.h"

class MidiInstrument: public axPanel
{
public:
    MidiInstrument(axWindow* parent,
                   const axRect& rect,
                   const std::string& trackName);
    
private:

    axFont* _font;
    
    axEVENT_ACCESSOR(axButton::Msg, OnExpand);
    
    void OnExpand(const axButton::Msg& msg);
    
    // axEvents.
    virtual void OnPaint();
};

#endif /* defined(__DrumNBass__MidiTrack__) */
