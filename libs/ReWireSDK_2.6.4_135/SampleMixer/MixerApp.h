#ifndef __MixerApp
#define __MixerApp

#include "ReWireSDK/ReWireMixerAPI.h"
#include "ReWireSDK/ReWireDeviceAPI.h"
#include "ReWireSDK/ReWirePanelAPI.h"

#include "SimpleAudioMIDI/SimpleAudioAPI.h"
#include "MIDI.h"
#include "SampleUtils/SampleUtils.h"


#include "Mixer.h"
#include <vector>

typedef ReWire::ReWire_int32_t TSamplePos;
typedef ReWire::ReWire_int32_t TPPQPos;
typedef ReWire::ReWire_int32_t TSampleRate;
typedef ReWire::ReWire_int32_t TTempo;
typedef double TDecimalPPQPos;

static const TSamplePos kMaxAudioChunkToRender=16384;

class CReWireDevice {
	public: CReWireDevice(ReWire::ReWire_int32_t deviceIndex);
	public: virtual ~CReWireDevice();

	public: ReWire::ReWireError Open();
	public: bool TryClose();
	public: bool IsOpen() const;
	public: bool IsCloseOK() const;
	public: ReWire::ReWireError DriveAudio(const ReWire::ReWireDriveAudioInputParams* inputParams,ReWire::ReWireDriveAudioOutputParams* outputParams);
	public: const ReWire::ReWireDeviceInfo* GetDeviceInfo() const;
	public: const ReWire::TRWMDeviceHandle GetDeviceHandle() const;
	public: ReWire::ReWire_int32_t GetDeviceIndex() const;

	public: ReWire::ReWireError LaunchPanelApp();
	public: char IsPanelAppLaunched();
	public: ReWire::ReWireError QuitPanelApp();

	public: ReWire::ReWireError ReWireGetEventInfo(ReWire::ReWireEventInfo* eventInfo) const;
	public: ReWire::ReWireError ReWireGetEventBusInfo(ReWire::ReWire_uint16_t busIndex,ReWire::ReWireEventBusInfo* eventBusInfo) const;
	public: ReWire::ReWireError ReWireGetEventChannelInfo(const ReWire::ReWireEventTarget* eventTarget,ReWire::ReWireEventChannelInfo* eventChannelInfo) const;
	public: ReWire::ReWireError ReWireGetControllerInfo(const ReWire::ReWireEventTarget* eventTarget,ReWire::ReWire_uint16_t controllerIndex,ReWire::ReWireEventControllerInfo* controllerInfo) const;
	public: ReWire::ReWireError ReWireGetNoteInfo(const ReWire::ReWireEventTarget* eventTarget,ReWire::ReWire_uint16_t noteIndex,ReWire::ReWireEventNoteInfo* noteInfo) const;
	
	private: void CheckInvariant() const;

	private: ReWire::ReWireDeviceInfo fDeviceInfo;
	private: ReWire::TRWMDeviceHandle fDeviceHandle;
	private: ReWire::ReWire_int32_t fDeviceIndex;
	private: int fOpenCount;

	private: CReWireDevice();
	private: CReWireDevice(const CReWireDevice& other);
	private: CReWireDevice& operator=(const CReWireDevice& other);
};

namespace NSDeviceFunctions {
	std::string GetDeviceName(const CReWireDevice& iDevice);
	void GetBusNames(const CReWireDevice& iDevice, std::vector<std::string>& oNames, std::vector<int>& oIndexes);
	void GetChannelNames(const CReWireDevice& iDevice, int iBusIndex, std::vector<std::string>& oNames, std::vector<int>& oIndexes);
} // NSDeviceFunctions

typedef std::vector<CReWireDevice*> TReWireDeviceList;

