#ifndef __AX_MIDI__
#define __AX_MIDI__

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "axUtils.h"

//#include "portmidi.h"
//#include "porttime.h"
#include "axAudio.h"

using namespace std;
//
//struct axMidiDevice
//{
//	axMidiDevice(){}
//	axMidiDevice(const string& dev_name, const int& id_):
//				id(id_), 
//				name(dev_name)
//	{
//	}
//
//	int id;
//	string name;
//};
//
//class axMidi
//{
//public:
//	axMidi(Audio* audio);
//	~axMidi();
//
//	void StartStream();
//	void StopStream();
//	void ReInitInput(int input);
//
//	bool IsActive() const;
//	PortMidiStream* GetMidiStream();
//
//	AudioMidiSeq* GetAudio()
//	{
//		return _audio;
//	}
//
//private:
//	AudioMidiSeq* _audio;
//	bool _active; 
//	int _selectedOutDevice, _selectedInDevice;
//	vector<axMidiDevice> _inDevices, _outDevices;
//	
//	void FillMidiDeviceInfo();
//
//	virtual void Process(PtTimestamp timestamp, axMidi* userData);
//
//	static void Process_Midi(PtTimestamp timestamp, void* userData)
//	{
//		return ((axMidi*)userData)->Process(timestamp, (axMidi*)userData);
//	}
//
//	PortMidiStream* _midiStream;
//
//};
#endif // __AX_MIDI__