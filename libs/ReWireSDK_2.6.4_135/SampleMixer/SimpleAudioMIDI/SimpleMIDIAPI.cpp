#if WINDOWS
#include <Windows.h>
#endif // WINDOWS

#include "SimpleMIDIAPI.h"
#include <stdexcept>
#include <iostream>

namespace NSSimpleMIDIAPI {

namespace NSInternals {

#if WINDOWS
class CMMEMIDIInInstance {
	public: CMMEMIDIInInstance(
				CSimpleMIDIInInstance& iInstance,
				const TMIDIInPortID& iMIDIInPortID,
				ISimpleMIDIInCallback& iSimpleMIDIInCallback);
	public: ~CMMEMIDIInInstance();

	public: const TMIDIInPortID& GetPortID() const;

	private: void Create();
	private: void Dispose();

	private: CSimpleMIDIInInstance& fInstance;
	private: TMIDIInPortID fMIDIInPortID;
	private: ISimpleMIDIInCallback& fSimpleMIDIInCallback;
	private: UINT fMMTimerPeriod;
	private: HMIDIIN fHMIDIIn;
	private: bool fIsOpen;

	private: static void CALLBACK MidiInProc(HMIDIIN /*hMidiIn*/, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
};

CMMEMIDIInInstance::CMMEMIDIInInstance(
		CSimpleMIDIInInstance& iInstance,
		const TMIDIInPortID& iMIDIInPortID,
		ISimpleMIDIInCallback& iSimpleMIDIInCallback)
	: fInstance(iInstance)
	, fMIDIInPortID(iMIDIInPortID)
	, fSimpleMIDIInCallback(iSimpleMIDIInCallback)
	, fMMTimerPeriod(0)
	, fHMIDIIn(0)
	, fIsOpen(false)
{
	try{
		Create();
	}
	catch(...){
		Dispose();
		throw;
	}
}

CMMEMIDIInInstance::~CMMEMIDIInInstance(){
	Dispose();
}

const TMIDIInPortID& CMMEMIDIInInstance::GetPortID() const{
	return fMIDIInPortID;
}


void CMMEMIDIInInstance::Create(){
	UINT period = 0;
	MMRESULT result = TIMERR_NOCANDO;
	while(result == TIMERR_NOCANDO){
		++period;
		result = ::timeBeginPeriod(period);
	}
	if(result != TIMERR_NOERROR){
		throw std::runtime_error("timeBeginPeriod failed");
	}
	fMMTimerPeriod = period;

	result = midiInOpen(&fHMIDIIn,
		fMIDIInPortID,
		(DWORD_PTR)MidiInProc,
		(DWORD_PTR)this,
		CALLBACK_FUNCTION);

	if(result != MMSYSERR_NOERROR){
		throw std::runtime_error("midiInOpen failed");
	}

	//	Start input.
	result = midiInStart(fHMIDIIn);

	if(result != MMSYSERR_NOERROR){
		throw std::runtime_error("midiInOpen failed");
	}

}

void CMMEMIDIInInstance::Dispose(){
	if(fMMTimerPeriod != 0){
		::timeEndPeriod(fMMTimerPeriod);
		fMMTimerPeriod = 0;
	}
	if(fHMIDIIn != 0){
		MMRESULT result = midiInStop(fHMIDIIn);
		result = midiInReset(fHMIDIIn);
		result = midiInClose(fHMIDIIn);
		fHMIDIIn=0;
	}
}




TMIDIByte GetByte0(DWORD_PTR value){
	return static_cast<TMIDIByte>(value & 0xff);
}
TMIDIByte GetByte8(DWORD_PTR value){
	return static_cast<TMIDIByte>((value >> 8L) & 0xff);
}
TMIDIByte GetByte16(DWORD_PTR value){
	return static_cast<TMIDIByte>((value >> 16L) & 0xff);
}




void CALLBACK CMMEMIDIInInstance::MidiInProc(HMIDIIN /*hMidiIn*/, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2){
	try{
		// DE: Buggy MIDI driver timestamp:
		// If we try to use the timestamp we get in the dwParam2 param we have problems:
		// 1. It's relative to the time that the port was opened - when exactly is that? Before or after call to midiInOpen?
		// 2. We got a bunch of reports of the MIDI latency increasing over time - this is due to the fact
		// that the midi stamp received in dwParam2 drifts ahead of time compared to the OS function timeGetTime().
		// I.e. after Reason has run for 10 minutes the timestamp we get in dwParam2+opentime is 120030 ms but
		// timeGetTime says 120000. This error increases over time.
		// We are contacting MIDIMan with this problem to see if we can resolve it some other way.
		// For now we just get the timestamp ourselves.

		DWORD systemMS = ::timeGetTime();

		CMMEMIDIInInstance* instance = reinterpret_cast<CMMEMIDIInInstance*>(dwInstance);

		if(instance != NULL){
			switch(wMsg){
				case MIM_DATA:
					if(instance->fIsOpen){
						TMIDIByte status = GetByte0(dwParam1);
						TMIDIByte data1 = GetByte8(dwParam1);
						TMIDIByte data2 = GetByte16(dwParam1);
						instance->fSimpleMIDIInCallback.ISimpleMIDIInCallback_OnShortMIDI(
							instance->fInstance,
							systemMS,
							status,
							data1,
							data2);
					}
					break;

				case MIM_LONGDATA:
					break;

				case MIM_OPEN:
					instance->fIsOpen = true;
					break;

				case MIM_CLOSE:
					instance->fIsOpen = false;
					break;

				case MIM_ERROR:
					break;

				case MIM_LONGERROR:
					break;

				case MIM_MOREDATA:
					break;

				default:
					break;
			}
		}
	}
	catch(const std::exception& /*e*/){
	}
	catch(...){
	}
}

bool GetMIDIInPortCaps(const TMIDIInPortID& iMIDIInPortID, TMIDIInPortCaps& oMIDIInPortCaps){
	MMRESULT mmresult = midiInGetDevCaps(
		iMIDIInPortID,
		&oMIDIInPortCaps,
		sizeof(MIDIINCAPS));
	if(mmresult == MMSYSERR_NOERROR){
		return true;
	}
	else{
		return false;
	}
}


TMIDIInPortIDs GetMIDIInPortIDs(){
	TMIDIInPortIDs result;
	UINT count = midiInGetNumDevs();
	result.reserve(static_cast<std::size_t>(count));
	for(UINT index = 0; index < count; ++index){
		TMIDIInPortCaps caps;
		if(GetMIDIInPortCaps(index, caps)){
			result.push_back(index);
		}
	}
	return result;
}
#endif // WINDOWS

#if MAC
class CCoreMIDIInstance {
	public: CCoreMIDIInstance(
							   CSimpleMIDIInInstance& iInstance,
							   const TMIDIInPortID& iMIDIInPortID,
							   ISimpleMIDIInCallback& iSimpleMIDIInCallback);
	public: ~CCoreMIDIInstance();
		
