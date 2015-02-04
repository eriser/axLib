#ifndef __AUDIO_OSCILLATOR__
#define __AUDIO_OSCILLATOR__

#include "axAudioWaveTable.h"

template<typename T>
struct AudioOscillatorModulation
{
    AudioOscillatorModulation():
    ampLfo(nullptr),
    panLfo(nullptr),
    freqLfo(nullptr),
    ampEnv(nullptr),
    panEnv(nullptr),
    freqEnv(nullptr)
    {
        
    }
    
    T* ampLfo;
    T* panLfo;
    T* freqLfo;
    
    T* ampEnv;
    T* panEnv;
    T* freqEnv;
};

class AudioOscillator
{
public:
    AudioOscillator();
    
    void SetActive(const bool& active);
    bool GetActive() const;
    void SetOctave(const int& oct);
    void SetSemi(const int& semi);
    void SetFine(const double& fine);
    void SetKeyboard(const double& kdb);
    void SetPan(const double& pan);
    void SetGain(const double& gain);
    void SetWaveform(const axAudioWaveTable::axWaveformType& type);
    
    void SetMidiNote(const int& note, const int& velocity);

    void Process(float* out);
    
    void SetFilter(const int& index, const bool& active);
    
    bool IsFilterActive(const int& index);
    
private:
    bool _filter[3];
    bool _active;
    int _oct, _semi;
    double _fine, _kdb, _pan, _gain;
    axAudioWaveTable* _waveTable;
    AudioOscillatorModulation<float> _mod;
    
};

#endif // __AUDIO_OSCILLATOR__