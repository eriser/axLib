//
//  PianoRoll.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-08.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__PianoRoll__
#define __DrumNBass__PianoRoll__

#include "axLib.h"
#include "PianoNote.h"

class PianoRoll: public axPanel
{
public:
    PianoRoll(axWindow* parent,
              const axRect& rect,
              const PianoNote::Events& events);
    
private:
    std::vector<PianoNote*> _notes;
    //    axFont* _font;
    
    // axEvents.
    virtual void OnPaint();
    virtual void OnResize();
    
    axEVENT_ACCESSOR(PianoNote::Msg, OnNoteChangeSize);
    void OnNoteChangeSize(const PianoNote::Msg& msg);
};

#endif /* defined(__DrumNBass__PianoRoll__) */
