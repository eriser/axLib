#include "axAudioBuffer.h"
#include <limits>
//std::mutex axAudioBuffer::audio_buffer_mutex;

axAudioBuffer::axAudioBuffer(const string& snd_path )
{
//    audio_buffer_mutex.lock();
    m_buffer = NULL;

    
    switch ( OpenSoundFile( snd_path ) )
    {
        case axOPEN_SND_ERROR:
            cout << "Error : Can't open audio file." << endl;
            break;

        case axNO_ERROR : break;
    }
//    audio_buffer_mutex.unlock();
}


struct header
{
    char chunk_id[4];
    int chunk_size;
    char format[4];
    char subchunk1_id[4];
    int subchunk1_size;
    short int audio_format;
    short int num_channels;
    int sample_rate; // sample_rate denotes the sampling rate.
    int byte_rate;
    short int block_align;
    short int bits_per_sample;
    char subchunk2_id[4];
    int subchunk2_size; // subchunk2_size denotes the number of samples.
};


axAudioBuffer::axAudioBuffer(const std::string& path, const int& test)
{
    m_path = path;
    FILE* infile = fopen(path.c_str(),"rb");     // Open wave file in read mode
    header* meta = (header*)malloc(sizeof(header));   // header_p points to a header struct that contains the wave file metadata fields
    m_buffer = nullptr;
    if (infile)
    {
        fread(meta, 1, sizeof(header), infile);
        
//        cout << " Size of Header file is "<<sizeof(*meta)<<" bytes" << endl;
//        
//        cout << " Sampling rate of the input wave file is "<< meta->sample_rate <<" Hz" << endl;
//        
//        cout << " Number of samples in wave file are " << meta->subchunk2_size << " samples" << endl;
//        
//        cout << " The number of channels of the file is "<< meta->num_channels << " channels" << endl;
//        
//        cout << "Bit per sample " << meta->bits_per_sample << " -- " << sizeof(short int) << std::endl;
//        
        _info.frames = meta->subchunk2_size / sizeof(short int);
        _info.sample_rate = meta->sample_rate;
        _info.channels = meta->num_channels;
        
        short int* tmp = new short int[_info.frames];
        
        fread(tmp, _info.frames, sizeof(short int), infile);
        
        m_buffer = new float[_info.frames];
        m_start = m_buffer;
        
        for(int i = 0; i < _info.frames; i++)
        {
            m_buffer[i] = 2.0 * tmp[i] / float(std::numeric_limits<short int>::max());
//            std::cout << "Value : " << m_buffer[i] << std::endl;
        }
        
        delete[] tmp;
        delete meta;
    }
    
    fclose(infile);
}


axSOUND_ERROR axAudioBuffer::OpenSoundFile( const string& snd_path )
{
    m_info = new SF_INFO;
    m_path = snd_path;

    if( !( m_sndFile = sf_open( m_path.c_str(), SFM_READ, m_info ) ) )
    {
        std::cerr << "Error : Can't open audio file." << std::endl;
        return axOPEN_SND_ERROR;
    }

    if( m_buffer )
    {
        delete m_buffer;
        m_buffer = nullptr;
    }

    m_buffer = new axFloat[m_info->frames * m_info->channels];

    sf_count_t numFrames = sf_readf_float(m_sndFile, m_buffer, m_info->frames);
    
    if(numFrames == 0)
    {
        std::cerr << "Error : Can't copy audio file, frame number is null." << std::endl;
    }

    m_start = m_buffer;
    m_end = &m_buffer[ m_info->frames * m_info->channels - 1 ];

    // Returns 0 on success, or an error number.
    int close_error = sf_close(m_sndFile);
    
    if(close_error != 0)
    {
        std::cerr << "Error : Can't close audio file." << std::endl;
    }

    _info.frames = m_info->frames;
    _info.sample_rate = m_info->samplerate;
    _info.channels = m_info->channels;
    
    return axNO_ERROR;
}

axBufferInfo axAudioBuffer::GetBufferInfo()
{
    return _info;
//    axBufferInfo info;
//    info.frames = m_info->frames;
//    info.channels = m_info->channels;
//    info.sample_rate = m_info->samplerate;
//
//    return info;
}

axFloat* axAudioBuffer::GetBuffer()
{
    return m_buffer;
}

int axAudioBuffer::GetSampleRate()
{
//    return m_info->samplerate;
    return _info.sample_rate;
}

int axAudioBuffer::GetNumberChannels()
{
//    return m_info->channels;
    return _info.channels;
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
