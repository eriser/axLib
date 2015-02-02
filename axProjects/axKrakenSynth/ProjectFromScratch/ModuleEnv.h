#ifndef __ModuleEnv__
#define __ModuleEnv__

#include "axLib.h"
#include "ModuleFrame.h"

/*******************************************************************************
 * ModuleEnv.
 ******************************************************************************/
class ModuleEnv: public axPanel
{
public:
    ModuleEnv(axWindow* parent, const axRect& rect);
};

/*******************************************************************************
 * Envelopes.
 ******************************************************************************/
class Envelopes: public ModuleFrame<ModuleEnv>
{
public:
    Envelopes(axWindow* parent, const axPoint& pos);
};


#endif // __ModuleEnv__
