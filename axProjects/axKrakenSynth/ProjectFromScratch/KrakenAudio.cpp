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
    /// @todo Change this.
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
    
//    _oscs[0]->SetActive(true);

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


void KrakenPolyVoice::ProcessChannel(int sampleFrames)
{
    double* out1 = _processBuffer[0];
    double* out2 = _processBuffer[1];
    
    while (--sampleFrames >= 0)
    {
        float value[2] = {0.0, 0.0};
        
        for(int i = 0; i < _oscs.size(); i++)
        {
            if(_oscs[i]->GetActive())
            {
                float v[2] = {0.0, 0.0};
                _oscs[i]->Process(v);
                value[0] += v[0];
                value[1] += v[1];
            }
        }

        double env = _env->Process();

        *out1++ = value[0] * env;
        *out2++ = value[1] * env;
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

int KrakenAudio::CallbackAudio(const float* input,
                                float* output,
                                unsigned long frameCount)
{
    // Process all voices.
    for(int i = 0; i < 10; i++)
    {
        _polyVoices[i]->ProcessChannel(frameCount);
    }
    
    // Get poly channel buffers.
    double* left_voices[10];
    double* right_voices[10];
    for(int i = 0; i < 10; i++)
    {
        left_voices[i] = _polyVoices[i]->GetProcessedBuffers()[0];
        right_voices[i] = _polyVoices[i]->GetProcessedBuffers()[1];
    }

    // Output process loop..
    for(int i = 0; i < frameCount; i++)
    {
        float vl = 0.0, vr = 0.0;
        for(int n = 0; n < 10; n++)
        {
            double channelValue = left_voices[n][i];
            double channelValueRight = right_voices[n][i];
            
            vl += channelValue;
            vr += channelValueRight;
        }

        *output++ = vl;
        *output++ = vr;
    }

    return 0;
}