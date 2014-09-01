#ifndef __AX_AUDIO__
#define __AX_AUDIO__

//------------------------------------------------------------------
// On linux : if you get this error:
// bt_audio_service_open: connect() failed: Connection refused (111)
// Solution : sudo apt-get purge bluez-als
//------------------------------------------------------------------

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include "axUtils.h"


#include "portaudio.h" 
#include "axAudioBuffer.h"
#include "axAudioFilter.h"
#include "axAudioEnvelope.h"

#include "axVstInterface.h"

#define LINE_INTERPOLE(y1, y2, mu) y1 + mu * (y2 - y1);

using namespace std;

struct TrackInfo
{
	int nSubTrack;
	double deviation;
	bool notes[3][16];
	double probability[3][16];
	double velocity[3][16];
};

struct DrumMachinePreset
{
	int nTracks;
	TrackInfo* info;

	DrumMachinePreset()
	{
		info = nullptr;
	}

	// Number of tracks. endl.
	// Number of subtrack. endl.
	// 16 * 3 bool of note on/off. endl.
	// 16 * 3 probability double. endl.
	// 16 * 3 velocity double. endl.
	DrumMachinePreset(const string& file_path)
	{
		 ifstream file(file_path);
		// ifstream file;
		// file.open(file_path);

		if(file.fail())
		{
			cerr << "Problem opening file " << file_path << endl;
		}
		else
		{
			string line;

			// axColor* ptr = &normal;
			//while (file.good())
			//{
				file >> nTracks;
				cout << "nb Tracks : " << nTracks << endl;

				if(nTracks)
				{
					info = new TrackInfo[nTracks];

					for(int i = 0; i < nTracks; i++)
					{
						file >> info[i].nSubTrack; 
						file >> info[i].deviation;
						// cout << "nb subTracks : " << info[i].nSubTrack << endl;

						// Fill on/off.
						cout << "Notes : " << endl;
						for(int k = 0; k < 3; k++)
						{
							for(int n = 0; n < 16; n++)
							{
								file >> info[i].notes[k][n];
								// cout << info[i].notes[k][n]; 
							}
							// cout << endl;
						}

						// Fill probabiblity.
						for(int k = 0; k < 3; k++)
						{
							for(int n = 0; n < 16; n++)
							{
								file >> info[i].probability[k][n];
								// cout << info[i].probability[k][n] << " "; 
							}
						}

						// Fill velocity.
						for(int k = 0; k < 3; k++)
						{
							for(int n = 0; n < 16; n++)
							{
								file >> info[i].velocity[k][n];
								// cout << info[i].velocity[k][n] << " "; 
							}
						}						
					}

				}
				else
				{
					cerr << "Preset has no tracks saved." << endl;
				}
			}
		}
		
	//}
};


#define TABLE_SIZE   (200)

struct paTestData
{
    float sine[TABLE_SIZE];
    int left_phase;
    int right_phase;
    char message[20];
};


//-----------------------------------------------------------------------------
// AudioTrack.
//-----------------------------------------------------------------------------
class AudioTrack
{
public:
	AudioTrack(const string& sndfile, const int& samplePerBeat);

	float* Process();

	void SetBeat(const int& index, const bool& on);

	void SetNote(const int& section, const int& index, const bool& on);

	void SetProbability(const int& section,	const int& index, const float& prob);

	void SetVelocity(const int& section, const int& index, const double& vel);

	void SetStandardDeviation(const double& dev);

	void SetNumberOfSection(const int& nSection);

	void SetPreset(TrackInfo* info);

	string GetSoundPath();

	axAudioBuffer* GetAudioBuffer();

	void SetMidiNoteOn();

	void SetFilterFrequency(const axFloat& freq);

	void SetFilterRes(const axFloat& q);

	void SetAttack(const axFloat& value);

	void SetDecay(const axFloat& value);

	void SetGain(const axFloat& value);

	void SetSpeed(const axFloat& speed);

	string GetTrackName() const;

	void SetTrackName(const string& name);

	void SetFilterFrequencyEnvelopeAmount(const axFloat& value);

	void SetPitchEnvelopeAmount(const axFloat& value);

private:
	static const int MAX_NUM_OF_PROB_TRACK = 3;
	static const int NUMBER_OF_NOTES = 16;

