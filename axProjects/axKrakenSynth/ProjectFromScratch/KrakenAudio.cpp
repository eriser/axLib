//
//  KrakenAudio.cpp
//  ProjectFromScratch
//
//  Created by Alexandre Arsenault on 2015-01-06.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "KrakenAudio.h"
#include "axLib.h"
#include "axAudioBuffer.h"

KrakenPolyVoice::KrakenPolyVoice()
{
    /// @todo Change this with right buffer size.
    _processBuffer = new double*[2];
    _processBuffer[0] = new double[8192];
    _processBuffer[1] = new double[8192];
    for(int i = 0; i < 8192; i++)
    {
        _processBuffer[0][i] = 0.0;
        _processBuffer[1][i] = 0.0;
    }

    _oscs.resize(5);
    for(int i = 0; i < _oscs.size(); i++)
    {
        _oscs[i] = new AudioOscillator();
        _oscs[i]->SetActive(true);
    }
    
    _filters.resize(3);
    for(int i = 0; i < _filters.size(); i++)
    {
        // Default values : freq=20000Hz q=0.707 gain=1.0.
        _filters[i] = new axAudioFilter();
    }
    
    _lfos.resize(5);
    for(int i = 0; i < _lfos.size(); i++)
    {
        _lfos[i] = new AudioLfo();
    }
    
    _env = new axAudioEnvelope();
    _env->SetAttack(0.001);
    _env->SetDecay(0.8);
    
}

double** KrakenPolyVoice::GetProcessedBuffers()
{
    return _processBuffer;
}

void KrakenPolyVoice::TriggerNote(const int& note, const int& velocity)
{
    for(auto& n : _oscs)
    {
        n->SetMidiNote(note, velocity);
    }
    _env->TriggerNote();
}


void KrakenPolyVoice::SetOscillatorOnOff(const int& index,
                                         const bool& active)
{
    if(index < _oscs.size())
    {
        _oscs[index]->SetActive(active);
    }
}

void KrakenPolyVoice::SetOscillatorGain(const int& index, const double& gain)
{
    if(index < _oscs.size())
    {
        _oscs[index]->SetGain(gain);
    }
}

void KrakenPolyVoice::SetOscillatorPan(const int& index, const double& pan)
{
    if(index < _oscs.size())
    {
        _oscs[index]->SetPan(pan);
    }
}


void KrakenPolyVoice::SetOscillatorOctave(const int& index, const int& oct)
{
    if(index < _oscs.size())
    {
        _oscs[index]->SetOctave(oct);
    }
}

void KrakenPolyVoice::SetOscillatorWaveform(const int& index,
                                        const axAudioWaveTable::axWaveformType& type)
{
    if(index < _oscs.size())
    {
        _oscs[index]->SetWaveform(type);
    }
}

void KrakenPolyVoice::SetOscillatorSemiTone(const int& index, const int& semi)
{
    if(index < _oscs.size())
    {
        _oscs[index]->SetSemi(semi);
    }
}

void KrakenPolyVoice::SetOscillatorOutputFilter(const int& index,
                                                const int& filter_index,
                                                const bool& active)
{
    if(index < _oscs.size())
    {
        _oscs[index]->SetFilter(filter_index, active);
    }
}

void KrakenPolyVoice::SetFilterFreq(const int& index, const double& freq)
{
    if(index < _filters.size())
    {
        _filters[index]->SetFreq(freq);
    }
}

void KrakenPolyVoice::SetFilterRes(const int& index, const double& res)
{
    if(index < _filters.size())
    {
        _filters[index]->SetQ(res);
    }
}

void KrakenPolyVoice::SetFilterGain(const int& index, const double& gain)
{
    if(index < _filters.size())
    {
        _filters[index]->SetGain(gain);
    }
}

void KrakenPolyVoice::SetLfoFreq(const int& index, const double& freq)
{
    if(index < _lfos.size())
    {
        _lfos[index]->SetFreq(freq);
    }
}

void KrakenPolyVoice::SetLfoGain(const int& index, const double& gain)
{
    if(index < _lfos.size())
    {
        _lfos[index]->SetGain(gain);
    }
}