class CMixerApp :
	public NSSimpleAudioAPI::ISimpleAudioRenderCallback
{
	public: CMixerApp();
	public: virtual ~CMixerApp();
	private: void Dispose();

	public: static CMixerApp& GetMixerApp();
	public: void Play();
	public: void Stop();
	public: void Fwd();
	public: void Rew();

	// ISimpleAudioRenderCallback
	public: virtual void ISimpleAudioRenderCallback_Render(NSSimpleAudioAPI::CSimpleAudioInstance& iInstance, NSSimpleAudioAPI::TFrameCount frames, const NSSimpleAudioAPI::TFloatSampleBuffers& outputBuffers);
	
	public: void OnIdle();

	private: void OpenSound();

	public: const TReWireDeviceList* GetDeviceList();

	public: void OpenReWire();
	public: bool TryCloseReWire();
	public: bool IsReWireOpen();

	// Realtime code
	public: void RequestNewPosition(TPPQPos newPos);
	public: TPPQPos GetPosition();
	public: TPPQPos GetShownPosition();
	public: void SetShownPosition(TPPQPos newPos);

	public: void SetShowPosMode(bool updateFromRealPPQFlag);
	public: TTempo GetTempo() const;
	public: void SetTempo(TTempo tempo);
	public: bool IsLoopOn() const;
	public: void SetLoopOn();
	public: void SetLoopOff();
	public: unsigned int GetSignatureNumerator() const;
	public: unsigned int GetSignatureDenominator() const;
	public: void SetSignatureNumerator(unsigned int numerator);
	public: void SetSignatureDenominator(unsigned int denominator);
	public: TPPQPos GetLoopStartPosition() const;
	public: TPPQPos GetLoopEndPosition() const;
	public: void SetLoopStartPosition(TPPQPos ppqPos);
	public: void SetLoopEndPosition(TPPQPos ppqPos);

	public: void AssignDeviceAndChannelToReWireChannel(CReWireDevice* device, long deviceChannelIndex, unsigned int mixerChannelIndex);
	public: void GetAssignedDeviceAndChannelToReWireChannel(unsigned int mixerChannelIndex, CReWireDevice*& device, long& deviceChannelIndex);

	public: CReWireDevice* GetEventDestinationDevice() const;
	public: void SetEventDestinationDevice(CReWireDevice* device);

	public: unsigned short GetEventBusDestination() const;
	public: void SetEventBusDestination(unsigned short destination);

	public: unsigned short GetEventChannelDestination() const;
	public: void SetEventChannelDestination(unsigned short destination);

	public: bool IsEventDestinationSelected() const;
	public: void SetEventDestinationSelected(bool isSelected);
	
	public: const CMixerParams& GetParams() const;
	public: void SetParams(const CMixerParams& params);


	// data
	private: static CMixerApp* fCurrentApp;
	private: CMixerParams fParams;
	public: TSamplePos fSamplePos;
	public: TTempo fTempo;
	private: CReWireDevice* fCurrentEventDevice;
	private: ReWire::ReWire_uint16_t fCurrentEventBusDestination;
	private: ReWire::ReWire_uint16_t fCurrentEventChannelDestination;
	private: bool fEventDestinationSelected;

	public: TReWireDeviceList fReWireDeviceList;

	private: CReWireDevice* fAssignedDevices[kMixerChannelCount];
	private: long fAssignedDeviceChannels[kMixerChannelCount];
	private: NSSampleUtils::CSpinLock fAssignedDevicesLock;

	private: TDecimalPPQPos fPPQPos;
	private: TPPQPos fShownPPQPos;
	private: TPPQPos fRequestedPPQPos;
	private: bool fUpdateShownPPQPosFromRealFlag;
	private: unsigned int fReWireOpenCount;
	private: TDecimalPPQPos fLoopStartPPQPos;
	private: TDecimalPPQPos fLoopEndPPQPos;
	private: bool fIsLooping;
	private: bool fDeviceChangePending;
	private: ReWire::ReWire_uint32_t fSignatureNumerator;
	private: ReWire::ReWire_uint32_t fSignatureDenominator;

	private: unsigned int fErrorBufPos;
	private: char* fErrorBuffer;

	private: ReWire::ReWireDriveAudioInputParams fDriveAudioInputParams;
	private: ReWire::ReWireDriveAudioOutputParams fDriveAudioOutputParams;

	private: enum {
		kEventInputBufferSize = 1024,
		kEventOutputBufferSize = 4096
	};
	private: ReWire::ReWireEvent fEventInputBuffer[kEventInputBufferSize];
	private: ReWire::ReWireEvent fEventOutputBuffer[kEventOutputBufferSize];
	private: float fTempReWireDeviceBuffer[kMixerChannelCount][kMaxAudioChunkToRender];

	private: TSampleRate fSampleRate;
	private: std::vector<ReWire::ReWire_uint32_t> fBufferSizes;

	private: CMIDIServer fMIDIServer;
	private: NSSimpleAudioAPI::CSimpleAudioInstance* fAudioInstance;
};


#endif // __MixerApp
