/*******************************************************************************//**
 * @file	axAudioBuffer.h
 * @author	Alexandre Arsenault <alexandre.arsenault@polymtl.ca>
 * @date	13/08/2013
 **********************************************************************************/

#ifndef DEF_AX_AUDIO_BUFFER_H
#define DEF_AX_AUDIO_BUFFER_H

#include "sndfile.h"
#include <iostream>

using namespace std;

typedef float axFloat;

enum axSOUND_ERROR
{
    axNO_ERROR = 0,
    axOPEN_SND_ERROR
};

struct axBufferInfo
{
    unsigned long frames;
    int sample_rate,
        channels;
};

//typedef struct
//{
//  sf_count_t frames;
//  int samplerate, channels, format, sections, seekable;
//} SF_INFO ;

class axAudioBuffer
{
public:
    axAudioBuffer( const string& snd_path );

    axSOUND_ERROR OpenSoundFile( const string& snd_path );

    axFloat* GetBuffer();

    axBufferInfo GetBufferInfo();

    int GetSampleRate();

    int GetNumberChannels();

    string GetSoundPath();

    void ShowInfo();

private:
    // Libsndfile.
    SNDFILE* m_sndFile;
    SF_INFO* m_info;
    string m_path;
    sf_count_t m_count;

    axFloat *m_buffer, *m_start, *m_end;
};

#endif // DEF_AX_AUDIO_BUFFER_H.
