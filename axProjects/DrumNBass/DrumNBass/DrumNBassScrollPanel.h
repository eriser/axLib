//
//  DrumNBassScrollPanel.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__DrumNBassScrollPanel__
#define __DrumNBass__DrumNBassScrollPanel__

#include "axLib.h"

class MasterModuleGUI; // Define in MasterModuleGUI.h
class DrumInstrumentGUI; // Define in DrumInstrumentGUI.h
class MyProject;
class KrakenSynthInstrumentGUI;

class DrumNBassScrollPanel : public axPanel
{
public:
    DrumNBassScrollPanel(axWindow* parent, const axRect& rect);
    
    virtual void OnResize();
    
private:
    MasterModuleGUI* _masterModule;
    DrumInstrumentGUI* _drumInstrument;
    MyProject* _tb303GUI;
    KrakenSynthInstrumentGUI* _krakenSynth;
    
    // axEvents.
    void OnPaint();
    
};

#endif /* defined(__DrumNBass__DrumNBassScrollPanel__) */
