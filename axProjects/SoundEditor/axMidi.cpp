//#include "axMidi.h"
//
//axMidi::axMidi(Audio* audio):
//	_active(false),
//	_selectedOutDevice(0),
//	_selectedInDevice(0),
//	_midiStream(nullptr)
//{
//	_audio = static_cast<AudioMidiSeq*>(audio);
//	FillMidiDeviceInfo();
//	_selectedInDevice = 1;
//}
//
//void axMidi::FillMidiDeviceInfo()
//{
//	const PmDeviceInfo *info;
//	int nDevice = 0;
//
//	if (nDevice = Pm_CountDevices())
//	{
//		for (int i = 0; i < nDevice; i++)
//		{
//			info = Pm_GetDeviceInfo(i);
//
//			if (info->input)
//			{
//				_inDevices.push_back(axMidiDevice(info->name, i));
//				cout << "IN : " << i << " " << info->name << endl;
//			}
//			else if (info->output)
//			{
//				_outDevices.push_back(axMidiDevice(info->name, i));
//				cout << "OUT : " << info->name << endl;
//			}
//		}
//	}
//}
//
//axMidi::~axMidi()
//{
//	if (_midiStream)
//	{
//		Pm_Close(_midiStream);
//	}
//}
//
//
//void axMidi::StartStream()
//{
//	if (_selectedInDevice < _inDevices.size())
//	{
//		Pt_Start(1, &axMidi::Process_Midi, (axMidi*)this);
//		Pm_Initialize();
//		PmError pmError = Pm_OpenInput(&_midiStream,
//			_selectedInDevice,
//			NULL, 512L, NULL, NULL);
//
//		_active = true;
//	}
//}
//
//void axMidi::StopStream()
//{
//	_active = false;
//	Pt_Stop();
//
//	if (_midiStream)
//	{
//		Pm_Close(_midiStream);
//	}
//
//}
//
//bool axMidi::IsActive() const
//{
//	return _active;
//}
//
//PortMidiStream* axMidi::GetMidiStream()
//{
//	return _midiStream;
//}
//
//void axMidi::Process(PtTimestamp timestamp, axMidi* userData)
//{
//	axMidi* midi = userData;
//	PortMidiStream* in = midi->GetMidiStream();
//	AudioMidiSeq* audio = midi->GetAudio();
//
//	PmError result;
//	PmEvent buffer; /* just one message at a time */
//
//	if (!midi->IsActive())
//	{
//		return;
//	}
//
//	while (Pm_Poll(in))
//	{
//		int status, data1, data2;
//
//		if (Pm_Read(in, &buffer, 1) == pmBufferOverflow)
//			continue;
//
//		status = Pm_MessageStatus(buffer.message);
//		data1 = Pm_MessageData1(buffer.message);
//		data2 = Pm_MessageData2(buffer.message);
//
//		if ((status & 0xF0) == 0x90)// || (status & 0xF0) == 0x80) 
//		{
//			cout << "Note On : " << data1 << " velocity : " << data2 << endl;
//
//			switch (data1)
//			{
//			case 36: // Kick.
//				audio->SetLiveMidiNoteOn(0);
//				break;
//
//			case 38: // Snare.
//			case 40:
//				audio->SetLiveMidiNoteOn(1);
//				break;
//
//			case 42: // Snare.
//				audio->SetLiveMidiNoteOn(2);
//				break;
//			}
//			
//		}
//		else if ((status & 0xF0) == 0x80)
//		{
//			cout << "Note Off : " << data1 << endl;
//		}
//	}
//}