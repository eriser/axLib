#ifndef __ModuleLfo__
#define __ModuleLfo__

#include "axLib.h"
#include "ModuleFrame.h"

/*******************************************************************************
 * ModuleLfo.
 ******************************************************************************/
class ModuleLfo: public axPanel
{
public:
    ModuleLfo(axWindow* parent, const axRect& rect);
    
    
private:
    axToggle* _waveformLed[4];
};

/*******************************************************************************
 * Lfos.
 ******************************************************************************/
class Lfos: public ModuleFrame<ModuleLfo>
{
public:
    Lfos(axWindow* parent, const axPoint& pos);
};


#endif // __ModuleLfo__
