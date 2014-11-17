//
//  axBufferPlayer.cpp
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-14.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "axAudioBufferPlayer.h"
#include "axAudioBuffer.h"

axAudioBufferPlayer::axAudioBufferPlayer():
_bufferCurrentIndex(0),
_buffer(nullptr),
_bufferData(nullptr),
_playing(false),
_playingType(AUDIO_PLAYING_TYPE_PLAY_ONCE)
{
    
}

axAudioBufferPlayer::axAudioBufferPlayer(axAudioBuffer* buffer):
_bufferCurrentIndex(0),
_buffer(buffer),
_bufferData(_buffer->GetBuffer()),
_playing(false),
_playingType(AUDIO_PLAYING_TYPE_PLAY_ONCE)
{
    
}

void axAudioBufferPlayer::SetBuffer(axAudioBuffer* buffer)
{
    _bufferCurrentIndex = 0;
    _buffer = buffer;
    _bufferData = _buffer->GetBuffer();
    _playing = false;
}

void axAudioBufferPlayer::Play()
{
    _bufferCurrentIndex = 0;
    _playing = true;
}

bool axAudioBufferPlayer::IsPlaying() const
{
    return _playing;
}

void axAudioBufferPlayer::SetPlayingType(const axAudioBufferPlayingType& type)
{
    _playingType = type;
}

double axAudioBufferPlayer::GetCursorPercentPosition() const
{
    double percent_pos = double(_bufferCurrentIndex) /
    double(_buffer->GetBufferInfo().frames * _buffer->GetBufferInfo().channels);
    
//    std::cout << "Percent_pos : " << percent_pos << std::endl;
    return percent_pos;
}

void axAudioBufferPlayer::ProcessSample(float* out)
{
    
}

void axAudioBufferPlayer::ProcessBlock(float* out, unsigned long frameCount)
{
    if(_buffer != nullptr && _bufferData != nullptr && _playing)
    {
        int nChan = _buffer->GetBufferInfo().channels;
        if( nChan == 1)
        {
            ProcessMonoBlock(out, frameCount);
        }
        else if(nChan == 2)
        {
            ProcessStereoBlock(out, frameCount);
        }
        else
        {
            for(int i = 0; i < frameCount; i++)
            {
                *out++ = 0.0f;
                *out++ = 0.0f;
            }

        }
    }
    else
    {
        for(int i = 0; i < frameCount; i++)
        {
            *out++ = 0.0f;
            *out++ = 0.0f;
        }
        
    }
}

void axAudioBufferPlayer::ProcessMonoSample(float* out)
{
    
}

void axAudioBufferPlayer::ProcessMonoBlock(float* out,
                                           unsigned long frameCount)
{
    float* buf = _bufferData;
    unsigned long buffer_total_frame = _buffer->GetBufferInfo().frames;
    unsigned long index = _bufferCurrentIndex;
    float value = 0.0;
    
    if(index + frameCount < buffer_total_frame)
    {
        for(int i = 0; i < frameCount; i++)
        {
            value = buf[index++];
            *out++ = value;
            *out++ = value;
        }
    }
    else
    {
        for(int i = 0; i < frameCount; i++)
        {
            value = _playing ? buf[index++] : 0.0f;
            *out++ = value;
            *out++ = value;
            
            if(index >= buffer_total_frame)
            {
                if(_playingType == AUDIO_PLAYING_TYPE_PLAY_ONCE)
                {
                    _playing = false;
                }
                
                index = 0;
            }
        }
    }
    
    _bufferCurrentIndex = index;
}

void axAudioBufferPlayer::ProcessStereoSample(float* out)
{
    
}

void axAudioBufferPlayer::ProcessStereoBlock(float* out,
                                             unsigned long frameCount)
{
    float* buf = _bufferData;
    unsigned long buffer_total_frame = _buffer->GetBufferInfo().frames * 2;
    unsigned long stereo_index = _bufferCurrentIndex;
    
    if(stereo_index + frameCount * 2 < buffer_total_frame * 2)
    {
        for(int i = 0; i < frameCount; i++)
        {
            *out++ = buf[stereo_index++];
            *out++ = buf[stereo_index++];
        }
    }
    else
    {
        for(int i = 0; i < frameCount; i++)
        {
            *out++ = _playing ? buf[stereo_index++] : 0.0f;
            *out++ = _playing ? buf[stereo_index++] : 0.0f;
            
            if(stereo_index >= buffer_total_frame * 2)
            {
                if(_playingType == AUDIO_PLAYING_TYPE_PLAY_ONCE)
                {
                    _playing = false;
                }
                
                stereo_index = 0;
            }
        }
    }
    
    _bufferCurrentIndex = stereo_index;
}