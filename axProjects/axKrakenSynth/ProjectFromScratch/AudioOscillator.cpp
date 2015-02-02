#include "AudioOscillator.h"
#include "axAudioUtils.h"
#include "axLib.h"

AudioOscillator::AudioOscillator():
_active(false), _oct(0), _semi(0), _fine(0.0), _kdb(1.0), _pan(0.5), _gain(1.0)
{
    _waveTable = new axAudioWaveTable();
    _waveTable->SetWaveformType(axAudioWaveTable::axWAVE_TYPE_SAW);
}

void AudioOscillator::SetActive(const bool& active)
{
    _active = active;
}

bool AudioOscillator::GetActive() const
{
    return _active;
}

void AudioOscillator::SetOctave(const int& oct)
{
    int o = axClamp<int>(oct, -3, 3);
    _oct = o * 12;
}

void AudioOscillator::SetSemi(const int& semi)
{
    _semi = axClamp<int>(semi, -12, 12);
}

void AudioOscillator::SetFine(const double& fine)
{
    double f = axClamp<double>(fine, -1.0, 1.0);
    _fine = pow(2, f / 12.0);
}

void AudioOscillator::SetKeyboard(const double& kdb)
{
    _kdb = axClamp<double>(kdb, 0.0, 1.0);
}

void AudioOscillator::SetPan(const double& pan)
{
    _pan = axClamp<double>(pan, 0.0, 1.0);
}

void AudioOscillator::SetGain(const double& gain)
{
    _gain = axClamp<double>(gain, 0.0, 1.0);
}

void AudioOscillator::SetMidiNote(const int& note, const int& velocity)
{
    int n = note + _oct + _semi;
    
    double freq = axAudioConstant::C0 * pow(axAudioConstant::SemiToneRatio, n);
    freq *= _fine;
    
    _waveTable->SetFreq(freq);
}

void AudioOscillator::Process(float* output)
{
//    float* out1 = output[0];
//    float* out2 = output[1];
    
//    int frames = nFrames;
    double value = 0.0;
    
//    while(frames--)
//    {
        double freq = _waveTable->GetFreq();
        _waveTable->ProcessSample(&value, &freq);
        
        *output++ = value * sqrt(1.0 - _pan) * _gain;
        *output++ = value * sqrt(_pan) * _gain;
//    }
    
}