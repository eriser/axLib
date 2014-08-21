/*
 *  MIDI.h
 *  CocoaMixer
 *
 *  Created by Joakim Nyström on 2010-09-29.
 *  Copyright 2010 Propellerhead Software AB. All rights reserved.
 *
 */

#include <vector>
#include "SimpleAudioMIDI/SimpleMIDIAPI.h"

class CMIDIEvent {
	public: enum EMIDIType {
		MIDI_NOTEON,
		MIDI_NOTEOFF,
		MIDI_CONTROL,
		MIDI_POLYPRESS,
		MIDI_PROGRAM,
		MIDI_MONOPRESS,
		MIDI_PITCHBEND,
		MIDI_OTHER
	};
	
	public: CMIDIEvent();
	public: CMIDIEvent(NSSimpleMIDIAPI::TMIDIByte status, NSSimpleMIDIAPI::TMIDIByte data1, NSSimpleMIDIAPI::TMIDIByte data2);

	public: EMIDIType GetType() const;
	
	public: NSSimpleMIDIAPI::TMIDIByte fStatus;
	public: NSSimpleMIDIAPI::TMIDIByte fData1;
	public: NSSimpleMIDIAPI::TMIDIByte fData2;
};

class CMIDIRingBuffer;
class CMIDIServer : NSSimpleMIDIAPI::ISimpleMIDIInCallback {
	public: CMIDIServer();
	public: ~CMIDIServer();
	
	public:	bool PopEvent(CMIDIEvent& event);
	
	public: virtual void ISimpleMIDIInCallback_OnShortMIDI(
														   NSSimpleMIDIAPI::CSimpleMIDIInInstance& iInstance,
														   NSSimpleMIDIAPI::TMilliSecond iTimeStampMS,
														   NSSimpleMIDIAPI::TMIDIByte iStatus,
														   NSSimpleMIDIAPI::TMIDIByte iData1,
														   NSSimpleMIDIAPI::TMIDIByte iData2);
	
	private: NSSimpleMIDIAPI::CSimpleMIDIInInstance* fMIDIInstance;
	private: CMIDIRingBuffer* fEvents;
};

