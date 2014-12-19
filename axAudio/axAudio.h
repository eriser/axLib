#ifndef __AX_AUDIO__
#define __AX_AUDIO__

//------------------------------------------------------------------
// On linux : if you get this error:
// bt_audio_service_open: connect() failed: Connection refused (111)
// Solution : sudo apt-get purge bluez-als
//------------------------------------------------------------------

#define _USE_MATH_DEFINES
#include <cmath>

//#include <iostream>
//#include <vector>
//#include <string>
//#include <random>
//#include <fstream>
#include "axUtils.h"


#include "portaudio.h" 
//#include "axAudioBuffer.h"
//#include "axAudioFilter.h"
//#include "axAudioEnvelope.h"

//#include "axVstInterface.h"

//#define LINE_INTERPOLE(y1, y2, mu) y1 + mu * (y2 - y1);

//using namespace std;
//
//struct TrackInfo
//{
//	int nSubTrack;
//	double deviation;
//	bool notes[3][16];
//	double probability[3][16];
//	double velocity[3][16];
//};
//
//struct DrumMachinePreset
//{
//	int nTracks;
//	TrackInfo* info;
//
//	DrumMachinePreset()
//	{
//		info = nullptr;
//	}
//
//	// Number of tracks. endl.
//	// Number of subtrack. endl.
//	// 16 * 3 bool of note on/off. endl.
//	// 16 * 3 probability double. endl.
//	// 16 * 3 velocity double. endl.
//	DrumMachinePreset(const string& file_path)
//	{
//		 ifstream file(file_path);
//		// ifstream file;
//		// file.open(file_path);
//
//		if(file.fail())
//		{
//			cerr << "Problem opening file " << file_path << endl;
//		}
//		else
//		{
//			string line;
//
//			// axColor* ptr = &normal;
//			//while (file.good())
//			//{
//				file >> nTracks;
//				cout << "nb Tracks : " << nTracks << endl;
//
//				if(nTracks)
//				{
//					info = new TrackInfo[nTracks];
//
//					for(int i = 0; i < nTracks; i++)
//					{
//						file >> info[i].nSubTrack; 
//						file >> info[i].deviation;
//						// cout << "nb subTracks : " << info[i].nSubTrack << endl;
//
//						// Fill on/off.
//						cout << "Notes : " << endl;
//						for(int k = 0; k < 3; k++)
//						{
//							for(int n = 0; n < 16; n++)
//							{
//								file >> info[i].notes[k][n];
//								// cout << info[i].notes[k][n]; 
//							}
//							// cout << endl;
//						}
//
//						// Fill probabiblity.
//						for(int k = 0; k < 3; k++)
//						{
//							for(int n = 0; n < 16; n++)
//							{
//								file >> info[i].probability[k][n];
//								// cout << info[i].probability[k][n] << " "; 
//							}
//						}
//
//						// Fill velocity.
//						for(int k = 0; k < 3; k++)
//						{
//							for(int n = 0; n < 16; n++)
//							{
//								file >> info[i].velocity[k][n];
//								// cout << info[i].velocity[k][n] << " "; 
//							}
//						}						
//					}
//
//				}
//				else
//				{
//					cerr << "Preset has no tracks saved." << endl;
//				}
//			}
//		}
//		
//	//}
//};
//
//
//#define TABLE_SIZE   (200)
//
//struct paTestData
//{
//    float sine[TABLE_SIZE];
//    int left_phase;
//    int right_phase;
//    char message[20];
//};
//
//
////-----------------------------------------------------------------------------
//// AudioTrack.
////-----------------------------------------------------------------------------
//class AudioTrack
//{
//public:
//	AudioTrack(const string& sndfile, const int& samplePerBeat);
//
//	float* Process();
//
//	void SetBeat(const int& index, const bool& on);
//
//	void SetNote(const int& section, const int& index, const bool& on);
//
//	void SetProbability(const int& section,	const int& index, const float& prob);
//
//	void SetVelocity(const int& section, const int& index, const double& vel);
//
//	void SetStandardDeviation(const double& dev);
//
//	void SetNumberOfSection(const int& nSection);
//
//	void SetPreset(TrackInfo* info);
//
//	string GetSoundPath();
//
//	axAudioBuffer* GetAudioBuffer();
//
//	void SetMidiNoteOn();
//
//	void SetFilterFrequency(const axFloat& freq);
//
//	void SetFilterRes(const axFloat& q);
//
//	void SetAttack(const axFloat& value);
//
//	void SetDecay(const axFloat& value);
//
//	void SetGain(const axFloat& value);
//
//	void SetSpeed(const axFloat& speed);
//
//	string GetTrackName() const;
//
//	void SetTrackName(const string& name);
//
//	void SetFilterFrequencyEnvelopeAmount(const axFloat& value);
//
//	void SetPitchEnvelopeAmount(const axFloat& value);
//
//private:
//	static const int MAX_NUM_OF_PROB_TRACK = 3;
//	static const int NUMBER_OF_NOTES = 16;
//
//	axAudioBuffer* _buffer;
//	string _trackName;
//	bool _notes[3][16];
//	float _probability[3][16];
//	double _velocity[3][16];
//	double _deviation;
//	axFloat _speed, _c0;
//
//	axFloat _pitchEnvValue;
//	axFloat _pitchEnvAmount;
//
//	bool _midiNoteOn;
//	int _beatIndex, _sampleCount, _nSamplePerBeat;
//	int _nSection, _selectedSection;
//	double _nFrameBuf, _currentVelocity;
//
//	// For return in audio callback.
//	float* _outBuffer;
//
//	axFloat _gain;
//	axFloat _modFilterFreqEnv, _modFilterFreqEnvAmount;
//	
//	std::random_device rd;
//	std::mt19937 gen;
//
//	// Audio effects.
//	axAudioFilter* _filter;
//	axAudioEnvelope* _env;
//};

//-----------------------------------------------------------------------------
// Audio.
//-----------------------------------------------------------------------------
class axAudio
{
public: 
	axAudio();
	~axAudio()
	{
		Pa_Terminate();
	}

    int InitAudio();
	void StartAudio();
    void StopAudio();
    
    virtual int CallbackAudio(const float *input,
                              float *output,
                              unsigned long frameCount)
    {
        float *out = output;
        
        for(int i = 0; i < frameCount; i++)
        {
            *out++ = 0.0;
            *out++ = 0.0;
        }
        
        return 0;
    }

	static int myPaCallback(const void *in,
							void *out, 
							unsigned long frameCount, 
							const PaStreamCallbackTimeInfo* timeInfo, 
							PaStreamCallbackFlags statusFlags,
							void* userData)		
	{
		axAudio* audio = static_cast<axAudio*>(userData);
        return audio->CallbackAudio((const float*)in, (float*)out, frameCount);
	}

// private:
	PaStreamParameters outputParameters;
    PaStream *stream;
    PaError err;
};

#endif // __AX_AUDIO__