	public: const TMIDIInPortID& GetPortID() const;
		
	private: void Create();
	private: void Dispose();
		
	private: CSimpleMIDIInInstance& fInstance;
	private: MIDIClientRef fClient;
	private: MIDIPortRef fPort;
	private: MIDIEndpointRef fSource;
	private: ISimpleMIDIInCallback& fSimpleMIDIInCallback;
		
	private: static void ReadProc(const MIDIPacketList *pktlist, void *readProcRefCon, void *srcConnRefCon);
};

	
CCoreMIDIInstance::CCoreMIDIInstance(
					  CSimpleMIDIInInstance& iInstance,
					  const TMIDIInPortID& iMIDIInPortID,
					  ISimpleMIDIInCallback& iSimpleMIDIInCallback)
	: fInstance(iInstance)
	, fClient(NULL)
	, fPort(NULL)
	, fSource(iMIDIInPortID)
	, fSimpleMIDIInCallback(iSimpleMIDIInCallback)
{
	Create();
}
	
	
CCoreMIDIInstance::~CCoreMIDIInstance()
{
	Dispose();
}

const TMIDIInPortID& CCoreMIDIInstance::GetPortID() const
{
	return fSource;
}
	
void CCoreMIDIInstance::Create()
{
	OSStatus status = MIDIClientCreate(CFSTR("SimpleMIDIAPI.client"), NULL, NULL, &fClient);
	if (status != noErr) {
		throw std::runtime_error("MIDIClientCreate() failed");
	}

	assert(fSource != 0);
		
//		CFStringRef description = NULL;
//		OSStatus status = MIDIObjectGetStringProperty(fSource, kMIDIPropertyDisplayName, &description);
//		assert(status == noErr);
//		CFShow(description);
		
	status = MIDIInputPortCreate(fClient, CFSTR("SimpleMIDIAPI.port"), &CCoreMIDIInstance::ReadProc, this, &fPort);
	if (status != noErr) {
		throw std::runtime_error("MIDIInputPortCreate() failed");
	}
	
	status = MIDIPortConnectSource(fPort, fSource, this);
	if (status != noErr) {
		throw std::runtime_error("MIDIInputPortCreate() failed");
	}
}


void CCoreMIDIInstance::Dispose()
{
	OSStatus status = MIDIPortDisconnectSource(fPort, fSource);
	if (status != noErr) {
		std::cerr << "MIDIPortDisconnectSource() failed" << std::endl;
	}
	
	if (fPort) {
		status = MIDIPortDispose(fPort);
		if (status != noErr) {
			std::cerr << "MIDIPortDispose() failed" << std::endl;
		}
		fPort = NULL;
	}
	
	if (fClient) {
		status = MIDIClientDispose(fClient);
		if (status != noErr) {
			std::cerr << "MIDIPortDispose() failed" << std::endl;
		}
		fClient = NULL;
	}
}

	
void CCoreMIDIInstance::ReadProc(const MIDIPacketList* pktlist, void* readProcRefCon, void* /*srcConnRefCon*/)
{
	CCoreMIDIInstance* instance = reinterpret_cast<CCoreMIDIInstance*>(readProcRefCon);
	if (instance != NULL) {
		int count = pktlist->numPackets;
		for (int i = 0; i < count; i++) {
			MIDIPacket currentPacket = *(pktlist->packet);
			if (currentPacket.length == 3) {				
				instance->fSimpleMIDIInCallback.ISimpleMIDIInCallback_OnShortMIDI(instance->fInstance, currentPacket.timeStamp, currentPacket.data[0], currentPacket.data[1], currentPacket.data[2]);
			}
		}
	}	
}

std::vector<MIDIEndpointRef> GetMIDIInPortIDs();
bool GetMIDIInPortCaps(const TMIDIInPortID& iMIDIInPortID, TMIDIInPortCaps& oMIDIInPortCaps);

std::vector<MIDIEndpointRef> GetMIDIInPortIDs()
{
	std::vector<MIDIEndpointRef> sourceList;	

	ItemCount sourceCount = MIDIGetNumberOfSources();
	for (ItemCount i=0; i<sourceCount; i++) {
		MIDIEndpointRef source = MIDIGetSource(i);
		sourceList.push_back(source);
	}
	
	return sourceList;
}

bool GetMIDIInPortCaps(const TMIDIInPortID& iMIDIInPortID, TMIDIInPortCaps& oMIDIInPortCaps)
{
	CFStringRef description = NULL;
	OSStatus status = MIDIObjectGetStringProperty(iMIDIInPortID, kMIDIPropertyDisplayName, &description);
	if (status == noErr) {
//		oMIDIInPortCaps = description;
		oMIDIInPortCaps = "JNDEBUG";
		return true;
	}
	else {
		return false;
	}
}
	
#endif // MAC
	
} // NSInternals
	

TMIDIInPortIDs GetMIDIInPortIDs(){
	return NSInternals::GetMIDIInPortIDs();
}

bool GetMIDIInPortCaps(const TMIDIInPortID& iMIDIInPortID, TMIDIInPortCaps& oMIDIInPortCaps){
	return NSInternals::GetMIDIInPortCaps(iMIDIInPortID, oMIDIInPortCaps);
}


CSimpleMIDIInInstance::CSimpleMIDIInInstance(
				const TMIDIInPortID& iMIDIInPortID,
				ISimpleMIDIInCallback& iSimpleMIDIInCallback)
#if WINDOWS
	: fMIDIInInstance(NULL)
#endif // WINDOWS
{
#if WINDOWS
	fMIDIInInstance = new NSInternals::CMMEMIDIInInstance(
		*this,
		iMIDIInPortID,
		iSimpleMIDIInCallback);
#endif // WINDOWS
#if MAC
	fMIDIInInstance = new NSInternals::CCoreMIDIInstance(*this, iMIDIInPortID, iSimpleMIDIInCallback);
#endif // MAC
}

CSimpleMIDIInInstance::~CSimpleMIDIInInstance(){
	delete fMIDIInInstance;
	fMIDIInInstance = NULL;
}

const TMIDIInPortID& CSimpleMIDIInInstance::GetPortID() const{
	return fMIDIInInstance->GetPortID();
}


} // NSSimpleMIDIAPI
