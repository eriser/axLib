#ifndef __MIDI_SEUENCER__
#define __MIDI_SEUENCER__

#include "axLib.h"
#include "SoundEditorAudio.h"
#include "EnvelopeEditor.h"

class axWaveform;
class axWaveformNavigator;
class axToolBar;
class axVolumeMeter;


class SoundEditor: public axPanel
{
public:
	SoundEditor(axApp* app,
                axWindow* parent,
                const axRect& rect);
    
    void SetPlayAudioEvent(axEvtFunction(int) fct);
    void SetChangePathAudioEvent(axEvtFunction(std::string) fct);
    
    axEVENT(axButtonMsg, OnPlayButton);
    axEVENT(axButtonMsg, OnTimerButton);
    axEVENT(axToggleMsg, OnEnvToggle);
    axEVENT(axButtonMsg, OnOpenDialog);
    axEVENT(axSliderMsg, OnZoomValue);
    axEVENT(axSliderMsg, OnSamplePosition);
    axEVENT(double, OnWaveformNavigator);
    axEVENT(axAudioPlayerMsg, OnPlayingPositionChange);
    
    axEVENT(EnvelopeEditorMsg, OnEnvelopeChange);
private:
    
    axToolBar* _toolbar;
    axWaveform* _waveform;
    axWaveformNavigator* _waveformNavig;
    axSlider* _zoomSlider;
    axVolumeMeter *_volumeMeterRight, *_volumeMeterLeft;
    EnvelopeEditor* _envEditor;
    bool _showEnv;
    std::vector<float> _envBuffer;
    
	// Events.
	virtual void OnPaint();
    virtual void OnResize();
    void OnPlayButton(const axButtonMsg& msg);
    void OnTimerButton(const axButtonMsg& msg);
    void OnEnvToggle(const axToggleMsg& msg);
    void OnOpenDialog(const axButtonMsg& msg);
    void OnZoomValue(const axSliderMsg& msg);
    void OnSamplePosition(const axSliderMsg& msg);
    void OnWaveformNavigator(const double&  left_border);
    void OnPlayingPositionChange(const axAudioPlayerMsg& position);
    
    void OnEnvelopeChange(const EnvelopeEditorMsg& msg);
    
    axEvtFunction(int) _evtPlayAudio;
    axEvtFunction(std::string) _evtChangePathAudio;
};


class axMain
{
public:
    axMain();
    static void MainEntryPoint(axApp* app);
};


#endif // __MIDI_SEUENCER__