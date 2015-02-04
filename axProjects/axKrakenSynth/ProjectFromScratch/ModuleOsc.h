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
    ModuleOsc(axWindow* parent, const axRect& rect, const int& index);
    
private:
    axKnob* _knobGain, *_knobPan;
    axToggle* _waveformLed[4];
    axFont* _font;
    int _index;
    
    virtual void OnPaint();
    
    axEVENT_ACCESSOR(axKnob::Msg, OnKnobGainChange);
    void OnKnobGainChange(const axKnob::Msg& msg);
    
    axEVENT_ACCESSOR(axKnob::Msg, OnKnobPanChange);
    void OnKnobPanChange(const axKnob::Msg& msg);
    
    axEVENT_ACCESSOR(axNumberBox::Msg, OnOctaveChange);
    void OnOctaveChange(const axNumberBox::Msg& msg);
    
    axEVENT_ACCESSOR(axNumberBox::Msg, OnSemiToneChange);
    void OnSemiToneChange(const axNumberBox::Msg& msg);
    
    axEVENT_ACCESSOR(axToggle::Msg, OnWaveformChange);
    void OnWaveformChange(const axToggle::Msg& msg);
    
    axEVENT_ACCESSOR(axToggle::Msg, OnFilterChange);
    void OnFilterChange(const axToggle::Msg& msg);
    
    
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
