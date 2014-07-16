#include "axAudio.h"

AudioTrack::AudioTrack(const string& sndfile, const int& samplePerBeat):
					   // Members.
					   // _notes(16, false),
					   _nFrameBuf(1410065408),
					   _beatIndex(0),
					   _sampleCount(0),
					   _nSamplePerBeat(samplePerBeat),
					   _nSection(2),
					   _selectedSection(0),
					   _currentVelocity(0.0)
{
	_buffer = new axAudioBuffer(sndfile);
	_outBuffer = new float[1024 * 2];

	for(int i = 0; i < 1024 * 2; i++)
	{
		_outBuffer[i] = 0.0;
	}

	for(int j = 0; j < 3; j++)
	{
		for(int i = 0; i < 16; i++)
		{
			_notes[j][i] = false;
			_probability[j][i] = 0.0;
			_velocity[j][i] = 0.0;
		}
	}

}

float* AudioTrack::Process()
{
 	axFloat* buffer = _buffer->GetBuffer();
 	axBufferInfo binfo =  _buffer->GetBufferInfo();

	for(int i = 0; i < 1024 * 2; i+=2)
    {
    	float out = 0.0;

    	if(_nFrameBuf < binfo.frames)
    	{
    		out = buffer[_nFrameBuf];
    		_nFrameBuf++;
    	}

    	if(_nFrameBuf > binfo.frames - 1)
    	{
    		_nFrameBuf = binfo.frames;
    	}


    	out *= _currentVelocity;

    	_outBuffer[i] = out;// * _velocity[_selectedSection][_beatIndex];
    	_outBuffer[i+1] = out;// * _velocity[_selectedSection][_beatIndex];

    	_sampleCount++;

    	if(_sampleCount > _nSamplePerBeat)
    	{
    		if(_notes[_selectedSection][_beatIndex] == true)
    		{
    			float r = rand() / double(RAND_MAX);

    			// cout << _probability[_selectedSection][_beatIndex] << endl;
    			if(_probability[_selectedSection][_beatIndex] >= r)
    			{
    				_nFrameBuf = 0;
    				_currentVelocity = _velocity[_selectedSection][_beatIndex];
    			}


    		}

    		_sampleCount = 0;
    		_beatIndex++;

    		if(_beatIndex > 16-1)
    		{
    			_beatIndex = 0;

    			_selectedSection = rand() % _nSection; 
    		}
    	}
    }

 	return _outBuffer;
}

//----------------------------------------------------
Audio::Audio()
{
    for(int i = 0; i < TABLE_SIZE; i++)
    {
        data.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
    }
    data.left_phase = data.right_phase = 0;

	err = Pa_Initialize();
	if( err != paNoError )
	{
		cerr << "Error." << endl;
		exit(1);
	}

	int numDevices;
	numDevices = Pa_GetDeviceCount();
	if( numDevices < 0 )
	{
	printf( "ERROR: Pa_CountDevices returned 0x%x\n", numDevices );
	// err = numDevices;
	// goto error;
	}

// If you want to get information about each device, simply loop through as follows:
	const PaDeviceInfo *deviceInfo;
	for( int i=0; i<numDevices; i++ )
	{
		deviceInfo = Pa_GetDeviceInfo( i );
		cout << "OUTPUT : " << i << " " << deviceInfo->name << endl;
	}


	outputParameters.device = 1;//Pa_GetDefaultOutputDevice();
	// cout << "OUTPU : " << outputParameters.device.name << endl;
	if(outputParameters.device == paNoDevice)
	{
		cerr << "Error." << endl;
		exit(1);
	}

	outputParameters.channelCount = 2;       /* stereo output */
	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
	outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
      &stream,
      NULL, /* no input */
      &outputParameters,
      44100,
      1024,
      paClipOff,      /* we won't output out of range samples so don't bother clipping them */
      myPaCallback,
      this);

	if( err != paNoError )
	{
		cerr << "Error." << endl;
		exit(1);
	}

   	// sprintf( data.message, "No Message" );
    // err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
    // if( err != paNoError ) goto error;
}

void Audio::StartAudio()
{
  	err = Pa_StartStream( stream );

	if( err != paNoError ) 
	{	
		cerr << "Error." << endl;
		exit(1);
	}
}