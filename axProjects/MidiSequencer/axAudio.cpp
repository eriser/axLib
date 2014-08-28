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
	_deviation = 0.0;
	_gain = 0.5;
	_speed = 1.0;

	/// @todo Need to set proper frame buffer size.
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
			_velocity[j][i] = 0.7;
		}
	}

	_filter = new axAudioFilter();
	_env = new axAudioEnvelope();
	
    

    // rd = random_device();
    // gen = std::mt19937(rd());
 
    // values near the mean are the most likely
    // standard deviation affects the dispersion of generated values from the mean
    // std::normal_distribution<> d(5,2);
 
    // std::map<int, int> hist;
    // for(int n=0; n<10000; ++n) {
    //     ++hist[std::round(d(gen))];

}

void AudioTrack::SetMidiNoteOn()
{
	_midiNoteOn = true;
}

float* AudioTrack::Process()
{
 	axFloat* buffer = _buffer->GetBuffer();
 	axBufferInfo binfo =  _buffer->GetBufferInfo();
	//axFloat speed = 0.5;



	for(int i = 0; i < 1024 * 2; i+=2)
    {
    	float out = 0.0;

    	if(_nFrameBuf < binfo.frames)
    	{
			long pos = _nFrameBuf;
			axFloat frac = _nFrameBuf - pos;
			out = LINE_INTERPOLE(buffer[pos], buffer[pos+1], frac);
    		//out = buffer[int(floor(_nFrameBuf))];
    		//_nFrameBuf++;
			_nFrameBuf += _speed;
    	}

    	if(_nFrameBuf > binfo.frames - 1)
    	{
    		_nFrameBuf = binfo.frames;
    	}

    	out *= _currentVelocity;
		out *= _gain;

		out = _filter->Process(out);
		out *= _env->Process();

    	_outBuffer[i] = out;// * _velocity[_selectedSection][_beatIndex];
    	_outBuffer[i+1] = out;// * _velocity[_selectedSection][_beatIndex];

    	_sampleCount++;

		if (_midiNoteOn == true)
		{
			_nFrameBuf = 0;
			_currentVelocity = 0.7;
			_midiNoteOn = false;
			_env->TriggerNote();
		}

    	if(_sampleCount > _nSamplePerBeat)
    	{
			if (_notes[_selectedSection][_beatIndex] == true)
    		{
    			float r = rand() / double(RAND_MAX);

    			// cout << _probability[_selectedSection][_beatIndex] << endl;
    			if(_probability[_selectedSection][_beatIndex] >= r)
    			{
    				_nFrameBuf = 0;
					_env->TriggerNote();

					double v = _velocity[_selectedSection][_beatIndex];
					_currentVelocity = axNormalDistributionRandomGenerator(v, _deviation);
    	//			std::mt19937 gen(rd());
    	//			double v = _velocity[_selectedSection][_beatIndex];
    	//			std::normal_distribution<> d(v,_deviation);
					//_currentVelocity = 1.0;//d(gen);
    				// _currentVelocity = v;
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


	outputParameters.device = 4;//Pa_GetDefaultOutputDevice(); //1
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