#include "AudioDrumMachine.h"

AudioTrack::AudioTrack(const string& sndfile, const int& samplePerBeat):
					   // Members.
					   _nFrameBuf(1410065408),
					   _beatIndex(0),
					   _sampleCount(0),
					   _nSamplePerBeat(samplePerBeat),
					   _nSection(2),
					   _selectedSection(0),
					   _currentVelocity(0.0) //,
					   //rd(random_device())
					    
{
	//_inVstBuffers = new float*[2];
	//_outVstBuffers = new float*[2];
	//for (int i = 0; i < 2; i++)
	//{
	//	_inVstBuffers[i] = new float[1024];
	//	_outVstBuffers[i] = new float[1024];
	//}

	//_vst.LoadVstPlugin();
	//_vst.startPlugin();
	//int err _vst.configurePluginCallbacks();
	//_plugin = LoadVstPlugin();
	//int err = configurePluginCallbacks(_plugin);
	//startPlugin(_plugin);


	_buffer = new axAudioBuffer(sndfile);
	_outBuffer = new float[1024 * 2];
	_deviation = 0.0;
	_gain = 0.5;
	_speed = 1.0;
	_trackName = "";
	_modFilterFreqEnv = 0.0;
	_modFilterFreqEnvAmount = 0.0;

	double c5 = 220.0 * pow(ST_RATIO, 3);
	_c0 = c5 * pow(0.5, 5);
	_pitchEnvValue = 0;
	_pitchEnvAmount = 0;



	/// @todo Need to set proper frame buffer size from portaudio.
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

	// cout << "Filter set ptr" << endl;
	_filter->SetFreqEnvelopePtr(&_modFilterFreqEnv);
	_filter->SetFreqEnvelopeAmountPtr(&_modFilterFreqEnvAmount);
   
    gen = std::mt19937(rd());
 }

void AudioTrack::SetMidiNoteOn()
{
	_midiNoteOn = true;
}

void AudioTrack::SetBeat(const int& index, const bool& on)
{
	_notes[0][index] = on;
}

void AudioTrack::SetNote(const int& section,
	const int& index,
	const bool& on)
{
	_notes[section][index] = on;
}

void AudioTrack::SetProbability(const int& section,
	const int& index,
	const float& prob)
{
	_probability[section][index] = prob;
}

void AudioTrack::SetVelocity(const int& section,
	const int& index,
	const double& vel)
{
	_velocity[section][index] = vel;
}

void AudioTrack::SetStandardDeviation(const double& dev)
{
	_deviation = dev;
	cout << "Value of standard deviation." << dev << endl;
}

void AudioTrack::SetNumberOfSection(const int& nSection)
{
	_nSection = nSection;
}

void AudioTrack::SetPreset(TrackInfo* info)
{
	//cout << __PRETTY_FUNCTION__ << endl;
	_nSection = info->nSubTrack;

	cout << "INFO VALUE : " << info->nSubTrack << endl;
	int nSubTrack;

	for (int j = 0; j < MAX_NUM_OF_PROB_TRACK; j++)
	{
		for (int i = 0; i < NUMBER_OF_NOTES; i++)
		{
			_notes[j][i] = info->notes[j][i];
			_probability[j][i] = info->probability[j][i];
			_velocity[j][i] = info->velocity[j][i];
		}
	}
}

string AudioTrack::GetSoundPath()
{
	return _buffer->GetSoundPath();
}

axAudioBuffer* AudioTrack::GetAudioBuffer()
{
	return _buffer;
}

void AudioTrack::SetFilterFrequency(const axFloat& freq)
{
	_filter->SetFreq(freq);
}

void AudioTrack::SetFilterRes(const axFloat& q)
{
	_filter->SetQ(q);
}

void AudioTrack::SetAttack(const axFloat& value)
{
	_env->SetAttack(value);
}

void AudioTrack::SetDecay(const axFloat& value)
{
	_env->SetDecay(value);
}

void AudioTrack::SetGain(const axFloat& value)
{
	_gain = value;
}

void AudioTrack::SetSpeed(const axFloat& speed)
{
	axFloat s = speed;
	if (s < 0.01)
	{
		s = 0.01;
	}
	_speed = s;
}

string AudioTrack::GetTrackName() const
{
	return _trackName;
}

void AudioTrack::SetTrackName(const string& name)
{
	_trackName = name;
}

void AudioTrack::SetFilterFrequencyEnvelopeAmount(const axFloat& value)
{
	_modFilterFreqEnvAmount = value;
}

void AudioTrack::SetPitchEnvelopeAmount(const axFloat& value)
{
	_pitchEnvAmount = value;
}

