#ifndef __ModuleOsc__
#define __ModuleOsc__

#include "axLib.h"
#include "ModuleFrame.h"
#include "KrakenAudio.h"

/*******************************************************************************
 * ModuleOsc.
 ******************************************************************************/
class ModuleOsc: public axPanel
{
public:
    ModuleOsc(axWindow* parent, const axRect& rect);
    
    
private:
    axToggle* _waveformLed[4];
};

/*******************************************************************************
 * Oscillators.
 ******************************************************************************/
class Oscillators: public ModuleFrame<ModuleOsc>
{
public:
    Oscillators(axWindow* parent, const axPoint& pos);
};

#endif // __ModuleOsc__
