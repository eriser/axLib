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

void axAudioBufferPlayer::SetPlayingType(const axAudioBufferPlayingType& type)
{
    _playingType = type;
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
//        else if(nChan == 2)
//        {
//            ProcessStereoBlock(out, frameCount);
//        }
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
//    float* buffer = _sndBuffer->GetBuffer();
//    int nChannelBuffer = _sndBuffer->GetNumberChannels();
//    unsigned int buffer_pos = _bufferCurrentFrame;
//    float value = 0.0;
//    
//    for(int i = 0; i < frameCount; i++)
//    {
//        if(nChannelBuffer == 1)
//        {
//            value = buffer[buffer_pos];
//            buffer_pos++;
//            *out++ = value;
//            *out++ = value;
//            
//            if(buffer_pos >= _sndBuffer->GetBufferInfo().frames)
//            {
//                buffer_pos = 0;
//            }
//        }
//        else if(nChannelBuffer == 2)
//        {
//            *out++ = 0.0;
//            *out++ = 0.0;
//        }
//        else
//        {
//            *out++ = 0.0;
//            *out++ = 0.0;
//        }
//    }
//    
//    _bufferCurrentFrame = buffer_pos;
}