float* AudioTrack::Process()
{
 	axFloat* buffer = _buffer->GetBuffer();
 	axBufferInfo binfo =  _buffer->GetBufferInfo();

	//int vstCount = 0;
	for(int i = 0; i < 1024 * 2; i+=2)
    {
    	float out = 0.0;
    	axFloat envValue = _env->Process();
    	_modFilterFreqEnv = envValue;
    	_pitchEnvValue = envValue;

    	axFloat speed = _speed;

    	if(_nFrameBuf < binfo.frames)
    	{
			// Change buffer playing speed.
			long pos = _nFrameBuf;
			axFloat frac = _nFrameBuf - pos;
			out = LINE_INTERPOLE(buffer[pos], buffer[pos+1], frac);

			// Pitch envelope.
			if(_pitchEnvAmount != 0.0)
			{
				axFloat modLfoPitch = _pitchEnvValue;
				speed = speed + 1.0 * _pitchEnvValue * _pitchEnvAmount;
			}

			_nFrameBuf += speed;
    	}

    	if(_nFrameBuf > binfo.frames - 1)
    	{
    		_nFrameBuf = binfo.frames;
    	}

		// Effects processing.
    	out *= _currentVelocity;
		out *= _gain;
		out = _filter->Process(out);
		out *= envValue;
		//out *= _modFilterFreqEnv;

    	_outBuffer[i] = out;
    	_outBuffer[i+1] = out;

		//_inVstBuffers[0][vstCount] = out;
		//_inVstBuffers[1][vstCount] = out;
		//_outVstBuffers[1][vstCount] = 0;
		//_outVstBuffers[1][vstCount] = 0;
		//vstCount++;


    	_sampleCount++;
		
		// Midi device note on.
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

    			if(_probability[_selectedSection][_beatIndex] >= r)
    			{
    				_nFrameBuf = 0;
					_env->TriggerNote();

					double v = _velocity[_selectedSection][_beatIndex];

					// The C++11 standard says the following (§26.5.8.5.1):
					// explicit normal_distribution(RealType mean = 0.0, RealType stddev = 1.0);
					// Requires: 0 < stddev.
					// http://cpp0x.centaur.ath.cx/rand.dist.norm.normal.html
					// As such a standard derivation of 0 is explicitly forbidden by the standard 
					// and therefore not guaranteed to work.
					if (_deviation > 0.0)
					{
						std::normal_distribution<> d(v, _deviation);
						_currentVelocity = d(gen);
					}
					else
					{
						_currentVelocity = v;
					}
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

	//_vst.processAudio(_inVstBuffers, _outVstBuffers, 1024);

	//int index = 0;
	//for (int i = 0; i < 1024 * 2; i+=2, index++)
	//{
	//	_outBuffer[i] = _outVstBuffers[0][index];
	//	_outBuffer[i+1] = _outVstBuffers[1][index];
	//}
 	return _outBuffer;
}

//----------------------------------------------------
//Audio::Audio()
//{
//    for(int i = 0; i < TABLE_SIZE; i++)
//    {
//        data.sine[i] = (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
//    }
//    data.left_phase = data.right_phase = 0;
//
//	err = Pa_Initialize();
//	if( err != paNoError )
//	{
//		cerr << "Error." << endl;
//		exit(1);
//	}
//
//	int numDevices;
//	numDevices = Pa_GetDeviceCount();
//	if( numDevices < 0 )
//	{
//	printf( "ERROR: Pa_CountDevices returned 0x%x\n", numDevices );
//	// err = numDevices;
//	// goto error;
//	}
//
//// If you want to get information about each device, simply loop through as follows:
//	const PaDeviceInfo *deviceInfo;
//	for( int i=0; i<numDevices; i++ )
//	{
//		deviceInfo = Pa_GetDeviceInfo( i );
//		cout << "OUTPUT : " << i << " " << deviceInfo->name << endl;
//	}
//
//#ifdef __linux__
//	outputParameters.device = 1;//1;//;//Pa_GetDefaultOutputDevice(); //1
//#elif _MSC_VER
//	outputParameters.device = 4;//1;//;//Pa_GetDefaultOutputDevice(); //1
//#endif
//    outputParameters.device = 2;
//	// cout << "OUTPU : " << outputParameters.device.name << endl;
//	if(outputParameters.device == paNoDevice)
//	{
//		cerr << "Error." << endl;
//		exit(1);
//	}
//
//	outputParameters.channelCount = 2;       /* stereo output */
//	outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
////	outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
//	outputParameters.hostApiSpecificStreamInfo = NULL;
//
//    err = Pa_OpenStream(
//      &stream,
//      NULL, /* no input */
//      &outputParameters,
//      44100,
//      1024,
//      paClipOff,      /* we won't output out of range samples so don't bother clipping them */
//      myPaCallback,
//      this);
//
//	if( err != paNoError )
//	{
//		cerr << "Error." << endl;
//		exit(1);
//	}
//
//   	// sprintf( data.message, "No Message" );
//    // err = Pa_SetStreamFinishedCallback( stream, &StreamFinished );
//    // if( err != paNoError ) goto error;
//}
//
//void Audio::StartAudio()
//{
//  	err = Pa_StartStream( stream );
//
//	if( err != paNoError ) 
//	{	
//		cerr << "Error." << endl;
//		exit(1);
//	}
//}