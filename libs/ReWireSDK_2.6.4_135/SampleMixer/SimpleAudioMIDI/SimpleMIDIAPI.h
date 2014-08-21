#pragma once

#include <vector>

#if WINDOWS
#include <MMSystem.h>
#endif // WINDOWS

#if MAC
#include <CoreMIDI/CoreMIDI.h>
#endif // MAC


namespace NSSimpleMIDIAPI {

#if WINDOWS

typedef UINT TMIDIInPortID;
typedef MIDIINCAPS TMIDIInPortCaps;
typedef DWORD TMilliSecond;
typedef UINT8 TMIDIByte;

namespace NSInternals {
	class CMMEMIDIInInstance;
} // NSInternals

#endif // WINDOWS
	
#if MAC
typedef MIDIEndpointRef TMIDIInPortID;
typedef std::string TMIDIInPortCaps;
typedef MIDITimeStamp TMilliSecond;
typedef Byte TMIDIByte;
	
namespace NSInternals {
	class CCoreMIDIInstance;
} // NSInternals
	
#endif // MAC



typedef std::vector<TMIDIInPortID> TMIDIInPortIDs;


TMIDIInPortIDs GetMIDIInPortIDs();
bool GetMIDIInPortCaps(const TMIDIInPortID& iMIDIInPortID, TMIDIInPortCaps& oMIDIInPortCaps);








class ISimpleMIDIInCallback;


class CSimpleMIDIInInstance {
	public: CSimpleMIDIInInstance(
				const TMIDIInPortID& iMIDIInPortID,
				ISimpleMIDIInCallback& iSimpleMIDIInCallback);
	public: ~CSimpleMIDIInInstance();

	public: const TMIDIInPortID& GetPortID() const;

#if WINDOWS
	private: NSInternals::CMMEMIDIInInstance* fMIDIInInstance;
#endif // WINDOWS
#if MAC
	private: NSInternals::CCoreMIDIInstance* fMIDIInInstance;
#endif // MAC
};



class ISimpleMIDIInCallback {
	protected: virtual ~ISimpleMIDIInCallback() {}

	public: virtual void ISimpleMIDIInCallback_OnShortMIDI(
				CSimpleMIDIInInstance& iInstance,
				TMilliSecond iTimeStampMS,
				TMIDIByte iStatus,
				TMIDIByte iData1,
				TMIDIByte iData2) = 0;
};

} // NSSimpleMIDIAPI
