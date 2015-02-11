#ifndef __ModuleAudioFile__
#define __ModuleAudioFile__

#include "axLib.h"
#include "ModuleFrame.h"

class axWaveform;
class axWaveformNavigator;

/*******************************************************************************
 * ModuleAudioFile.
 ******************************************************************************/
class ModuleAudioFile: public axPanel
{
public:
    ModuleAudioFile(axWindow* parent,
              const axRect& rect);
    
private:
//    axWaveform* _wave;
//    axWaveformNavigator* _navig;
    
    // Private event accessors.
    axEVENT_ACCESSOR(axSliderMsg, OnZoomValue);
    
    // Events.
    void OnZoomValue(const axSliderMsg& msg);
};

/*******************************************************************************
 * AudioFiles.
 ******************************************************************************/
class AudioFiles: public ModuleFrame<ModuleAudioFile>
{
public:
    AudioFiles(axWindow* parent, const axPoint& pos);
};

#endif // __ModuleAudioFile__
