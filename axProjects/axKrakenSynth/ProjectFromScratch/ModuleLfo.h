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
    ModuleLfo(axWindow* parent, const axRect& rect, const int& index);
    
    
private:
    axToggle* _waveformLed[4];
    int _index;
    axKnob* _gain, *_freq;
    
    axEVENT_ACCESSOR(axKnob::Msg, OnGainChange);
    void OnGainChange(const axKnob::Msg& msg);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnFreqChange);
    void OnFreqChange(const axKnob::Msg& msg);
    
    axEVENT_ACCESSOR(axToggle::Msg, OnWaveformChange);
    void OnWaveformChange(const axToggle::Msg& msg);
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
