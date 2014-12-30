//
//  axBufferPlayer.cpp
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-14.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "axAudioWaveTable.h"
//#include "axAudioBuffer.h"
#include "axAudioUtils.h"
#include <cmath>
#include "axUtils.h"

axAudioWaveTable::axAudioWaveTable():
_cPhase(0.0)
{
    SetWaveformType(axWAVE_TYPE_SINE);
}

//axAudioWaveTable::axAudioWaveTable(axAudioBuffer* buffer)
//{
//    
//}

void axAudioWaveTable::SetWaveformType(const axWaveformType& type)
{
    double buffer_size = axBUFFER_SIZE;
    double one_over_buffer_size = 1.0 / buffer_size;

    switch(type)
    {
        case axWAVE_TYPE_SINE:
        {
            for(int i = 0; i < axBUFFER_SIZE + 1; i++)
            {
                _data[i] = sin(i * 2.0 * M_PI * one_over_buffer_size);
            }
        } break;
            
        case axWAVE_TYPE_TRIANGLE:
        {
            for(int i = 0; i < axBUFFER_SIZE; i++)
            {
                if(2 * i >= buffer_size)
                {
                    _data[i] = 1 - 2.0 * i * one_over_buffer_size;
                }
                
                else
                {
                    _data[i] = 2.0 * i * one_over_buffer_size;
                }
            }
            _data[axBUFFER_SIZE] = _data[0];
            
        } break;
            
        case axWAVE_TYPE_SQUARE:
        {
            for(int i = 0; i <  axBUFFER_SIZE + 1; i++)
            {
                _data[i] = sin(i * 2.0 * M_PI * one_over_buffer_size);
                
                if(_data[i] > 0)
                {
                    _data[i] = 1;
                }
                
                if(_data[i] < 0)
                {
                    _data[i] = -1;
                }
            }
        } break;
            
        case axWAVE_TYPE_SAW:
        {
            for(int i = 0; i < axBUFFER_SIZE; i++)
            {
                _data[i] = 1.0 - ( i * one_over_buffer_size );
            }
            
            _data[axBUFFER_SIZE] = _data[0];
            
        } break;
    }

}

void axAudioWaveTable::SetFreq(const double& freq)
{
    _freq = freq;
}

double axAudioWaveTable::WaveInterpole(const double& freq,
                                       const unsigned int& len,
                                       const double& phase)
{
    double two_pi = 2.0 * M_PI;
    
    //PHASOR --> A CHANGER ( ENLEVER FMOD )
    double v = 1.0 - _cPhase * axINVTWOPI;
    _cPhase += ((two_pi / 44100.0) * (_freq + phase));
    
    if(_cPhase > two_pi)
    {
        _cPhase = fmod(_cPhase, two_pi);
    }
    if(_cPhase < 0.0)
    {
        _cPhase = fmod(_cPhase, two_pi) * (-1.0);
    }
    
    //WAVETABLE
    long pos = v * ((double)len - 1.0);
    double frac = v * (double)len - pos;
    
    return axLineairInterpole<double>(_data[pos], _data[pos + 1], frac);
}

void axAudioWaveTable::ProcessSample(float* out)
{
    *out = WaveInterpole(200.0, axBUFFER_SIZE, 0);
}

void axAudioWaveTable::ProcessSample(double* out)
{
    *out = WaveInterpole(200.0, axBUFFER_SIZE, 0);
}

void axAudioWaveTable::ProcessBlock(float* out, unsigned long frameCount)
{
    for(int i = 0; i < frameCount; i++)
    {
        double v = WaveInterpole(200.0, axBUFFER_SIZE, 0);

        *out++ = v;
        *out++ = v;
    }
}