//
//  KrakenSynthInstrumentGUI.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__KrakenSynthInstrumentGUI__
#define __DrumNBass__KrakenSynthInstrumentGUI__

#include "axLib.h"
#include "InstrumentGUI.h"

class KrakenSynthInstrumentGUI : public axPanel, public InstrumentGUI
{
public:
    KrakenSynthInstrumentGUI(axWindow* parent, const axRect& rect);
    
private:
    
    // axEvents.
    void OnPaint();
};

#endif /* defined(__DrumNBass__KrakenSynthInstrumentGUI__) */
