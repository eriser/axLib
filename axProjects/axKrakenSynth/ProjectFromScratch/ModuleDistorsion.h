#ifndef __ProjectFromScratch__ModuleDistorsion__
#define __ProjectFromScratch__ModuleDistorsion__

#include "axLib.h"
#include "ModuleFrame.h"

/*******************************************************************************
 * ModuleDistorsion.
 ******************************************************************************/
class ModuleDistorsion: public axPanel
{
public:
    ModuleDistorsion(axWindow* parent, const axRect& rect);
};

/*******************************************************************************
 * Lfos.
 ******************************************************************************/
class Distorsions: public ModuleFrame<ModuleDistorsion>
{
public:
    Distorsions(axWindow* parent, const axPoint& pos);
};

#endif /* defined(__ProjectFromScratch__ModuleDistorsion__) */
