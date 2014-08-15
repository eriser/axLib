#include "axAudioBuffer.h"

axAudioBuffer::axAudioBuffer( const string& snd_path )
{
    m_buffer = NULL;

    switch ( OpenSoundFile( snd_path ) )
    {
        case axOPEN_SND_ERROR:
            cout << "Error : Can't open audio file." << endl;
            break;

        case axNO_ERROR : break;
    }
}

axSOUND_ERROR axAudioBuffer::OpenSoundFile( const string& snd_path )
{
    m_info = new SF_INFO;
    m_path = snd_path;

    if( !( m_sndFile = sf_open( m_path.c_str(), SFM_READ, m_info ) ) )
    {
        return axOPEN_SND_ERROR;
    }

    if( m_buffer )
    {
        delete m_buffer;
        m_buffer = NULL;
    }

    m_buffer = new axFloat[ m_info->frames * m_info->channels ];

    /*unsigned int numFrames = */sf_readf_float( m_sndFile, m_buffer, m_info->frames );

    m_start = m_buffer;
    m_end = &m_buffer[ m_info->frames * m_info->channels - 1 ];

    sf_close( m_sndFile );

    return axNO_ERROR;
}

axBufferInfo axAudioBuffer::GetBufferInfo()
{
    axBufferInfo info;
    info.frames = m_info->frames;
    info.channels = m_info->channels;
    info.sample_rate = m_info->samplerate;

    return info;
}

axFloat* axAudioBuffer::GetBuffer()
{
    return m_buffer;
}

int axAudioBuffer::GetSampleRate()
{
    return m_info->samplerate;
}

int axAudioBuffer::GetNumberChannels()
{
    return m_info->channels;
}

string axAudioBuffer::GetSoundPath()
{
    return m_path;
}

void axAudioBuffer::ShowInfo()
{
 /*   cout << "Snd file  : " << m_path << endl;
    cout << "Nb Frames : " << m_info->frames << endl;
    cout << "Nb Chans  : " << m_info->channels << endl;
    cout << "SR        : " << m_info->samplerate << endl;*/
}
