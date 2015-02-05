#ifndef __AUDIO_LFO__
#define __AUDIO_LFO__

#include "axAudioWaveTable.h"

class AudioLfo
{
public:
    AudioLfo();
    
    void SetActive(const bool& active);
    bool GetActive() const;
    void SetPan(const double& pan);
    void SetGain(const double& gain);
    void SetWaveform(const axAudioWaveTable::axWaveformType& type);
    
    void SetFreq(const double& freq);
    
    void Process(float* out);
    
private:
    bool _active;
    double _pan, _gain;
    axAudioWaveTable* _waveTable;
    
//    float* _processBuffer;
};

#endif // __AUDIO_LFO__