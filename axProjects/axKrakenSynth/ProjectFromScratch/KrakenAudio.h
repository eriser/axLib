#ifndef __KRAKEN_AUDIO__
#define __KRAKEN_AUDIO__

#include "axAudio.h"
#include "axAudioEnvelope.h"
#include "axAudioWaveTable.h"

#include "AudioOscillator.h"
#include "axAudioFilter.h"

class KrakenPolyVoice
{
public:
    KrakenPolyVoice();
    
    void ProcessChannel(int sampleFrames);
    
    double** GetProcessedBuffers();
    
    void TriggerNote(const int& note, const int& velocity);
    
    void SetOscillatorOnOff(const int& index, const bool& active);
    void SetOscillatorGain(const int& index, const double& gain);
    void SetOscillatorPan(const int& index, const double& pan);
    void SetOscillatorOctave(const int& index, const int& oct);
    void SetOscillatorSemiTone(const int& index, const int& semi);
    void SetOscillatorWaveform(const int& index,
                               const axAudioWaveTable::axWaveformType& type);
    void SetOscillatorOutputFilter(const int& index,
                                   const int& filter_index,
                                   const bool& active);
    
    void SetFilterFreq(const int& index, const double& freq);
    void SetFilterRes(const int& index, const double& res);
    void SetFilterGain(const int& index, const double& gain);
    
    
private:
//    axAudioWaveTable* _waveTable;
    std::vector<AudioOscillator*> _oscs;
    std::vector<axAudioFilter*> _filters;
//    std::vector<bool> _waveTablesActive;
    axAudioEnvelope* _env;
    
    double** _processBuffer;
    
};

class KrakenAudio: public axAudio
{
public:
    static KrakenAudio* GetInstance();
    
    void TriggerNote(const int& note, const int& velocity);
    
    // Oscillator.
    void SetOscillatorOnOff(const int& index, const bool& active);
    void SetOscillatorGain(const int& index, const double& gain);
    void SetOscillatorPan(const int& index, const double& pan);
    void SetOscillatorOctave(const int& index, const int& oct);
    void SetOscillatorWaveform(const int& index,
                               const axAudioWaveTable::axWaveformType& type);
    void SetOscillatorSemiTone(const int& index, const int& semi);
    void SetOscillatorOutputFilter(const int& index,
                                   const int& filter_index,
                                   const bool& active);
    
    void SetFilterFreq(const int& index, const double& freq);
    void SetFilterRes(const int& index, const double& res);
    void SetFilterGain(const int& index, const double& gain);
    
    
private:
    KrakenAudio();
    static KrakenAudio* _instance;
    
    virtual int CallbackAudio(const float* input,
                              float* output,
                              unsigned long frameCount);
    
    std::vector<KrakenPolyVoice*> _polyVoices;
    int _polyChannelIndex;
};

#endif // __KRAKEN_AUDIO__
