#include "AudioOscillator.h"
#include "axAudioUtils.h"
#include "axLib.h"

AudioOscillator::AudioOscillator():
_active(false), _oct(0), _semi(0), _fine(0.0), _kdb(1.0), _pan(0.5), _gain(0.0)
{
    _waveTable = new axAudioWaveTable();
    _waveTable->SetWaveformType(axAudioWaveTable::axWAVE_TYPE_SINE);
    
    _filter[0] = false;
    _filter[1] = false;
    _filter[2] = false;
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

void AudioOscillator::SetFilter(const int& index, const bool& active)
{
    if(index < 3)
    {
      _filter[index] = active;
    }
}

bool AudioOscillator::IsFilterActive(const int& index)
{
    if(index < 3)
    {
        return _filter[index];
    }
    
    return false;
}

void AudioOscillator::SetWaveform(const axAudioWaveTable::axWaveformType& type)
{
    _waveTable->SetWaveformType(type);
}

void AudioOscillator::SetMidiNote(const int& note, const int& velocity)
{
//    std::cout << "AudioOsc note : " << note << std::endl;
    int n = note + _oct + _semi;
    
    double freq = axAudioConstant::C0 * pow(axAudioConstant::SemiToneRatio, n);
    freq += _fine * pow(2.0, 12.0);
    
//    std::cout << "Freq : " << freq << std::endl;
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
    
    
//    *output++ = value;
//    *output++ = value;
        *output++ = value * sqrt(1.0 - _pan) * _gain;
        *output++ = value * sqrt(_pan) * _gain;
//    }
    
}