#include "AudioLfo.h"
#include "axAudioUtils.h"
#include "axLib.h"

AudioLfo::AudioLfo():
_active(false), _pan(0.5), _gain(0.0)
{
    _waveTable = new axAudioWaveTable();
    _waveTable->SetWaveformType(axAudioWaveTable::axWAVE_TYPE_SINE);
    
//    float* _processBuffer;
}

void AudioLfo::SetActive(const bool& active)
{
    _active = active;
}

bool AudioLfo::GetActive() const
{
    return _active;
}

void AudioLfo::SetPan(const double& pan)
{
    _pan = axClamp<double>(pan, 0.0, 1.0);
}

void AudioLfo::SetGain(const double& gain)
{
    _gain = axClamp<double>(gain, 0.0, 1.0);
}

void AudioLfo::SetFreq(const double& freq)
{
    _waveTable->SetFreq(axClamp<double>(freq, 0.1, 20.0));
}

void AudioLfo::SetWaveform(const axAudioWaveTable::axWaveformType& type)
{
    _waveTable->SetWaveformType(type);
}

void AudioLfo::Process(float* out)
{
    double value = 0.0;
    
    double freq = _waveTable->GetFreq();
    _waveTable->ProcessSample(&value, &freq);
    
    *out++ = value * sqrt(1.0 - _pan) * _gain;
    *out++ = value * sqrt(_pan) * _gain;
}