void KrakenPolyVoice::SetLfoWaveform(const int& index,
                                            const axAudioWaveTable::axWaveformType& type)
{
    if(index < _lfos.size())
    {
        _lfos[index]->SetWaveform(type);
    }
}

void KrakenPolyVoice::ProcessChannel(int sampleFrames)
{
    double* out1 = _processBuffer[0];
    double* out2 = _processBuffer[1];
    
    while (--sampleFrames >= 0)
    {
        
        float lfo_values[5][2] = {{0.0, 0.0},
                                  {0.0, 0.0},
                                  {0.0, 0.0},
                                  {0.0, 0.0},
                                  {0.0, 0.0}};
        _lfos[0]->Process(lfo_values[0]);
        _lfos[1]->Process(lfo_values[1]);
        _lfos[2]->Process(lfo_values[2]);
        _lfos[3]->Process(lfo_values[3]);
        _lfos[4]->Process(lfo_values[4]);
        
        float filter_values[3][2] = {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}};
        
        for(int i = 0; i < _oscs.size(); i++)
        {
            if(_oscs[i]->GetActive())
            {
                // Process oscillator.
                float osc_value[2] = {0.0, 0.0};
                _oscs[i]->Process(osc_value);
                
                // Add filter's intput.
                bool filter_on[3] = {_oscs[i]->IsFilterActive(0),
                                     _oscs[i]->IsFilterActive(1),
                                     _oscs[i]->IsFilterActive(2)};
                
                filter_values[0][0] += filter_on[0] ? osc_value[0] : 0.0;
                filter_values[0][1] += filter_on[0] ? osc_value[1] : 0.0;
                
                filter_values[1][0] += filter_on[1] ? osc_value[0] : 0.0;
                filter_values[1][1] += filter_on[1] ? osc_value[1] : 0.0;
                
                filter_values[2][0] += filter_on[2] ? osc_value[0] : 0.0;
                filter_values[2][1] += filter_on[2] ? osc_value[1] : 0.0;
            }
        }

        float out_values[2] = {0.0, 0.0};
        // Process filters.
        _filters[0]->ProcessStereo(filter_values[0], out_values);
        _filters[1]->ProcessStereo(filter_values[1], out_values);
        _filters[2]->ProcessStereo(filter_values[2], out_values);
        
        double env = _env->Process();

        *out1++ = out_values[0] * env;
        *out2++ = out_values[1] * env;
    }
}



/*******************************************************************************
 * KrakenAudio.
 ******************************************************************************/
KrakenAudio* KrakenAudio::_instance = nullptr;

KrakenAudio* KrakenAudio::GetInstance()
{
    return _instance == nullptr ? _instance = new KrakenAudio() : _instance;
}

KrakenAudio::KrakenAudio():
axAudio()
{
    std::string app_path = axApp::GetInstance()->GetAppDirectory();
    
    _polyVoices.resize(10);
    for(int i = 0; i < 10; i++)
    {
        _polyVoices[i] = new KrakenPolyVoice();
    }
    
    _polyChannelIndex = 0;
}

void KrakenAudio::TriggerNote(const int& note, const int& velocity)
{
    _polyVoices[_polyChannelIndex]->TriggerNote(note, velocity);
    
    if(_polyChannelIndex + 1 == 10)
    {
        _polyChannelIndex = 0;
    }
    else
    {
        _polyChannelIndex++;
    }
}

void KrakenAudio::SetOscillatorOnOff(const int& index, const bool& active)
{
    for(auto& n : _polyVoices)
    {
        n->SetOscillatorOnOff(index, active);
    }
}

void KrakenAudio::SetOscillatorGain(const int& index, const double& gain)
{
    for(auto& n : _polyVoices)
    {
        n->SetOscillatorGain(index, gain);
    }
}

void KrakenAudio::SetOscillatorPan(const int& index, const double& pan)
{
    for(auto& n : _polyVoices)
    {
        n->SetOscillatorPan(index, pan);
    }
}

void KrakenAudio::SetOscillatorOctave(const int& index, const int& oct)
{
    for(auto& n : _polyVoices)
    {
        n->SetOscillatorOctave(index, oct);
    }
}

