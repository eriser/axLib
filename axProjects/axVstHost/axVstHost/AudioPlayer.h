#ifndef __AudioApp__AudioPlayer__
#define __AudioApp__AudioPlayer__

#include <axLib/axLib.h>
#include <axAudio/axAudioBuffer.h>
#include "axWaveform.h"
#include "axWaveformNavigator.h"

class AudioPlayer : public axPanel
{
public:
    AudioPlayer(axWindow* parent, const axRect& rect);
    
private:
    axButton* _openBtn;
    axWaveform* _waveform;
    axWaveformNavigator* _waveformNavigator;
    
    axAudioBuffer* _audioBuffer;
    
    axEVENT_ACCESSOR(axButton::Msg, OnOpenAudioFile);
    void OnOpenAudioFile(const axButton::Msg& msg);
    
    axEVENT_ACCESSOR(axToggle::Msg, OnPlayAudioFile);
    void OnPlayAudioFile(const axToggle::Msg& msg);
  
    axEVENT_ACCESSOR(axSliderMsg, OnZoomValueChange);
    void OnZoomValueChange(const axSliderMsg& msg);
    
    axEVENT_ACCESSOR(axSliderMsg, OnNavigatorEvent);
    void OnNavigatorEvent(const axSliderMsg& msg);

    virtual void OnPaint();
};

#endif // __AudioApp__AudioPlayer__
