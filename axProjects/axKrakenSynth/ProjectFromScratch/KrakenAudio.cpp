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
//    _waveTablesActive.resize(5);
    for(int i = 0; i < _oscs.size(); i++)
    {
        _oscs[i] = new AudioOscillator();
        
//        _oscs[i] = new axAudioWaveTable();
//        _oscs[i]->SetWaveformType(axAudioWaveTable::axWAVE_TYPE_SAW);
//        _waveTablesActive[i] = false;
    }
    
    _oscs[0]->SetActive(true);

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
    double* voices[10];
    for(int i = 0; i < 10; i++)
    {
        voices[i] = _polyVoices[i]->GetProcessedBuffers()[0];
    }

    // Output process loop..
    for(int i = 0; i < frameCount; i++)
    {
        float v = 0.0;
        for(int n = 0; n < 10; n++)
        {
            double channelValue = voices[n][i];
            
            v = v + channelValue;
        }

        *output++ = v;
        *output++ = v;
    }

    return 0;
}