	axAudioBuffer* _buffer;
	string _trackName;
	bool _notes[3][16];
	float _probability[3][16];
	double _velocity[3][16];
	double _deviation;
	axFloat _speed, _c0;

	axFloat _pitchEnvValue;
	axFloat _pitchEnvAmount;

	bool _midiNoteOn;
	int _beatIndex, _sampleCount, _nSamplePerBeat;
	int _nSection, _selectedSection;
	double _nFrameBuf, _currentVelocity;

	// For return in audio callback.
	float* _outBuffer;

	axFloat _gain;
	axFloat _modFilterFreqEnv, _modFilterFreqEnvAmount;
	
	std::random_device rd;
	std::mt19937 gen;

	// Audio effects.
	axAudioFilter* _filter;
	axAudioEnvelope* _env;
};

//-----------------------------------------------------------------------------
// Audio.
//-----------------------------------------------------------------------------
class Audio
{
public: 
	Audio();
	~Audio()
	{
		Pa_Terminate();
	}

	void StartAudio();

	virtual int myMemberCallback(const void *input, 
						 void *output,
						 unsigned long frameCount, 
						 const PaStreamCallbackTimeInfo* timeInfo,
						 PaStreamCallbackFlags statusFlags,
						 Audio* userData)
	{
		 float *out = (float*)output;
		 paTestData* data = &userData->data;

	    for(int i = 0; i < frameCount; i++)
	    {
	        // *out++ = 0.0;
	        *out++ = data->sine[data->left_phase];  /* left */
	        // *out++ = 0.0;
	        *out++ = data->sine[data->right_phase];  /* right */
	        data->left_phase += 1;
	        if( data->left_phase >= TABLE_SIZE ) data->left_phase -= TABLE_SIZE;
	        data->right_phase += 3; /* higher pitch so we can distinguish left and right. */
	        if( data->right_phase >= TABLE_SIZE ) data->right_phase -= TABLE_SIZE;
	    }
    
    	return paContinue;
	}

	static int myPaCallback(const void *in, 
							void *out, 
							unsigned long frameCount, 
							const PaStreamCallbackTimeInfo* timeInfo, 
							PaStreamCallbackFlags statusFlags,
							void* userData)		
	{
		// static_cast<CallbackImplBase*>(userData)
		Audio* audio = static_cast<Audio*>(userData);//(Audio*)userData;
		return audio->myMemberCallback(in, out, frameCount, timeInfo, statusFlags, audio);
	}

// private:
	PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
    paTestData data;

};


//-----------------------------------------------------------------------------
// AudioMidiSeq.
//-----------------------------------------------------------------------------
class AudioMidiSeq: public Audio
{
public:
	AudioMidiSeq():
		// Members.
		_vst(44100, 1024)
	{
		// Vst plugin.
		_vst.LoadVstPlugin("C:\\VST\\TAL-Chorus-LX.dll");
		_vst.startPlugin();

		_buffer = new axAudioBuffer("kickw.wav");
		_buffer->ShowInfo();

		_nFrameBuf = 1410065408;
		_tempo = 120.0;
		_nSamplePerBeat = (60.0 / _tempo * 44100.0)/  4.0;
		_sampleCount = 0;

		_kickNote.resize(16);
		for(int i = 0; i < 16; i++)
		{
			_kickNote[i] = 0;
		}

		_beatIndex = 0;

		_track = new AudioTrack("kickw.wav", _nSamplePerBeat);

		_tracks.push_back(new AudioTrack("kickw.wav", _nSamplePerBeat));
		_tracks.push_back(new AudioTrack("snare06.wav", _nSamplePerBeat));
		_tracks.push_back(new AudioTrack("hihat3.wav", _nSamplePerBeat));
		_tracks.push_back(new AudioTrack("ohat.wav", _nSamplePerBeat));
		_tracks.push_back(new AudioTrack("snare2.wav", _nSamplePerBeat));
		_tracks.push_back(new AudioTrack("ohat.wav", _nSamplePerBeat));
		_tracks.push_back(new AudioTrack("snare2.wav", _nSamplePerBeat));
		_tracks.push_back(new AudioTrack("ohat.wav", _nSamplePerBeat));

		_tracks[0]->SetTrackName("Kick");
		_tracks[1]->SetTrackName("Snare");
		_tracks[2]->SetTrackName("HiHat");
		_tracks[3]->SetTrackName("OHat");
		_tracks[4]->SetTrackName("Clap");
		_tracks[5]->SetTrackName("Tom");
		_tracks[6]->SetTrackName("Bell");
		_tracks[7]->SetTrackName("Crash");
}

