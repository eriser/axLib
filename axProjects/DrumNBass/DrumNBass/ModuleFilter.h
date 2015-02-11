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
    ModuleFilter(axWindow* parent, const axRect& rect, const int& index);
    
private:
    
    axEVENT_ACCESSOR(axKnob::Msg, OnFreqChange);
    void OnFreqChange(const axKnob::Msg& msg);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnResChange);
    void OnResChange(const axKnob::Msg& msg);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnGainChange);
    void OnGainChange(const axKnob::Msg& msg);
    
    axKnob *_freqKnob, *_resKnob, *_gainKnob;
    int _index;
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
