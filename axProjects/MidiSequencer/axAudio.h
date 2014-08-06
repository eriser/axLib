#ifndef __AX_AUDIO__
#define __AX_AUDIO__

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <fstream>


#include "portaudio.h" 
#include "axAudioBuffer.h"

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

//------------------------------------------------------------------
// You get this error:
// bt_audio_service_open: connect() failed: Connection refused (111)
// Solution : sudo apt-get purge bluez-als
//------------------------------------------------------------------
#define TABLE_SIZE   (200)

struct paTestData
{
    float sine[TABLE_SIZE];
    int left_phase;
    int right_phase;
    char message[20];
};



class AudioTrack
{
public:
	AudioTrack(const string& sndfile, const int& samplePerBeat);

	float* Process();

	void SetBeat(const int& index, const bool& on)
	{
		_notes[0][index] = on;
	}

	void SetNote(const int& section, 
				 const int& index, 
				 const bool& on)
	{
		_notes[section][index] = on;
	}

	void SetProbability(const int& section, 
				 		const int& index, 
				 		const float& prob)
	{
		_probability[section][index] = prob;
	}

	void SetVelocity(const int& section, 
		 			 const int& index, 
			 		 const double& vel)
	{
		_velocity[section][index] = vel;
	}

	void SetStandardDeviation(const double& dev)
	{
		_deviation = dev;

		cout << "Value " << dev << endl;
	}

	void SetNumberOfSection(const int& nSection)
	{
		_nSection = nSection;
	}

	void SetPreset(TrackInfo* info)
	{ 
		cout << __PRETTY_FUNCTION__ << endl;
		_nSection = info->nSubTrack;

		cout << "INFO VALUE : " << info->nSubTrack << endl;
		int nSubTrack;

		for(int j = 0; j < MAX_NUM_OF_PROB_TRACK; j++)
		{
			for(int i = 0; i < NUMBER_OF_NOTES; i++)
			{
				_notes[j][i] = info->notes[j][i];
				_probability[j][i] = info->probability[j][i];
				_velocity[j][i] = info->velocity[j][i];
			}
		}
	}


private:
	static const int MAX_NUM_OF_PROB_TRACK = 3;
	static const int NUMBER_OF_NOTES = 16;


	axAudioBuffer* _buffer;
	// vector<bool> _notes;
	bool _notes[3][16];
	float _probability[3][16];
	double _velocity[3][16];
	double _deviation;

	int _nFrameBuf;
	int _beatIndex, _sampleCount, _nSamplePerBeat;
	int _nSection;
	int _selectedSection;
	double _currentVelocity;

	float* _outBuffer;

	std::random_device rd;
    // std::mt19937 gen;
};

class Audio
{
public: 
	Audio();

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



class AudioMidiSeq: public Audio
{
public:
	AudioMidiSeq()
	{
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

		// cout << __PRETTY_FUNCTION__ << endl;
		cout << "TRACKS SIZE : " << _tracks.size() << endl;
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

		// float* track1 = _tracks[0]->Process();
		// float* track2 = _tracks[1]->Process();
		// float* track3 = _tracks[2]->Process();
		// float* track4 = _tracks[3]->Process();
		// float* track5 = _tracks[4]->Process();

		for(int i = 0; i < 1024; i++)
		{
			float v_l = 0.0;
			float v_r = 0.0;

			for(int n = 0; n < _tracks.size(); n++)
			{
				// float* f = *tracks_out[n]++;
				v_l += *tracks_out[n]++;;
				v_r += *tracks_out[n]++;
				// tracks_out[n] = f;
			}

			*out++ = v_l;
			*out++ = v_r;
			// *out++ = *track1++ + *track2++ + *track3++ + *track4++ + *track5++;
			// *out++ = *track1++ + *track2++ + *track3++ + *track4++ + *track5++;
		}

    	return paContinue;
	}

	axAudioBuffer* GetAudioBuffer()
	{
		return _buffer;
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

};

#endif // __AX_AUDIO__