void KrakenAudio::SetOscillatorWaveform(const int& index,
                           const axAudioWaveTable::axWaveformType& type)
{
    for(auto& n : _polyVoices)
    {
        n->SetOscillatorWaveform(index, type);
    }
}

void KrakenAudio::SetOscillatorSemiTone(const int& index, const int& semi)
{
    for(auto& n : _polyVoices)
    {
        n->SetOscillatorSemiTone(index, semi);
    }
}

void KrakenAudio::SetOscillatorOutputFilter(const int& index,
                                            const int& filter_index,
                                            const bool& active)
{
    for(auto& n : _polyVoices)
    {
        n->SetOscillatorOutputFilter(index, filter_index, active);
    }
}

void KrakenAudio::SetFilterFreq(const int& index, const double& freq)
{
    for(auto& n : _polyVoices)
    {
        n->SetFilterFreq(index, freq);
    }
}

void KrakenAudio::SetFilterRes(const int& index, const double& res)
{
    for(auto& n : _polyVoices)
    {
        n->SetFilterRes(index, res);
    }
}

void KrakenAudio::SetFilterGain(const int& index, const double& gain)
{
    for(auto& n : _polyVoices)
    {
        n->SetFilterGain(index, gain);
    }
}

void KrakenAudio::SetLfoFreq(const int& index, const double& freq)
{
    for(auto& n : _polyVoices)
    {
        n->SetLfoFreq(index, freq);
    }
}

void KrakenAudio::SetLfoGain(const int& index, const double& gain)
{
    for(auto& n : _polyVoices)
    {
        n->SetLfoGain(index, gain);
    }
}

void KrakenAudio::SetLfoWaveform(const int& index,
                                 const axAudioWaveTable::axWaveformType& type)
{
    for(auto& n : _polyVoices)
    {
        n->SetLfoWaveform(index, type);
    }
}

int KrakenAudio::CallbackAudio(const float* input,
                               float* output,
                               unsigned long frameCount)
{
    //--------------------------------------------------------------------------
    // Multi thread version.
    //--------------------------------------------------------------------------
//    std::thread voices_thread[10] =
//    {
//        std::thread([&](){_polyVoices[0]->ProcessChannel(frameCount);}),
//        std::thread([&](){_polyVoices[1]->ProcessChannel(frameCount);}),
//        std::thread([&](){_polyVoices[2]->ProcessChannel(frameCount);}),
//        std::thread([&](){_polyVoices[3]->ProcessChannel(frameCount);}),
//        std::thread([&](){_polyVoices[4]->ProcessChannel(frameCount);}),
//        std::thread([&](){_polyVoices[5]->ProcessChannel(frameCount);}),
//        std::thread([&](){_polyVoices[6]->ProcessChannel(frameCount);}),
//        std::thread([&](){_polyVoices[7]->ProcessChannel(frameCount);}),
//        std::thread([&](){_polyVoices[8]->ProcessChannel(frameCount);}),
//        std::thread([&](){_polyVoices[9]->ProcessChannel(frameCount);}),
//    };
//
//    for(int i = 0; i < 10; i++)
//    {
//        voices_thread[i].join();
//    }
    
    //--------------------------------------------------------------------------
    // single thread version.
    //--------------------------------------------------------------------------
    // Process all voices.
    for(int i = 0; i < 10; i++)
    {
        _polyVoices[i]->ProcessChannel(frameCount);
    }
    
    // Get poly channel buffers.
    double* left_voices_buffer[10];
    double* right_voices_buffer[10];
    
    for(int i = 0; i < 10; i++)
    {
        left_voices_buffer[i] = _polyVoices[i]->GetProcessedBuffers()[0];
        right_voices_buffer[i] = _polyVoices[i]->GetProcessedBuffers()[1];
    }

    // Output process loop.
    // Add all voices to stereo output.
    for(int i = 0; i < frameCount; i++)
    {
        float value_out[2];
    
        for(int n = 0; n < 10; n++)
        {
            value_out[0] += left_voices_buffer[n][i];
            value_out[1] += right_voices_buffer[n][i];
        }

        *output++ = value_out[0] * 0.5;
        *output++ = value_out[1] * 0.5;
    }

    return 0;
}