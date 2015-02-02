#include <cassert>

#if WINDOWS
#include <Windows.h>
#endif // WINDOWS

#include "MIDI.h"
#include <iostream>

#if MAC
#include <libkern/OSAtomic.h>
#endif // MAC

	
const int kRingBufferSize = 512;

#if WINDOWS
typedef long TAtomicValue;
#endif // WINDOWS
#if MAC
typedef int32_t TAtomicValue;
#endif // MAC

class CMIDIRingBuffer
{
	public: CMIDIRingBuffer();
	public: ~CMIDIRingBuffer();
	
	public: void Push(const CMIDIEvent& event);
	public: bool Pop(CMIDIEvent& event);

	private: CMIDIEvent fBuffer[kRingBufferSize];
	private: int fWriteIndex;
	private: int fReadIndex;
	private: volatile TAtomicValue fSize;
};

CMIDIRingBuffer::CMIDIRingBuffer()
: fWriteIndex(0)
, fReadIndex(0)
, fSize(0)
{
}

CMIDIRingBuffer::~CMIDIRingBuffer()
{
}

void CMIDIRingBuffer::Push(const CMIDIEvent& event)
{
	if (fSize < kRingBufferSize) {
		fBuffer[fWriteIndex] = event;
		fWriteIndex++;
		if (fWriteIndex >= kRingBufferSize) {
			fWriteIndex = 0;
		}
#if WINDOWS
		(void) InterlockedExchangeAdd(&fSize, 1);
#endif // WINDOWS
#if MAC
		(void) OSAtomicIncrement32Barrier(&fSize);
#endif // MAC
	}
	else {
		std::cout << "Ring buffer FULL!!!" << std::endl;
	}
}

bool CMIDIRingBuffer::Pop(CMIDIEvent& event)
{
	if (fSize > 0) {
		event = fBuffer[fReadIndex];
		fReadIndex++;
		if (fReadIndex >= kRingBufferSize) {
			fReadIndex = 0;
		}
#if WINDOWS
		(void) InterlockedExchangeAdd(&fSize, -1);
#endif // WINDOWS
#if MAC
		(void) OSAtomicDecrement32Barrier(&fSize);
#endif // MAC
		return true;
	}
	else {
		return false;
	}
}
	


// CMIDIEvent
/////////////
	
CMIDIEvent::CMIDIEvent()
{
	fStatus = 0;
	fData1 = 0;
	fData2 = 0;
}


CMIDIEvent::CMIDIEvent(NSSimpleMIDIAPI::TMIDIByte status, NSSimpleMIDIAPI::TMIDIByte data1, NSSimpleMIDIAPI::TMIDIByte data2)
{
	fStatus = status;
	fData1 = data1;
	fData2 = data2;
}


CMIDIEvent::EMIDIType CMIDIEvent::GetType() const
{
	switch (fStatus & 0xf0) {
		case 0x90:
			if (fData2 != 0) {
				return MIDI_NOTEON;
			}
			else {
				return MIDI_NOTEOFF;
			}
			break;
		case 0x80:
			return MIDI_NOTEOFF;
			break;
		case 0xB0:
			return MIDI_CONTROL;
			break;
		case 0xa0:
			return MIDI_POLYPRESS;
			break;
		case 0xc0:
			return MIDI_PROGRAM;
			break;
		case 0xd0:
			return MIDI_MONOPRESS;
			break;
		case 0xe0:
			return MIDI_PITCHBEND;
			break;
		default:
			return MIDI_OTHER;
	}
}




// CMIDIServer
//////////////

CMIDIServer::CMIDIServer()
: fMIDIInstance(NULL)
, fEvents(new CMIDIRingBuffer)
{
	NSSimpleMIDIAPI::TMIDIInPortIDs ports = NSSimpleMIDIAPI::GetMIDIInPortIDs();
	if (ports.size() > 0) {
		fMIDIInstance = new NSSimpleMIDIAPI::CSimpleMIDIInInstance(ports[0], *this);
	}
}

CMIDIServer::~CMIDIServer()
{
	if (fMIDIInstance) {
		delete fMIDIInstance;
		fMIDIInstance = NULL;
	}
	
	if (fEvents) {
		delete fEvents;
		fEvents = NULL;
	}
}



bool CMIDIServer::PopEvent(CMIDIEvent& event)
{
	assert(fEvents != NULL);
	return fEvents->Pop(event);
}

void CMIDIServer::ISimpleMIDIInCallback_OnShortMIDI(
	NSSimpleMIDIAPI::CSimpleMIDIInInstance& /*iInstance*/,
	NSSimpleMIDIAPI::TMilliSecond /*iTimeStampMS*/,
	NSSimpleMIDIAPI::TMIDIByte iStatus,
	NSSimpleMIDIAPI::TMIDIByte iData1,
	NSSimpleMIDIAPI::TMIDIByte iData2)
{
	CMIDIEvent event(iStatus, iData1, iData2);
	fEvents->Push(event);
}

