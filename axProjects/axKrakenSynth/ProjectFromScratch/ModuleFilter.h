#ifndef __ModuleFilter__
#define __ModuleFilter__

#include "axLib.h"
#include "ModuleFrame.h"

/*******************************************************************************
 * ModuleFilter.
 ******************************************************************************/
class ModuleFilter: public axPanel
{
public:
    ModuleFilter(axWindow* parent, const axRect& rect);
};

/*******************************************************************************
 * Filters.
 ******************************************************************************/
class Filters: public ModuleFrame<ModuleFilter>
{
public:
    Filters(axWindow* parent, const axPoint& pos);
};


#endif // __ModuleFilter__