	void SetPreset(DrumMachinePreset* preset)
	{
		// cout << __PRETTY_FUNCTION__ << endl;

		if(preset->info != nullptr)
		{
			if(preset->nTracks <= _tracks.size())
			{
				for(int i = 0; i < preset->nTracks; i++)
				{
					 _tracks[i]->SetPreset(&preset->info[i]);
				}
			}
			else
			{
				cerr << "Error : Too many tracks in presets." << endl;
			}
		}
		else
		{
			cerr << "Error : Preset is not loaded." << endl;
		}


	}

	void SetKickBeat(vector<bool>& beat)
	{
		_kickNote = beat;
	}

	void SetKickBeat(const int& index, const bool& on)
	{
		_tracks[0]->SetBeat(index, on);
	}

	void SetBeat(const int& track_index, const int& index, const bool& on)
	{
		_tracks[track_index]->SetBeat(index, on);
	}

	void SetStandardDeviation(const int& track_index, const double& dev)
	{
		_tracks[track_index]->SetStandardDeviation(dev);
	}

	void SetLiveMidiNoteOn(const int& track_index)
	{
		_tracks[track_index]->SetMidiNoteOn();
	}
	
	void SetMidiNote(const int& track_index, 
					 const int& section_index, 
					 const int& index, 
					 const bool& on)
	{
		_tracks[track_index]->SetNote(section_index, index, on);
	}

	void SetTrackNumberOfSection(const int& track, const int& section)
	{
		_tracks[track]->SetNumberOfSection(section);
	}

	axAudioBuffer* GetAudioBufferFromTrack(const int& track_num)
	{
		return _tracks[track_num]->GetAudioBuffer();
	}

	void SetProbability(const int& track,
						const int& section, 
				 		const int& index, 
				 		const float& prob)
	{
		_tracks[track]->SetProbability(section, 
				 					   index, 
				 					   prob);
	}

	void SetVelocity(const int& track,
						const int& section, 
				 		const int& index, 
				 		const double& vel)
	{
		_tracks[track]->SetVelocity(section, 
				 					   index, 
				 					   vel);
	}

	AudioTrack* GetAudioTrack(const int& track_num)
	{
		return _tracks[track_num];
	}

	virtual int myMemberCallback(const void *input, 
						 void *output,
						 unsigned long frameCount, 
						 const PaStreamCallbackTimeInfo* timeInfo,
						 PaStreamCallbackFlags statusFlags,
						 Audio* userData)
	{
		float *out = (float*)output;

		float* tracks_out[15];
		for(int i = 0; i < _tracks.size(); i++)
		{
			tracks_out[i] = _tracks[i]->Process();
		} 

		axFloat** vst_input_buffers = _vst.GetInputBuffers();
		axFloat** vst_output_buffers = _vst.GetOutputBuffers();

		for(int i = 0; i < 1024; i++)
		{
			float v_l = 0.0;
			float v_r = 0.0;

			for(int n = 0; n < _tracks.size(); n++)
			{
				v_l += *tracks_out[n]++;;
				v_r += *tracks_out[n]++;
			}

			vst_input_buffers[0][i] = v_l;
			vst_input_buffers[1][i] = v_r;
			//*out++ = v_l;
			//*out++ = v_r;
		}

		_vst.processAudio(vst_input_buffers, vst_output_buffers);

		int index = 0;
		for (int i = 0; i < 1024 * 2; i+=2, index++)
		{
			out[i] = vst_output_buffers[0][index];
			out[i + 1] = vst_output_buffers[1][index];
		}

    	return paContinue;
	}

	axAudioBuffer* GetAudioBuffer()
	{
		return _buffer;
	}

	int GetNumberOfTrack() const
	{
		return _tracks.size();
	}

	string GetSoundPath(const int& track)
	{
		return _tracks[track]->GetSoundPath();
	}

private:
	axAudioBuffer* _buffer;
	int _nFrameBuf;
	int _nSamplePerBeat;
	float _tempo;
	unsigned int _sampleCount;

	vector<bool> _kickNote;
	int _beatIndex;

	AudioTrack* _track;
	vector<AudioTrack*> _tracks;
	axVstInterface _vst;

};

#endif // __AX_AUDIO__