#include "MixerApp.h"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>



const char* kCouldNotGetDeviceInfo = "Could not get device info.";

#if REWIRE_MIXER_APP_API_VERSION != REWIRE_BUILD_VERSION(3,0,0)
#error	This version requires above version of ReWire.
#endif // REWIRE_MIXER_APP_API_VERSION != REWIRE_BUILD_VERSION(3,0,0)


typedef ReWire::ReWire_uint16_t GAIN_PARAM;
const GAIN_PARAM kMaxGainParam = 127;

//	0.5=6db per step,
//	0.25=3db per step.
const float kDampPerStep = 0.5f / 12.0f;

float gGainToKArray[kMaxGainParam + 1];

static void BuildGainArrays(){
	gGainToKArray[kMaxGainParam]=1.0f;
	{
		for(GAIN_PARAM gain7 = kMaxGainParam - 1; gain7 > 0; gain7--){
			float prev;

			prev = gGainToKArray[gain7 + 1];
			gGainToKArray[gain7] = prev - prev * kDampPerStep;
		}
		gGainToKArray[0] = 0.0f;
	}
}





/////////////////////////////////////		CMixerParams

CMixerParams::CMixerParams() {
	for (unsigned int i = 0; i < kMixerChannelCount; i++) {
		fMixerLevel[i] = 0;
		fSelectIx[i] = 0;
	}
	fPlayFlag = false;
}





/////////////////////////////////////		CReWireDevice


CReWireDevice::CReWireDevice(ReWire::ReWire_int32_t deviceIndex):
	fDeviceIndex(deviceIndex),
	fDeviceHandle(0),
	fOpenCount(0)
{
	ReWire::ReWirePrepareDeviceInfo(&fDeviceInfo);
	ReWire::ReWireError result=ReWire::RWMGetDeviceInfo(deviceIndex,&fDeviceInfo);
	if(ReWire::kReWireError_NoError!=result){
		assert(false);
		throw std::exception();
	}
	CheckInvariant();
}

CReWireDevice::~CReWireDevice(){
	CheckInvariant();
	assert(!IsOpen());
}

ReWire::ReWireError CReWireDevice::Open(){
	CheckInvariant();
	if(!IsOpen()){
		ReWire::ReWireError result=ReWire::RWMOpenDevice(fDeviceIndex,&fDeviceHandle);
		if(ReWire::kReWireError_NoError!=result){
			CheckInvariant();
			return result;
		}
	}
	fOpenCount++;
	CheckInvariant();
	return ReWire::kReWireError_NoError;
}

bool CReWireDevice::TryClose(){
	CheckInvariant();
	assert(IsOpen());
	bool closeSuccessful = false;
	if(1==fOpenCount){
		ReWire::ReWireError result = ReWire::RWMQuitPanelApp(fDeviceHandle);
		if (result == ReWire::kReWireError_NoError) {
			ReWire::ReWire_char_t okFlag = 0;
			result = ReWire::RWMIsCloseDeviceOK(fDeviceHandle, &okFlag);
			if (result == ReWire::kReWireError_NoError) {
				if (okFlag != 0) {
					result = ReWire::RWMCloseDevice(fDeviceHandle);
					
					if (result == ReWire::kReWireError_NoError) {
						fDeviceHandle = 0;
						closeSuccessful = true;
					}
					else {
						assert(false);
					}
				}
				else {
					std::cerr << "Closing the device is not OK at the moment" << std::endl;
				}
			}
			else {
				std::cerr << "RWMIsCloseDeviceOK() failed" << std::endl;
				assert(false);
			}
		}
		else {
			std::cerr << "Could not quit panel app" << std::endl;
		}
	}
	else {
		// Nothing to do yet - just decrease counter
		closeSuccessful = true;
	}
	if (closeSuccessful) {
		fOpenCount--;
	}
	CheckInvariant();
	return closeSuccessful;
}

bool CReWireDevice::IsOpen() const {
	CheckInvariant();
	if (fOpenCount > 0) {
		return true;
	}
	else {
		return false;
	}
}

bool CReWireDevice::IsCloseOK() const {
	CheckInvariant();
	ReWire::ReWire_char_t okFlag=0;
	ReWire::ReWireError result=ReWire::RWMIsCloseDeviceOK(fDeviceHandle,&okFlag);
	if (result != ReWire::kReWireError_NoError) {
		std::cerr << "RWMIsCloseDeviceOK() failed -> " <<  NSSampleUtils::GetErrorString(result) << std::endl;
		assert(false);
		return false;
	}
	
	return okFlag!=0;
}

const ReWire::ReWireDeviceInfo* CReWireDevice::GetDeviceInfo() const{
	CheckInvariant();
	return &fDeviceInfo;
}

const ReWire::TRWMDeviceHandle CReWireDevice::GetDeviceHandle() const{
	CheckInvariant();
	return fDeviceHandle;
}

ReWire::ReWire_int32_t CReWireDevice::GetDeviceIndex() const{
	CheckInvariant();
	return fDeviceIndex;
}

ReWire::ReWireError CReWireDevice::DriveAudio(const ReWire::ReWireDriveAudioInputParams* inputParams,ReWire::ReWireDriveAudioOutputParams* outputParams){
	CheckInvariant();
	assert(IsOpen());

	return ReWire::RWMDriveAudio(fDeviceHandle,inputParams,outputParams);
}

ReWire::ReWireError CReWireDevice::LaunchPanelApp(){
	CheckInvariant();
	assert(IsOpen());
	return ReWire::RWMLaunchPanelApp(fDeviceHandle);
}

char CReWireDevice::IsPanelAppLaunched(){
	CheckInvariant();
	assert(IsOpen());
	
	char isLaunchedFlag=0;
	ReWire::ReWireError result=ReWire::RWMIsPanelAppLaunched(fDeviceHandle,&isLaunchedFlag);
	if (result != ReWire::kReWireError_NoError) {
		std::cerr << "RWMIsPanelAppLaunched() failed -> " <<  NSSampleUtils::GetErrorString(result) << std::endl;
		throw std::exception();
	}
	return isLaunchedFlag;
}

ReWire::ReWireError CReWireDevice::QuitPanelApp(){
	CheckInvariant();
	assert(IsOpen());
	return ReWire::RWMQuitPanelApp(fDeviceHandle);
}

ReWire::ReWireError CReWireDevice::ReWireGetEventInfo(ReWire::ReWireEventInfo* eventInfo) const {
	CheckInvariant();
	assert(IsOpen());
	return ReWire::RWMGetEventInfo(fDeviceHandle,eventInfo);
}

ReWire::ReWireError CReWireDevice::ReWireGetEventBusInfo(ReWire::ReWire_uint16_t busIndex,ReWire::ReWireEventBusInfo* eventBusInfo) const {
	CheckInvariant();
	assert(IsOpen());
	return ReWire::RWMGetEventBusInfo(fDeviceHandle,busIndex,eventBusInfo);
}

ReWire::ReWireError CReWireDevice::ReWireGetEventChannelInfo(const ReWire::ReWireEventTarget* eventTarget,ReWire::ReWireEventChannelInfo* eventChannelInfo) const {
	CheckInvariant();
	assert(IsOpen());
	return ReWire::RWMGetEventChannelInfo(fDeviceHandle,eventTarget,eventChannelInfo);
}

ReWire::ReWireError CReWireDevice::ReWireGetControllerInfo(const ReWire::ReWireEventTarget* eventTarget,ReWire::ReWire_uint16_t controllerIndex,ReWire::ReWireEventControllerInfo* controllerInfo) const {
	assert(0!=fDeviceHandle);
	return ReWire::RWMGetControllerInfo(fDeviceHandle,eventTarget,controllerIndex,controllerInfo);
}

ReWire::ReWireError CReWireDevice::ReWireGetNoteInfo(const ReWire::ReWireEventTarget* eventTarget,ReWire::ReWire_uint16_t noteIndex,ReWire::ReWireEventNoteInfo* noteInfo) const {
	CheckInvariant();
	assert(IsOpen());
	return ReWire::RWMGetNoteInfo(fDeviceHandle,eventTarget,noteIndex,noteInfo);
}

void CReWireDevice::CheckInvariant() const
{
	assert(fOpenCount >= 0);
	if (fOpenCount == 0) {
		assert(fDeviceHandle == 0);
	}
	else {
		assert(fDeviceHandle != 0);
	}
}


// Help functions
std::string NSDeviceFunctions::GetDeviceName(const CReWireDevice& iDevice)
{
	const ReWire::ReWireDeviceInfo* info = iDevice.GetDeviceInfo();
	if (info) {
		return std::string(info->fName);
	}
	else {
		return std::string();
	}
}

void NSDeviceFunctions::GetBusNames(const CReWireDevice& iDevice, std::vector<std::string>& oNames, std::vector<int>& oIndexes)
{
	oNames = std::vector<std::string>();
	oIndexes = std::vector<int>();
	
	ReWire::ReWireEventInfo eventInfo;
	ReWire::ReWirePrepareEventInfo(&eventInfo);
	ReWire::ReWireError error = iDevice.ReWireGetEventInfo(&eventInfo);
	if (error == ReWire::kReWireError_NoError) {
		for (ReWire::ReWire_uint16_t busIndex = 0; busIndex < kReWireEventBusCount; busIndex++) {
			if (ReWire::ReWireIsBitInBitFieldSet(eventInfo.fUsedBusBitField, busIndex)) {
				ReWire::ReWireEventBusInfo eventBusInfo;
				ReWire::ReWirePrepareEventBusInfo(&eventBusInfo);
				error = iDevice.ReWireGetEventBusInfo(busIndex, &eventBusInfo);
				if (error == ReWire::kReWireError_NoError) {
					std::string busName = std::string(eventBusInfo.fBusName);
					if (busName == "") {
						char generatedName[256] = "";
						sprintf(generatedName, "Bus#%d", busIndex);
						busName = generatedName;
					}
					oNames.push_back(busName);
					oIndexes.push_back(busIndex);
				}
			}
		}
	}
}

void NSDeviceFunctions::GetChannelNames(const CReWireDevice& iDevice, int iBusIndex, std::vector<std::string>& oNames, std::vector<int>& oIndexes)
{
	oNames = std::vector<std::string>();
	oIndexes = std::vector<int>();
	
	ReWire::ReWireEventInfo eventInfo;
	ReWire::ReWirePrepareEventInfo(&eventInfo);
	ReWire::ReWireError error = iDevice.ReWireGetEventInfo(&eventInfo);
	if (error != ReWire::kReWireError_NoError) {
		return;
	}
	
	if (!ReWire::ReWireIsBitInBitFieldSet(eventInfo.fUsedBusBitField, iBusIndex)) {
		return;		
	}

	ReWire::ReWireEventBusInfo eventBusInfo;
	ReWire::ReWirePrepareEventBusInfo(&eventBusInfo);
	error = iDevice.ReWireGetEventBusInfo(iBusIndex, &eventBusInfo);
	if (error != ReWire::kReWireError_NoError) {
		return;				
	}
	
	for (ReWire::ReWire_uint16_t channelIndex = 0; channelIndex < kReWireEventChannelCount; channelIndex++) {
		if (ReWire::ReWireIsBitInBitFieldSet(eventBusInfo.fUsedChannelBitField, channelIndex)) {
			
			ReWire::ReWireEventTarget target;
			ReWire::ReWirePrepareEventTarget(&target, iBusIndex, channelIndex);
			ReWire::ReWireEventChannelInfo channelInfo;
			ReWire::ReWirePrepareEventChannelInfo(&channelInfo);
			error = iDevice.ReWireGetEventChannelInfo(&target, &channelInfo);
			
			if (error == ReWire::kReWireError_NoError) {
				std::string channelName = std::string(channelInfo.fChannelName);
				if (channelName == "") {
					char generatedName[256] = "";
					sprintf(generatedName, "Channel#%d", channelIndex);
					channelName = generatedName;
				}
				oNames.push_back(channelName);
				oIndexes.push_back(channelIndex);
			}
		}
	}
}



////////////////		CMixerApp

CMixerApp* CMixerApp::fCurrentApp = NULL;

CMixerApp::CMixerApp() :
	fSamplePos(0),
	fPPQPos(0),
	fShownPPQPos(0),
	fRequestedPPQPos(-1),
	fUpdateShownPPQPosFromRealFlag(true),
	fTempo(120000),
	fReWireOpenCount(0),
	fErrorBuffer(NULL),
	fErrorBufPos(0),
	fLoopStartPPQPos(0),
	fLoopEndPPQPos(4*kReWirePPQ*4),
	fIsLooping(true),
	fDeviceChangePending(false),
	fSignatureNumerator(4),
	fSignatureDenominator(4),
	fSampleRate(44100),
	fCurrentEventDevice(NULL),
	fCurrentEventBusDestination(255),
	fCurrentEventChannelDestination(255),
	fEventDestinationSelected(false),
	fAudioInstance(NULL)
{
	fBufferSizes.push_back(16384);

	for (unsigned int i = 0; i < kMixerChannelCount; i++){
		fAssignedDevices[i] = 0;
		fAssignedDeviceChannels[i] = 0;
	}
	
	BuildGainArrays();
	fCurrentApp = this;
	
	OpenSound();
}

CMixerApp::~CMixerApp(){
	Dispose();
	fCurrentApp = NULL;
}

void CMixerApp::Dispose(){
	if (fAudioInstance) {
		delete fAudioInstance;
		fAudioInstance = NULL;
	}

	fCurrentEventDevice = NULL;
	fCurrentEventBusDestination = 255;
	fCurrentEventChannelDestination = 255;
	delete [] fErrorBuffer;
	fErrorBuffer = NULL;
}

CMixerApp& CMixerApp::GetMixerApp() {
	assert(fCurrentApp != NULL);
	return *fCurrentApp;
}


void CMixerApp::Play()
{
	fParams.fPlayFlag = true;
}

void CMixerApp::Stop()
{
	fParams.fPlayFlag = false;
}


void CMixerApp::Fwd()
{
	fPPQPos += ((15360 / 4) * fSignatureDenominator * fSignatureNumerator);
}


void CMixerApp::Rew()
{
	TPPQPos ppqPerBar = ((15360 / 4) * fSignatureDenominator * fSignatureNumerator);
	if (fPPQPos > ppqPerBar) {
		fPPQPos -= ppqPerBar;
	}
	else {
		fPPQPos = 0;
	}
}



static ReWire::ReWire_int16_t ConvertMIDIPitchBendToValue(ReWire::ReWire_uint8_t inMidiData1, ReWire::ReWire_uint8_t inMidiData2){
	//	Pitchbend is 14 bit binary offset. 0x40,0x00 is center, 0x00,0x00 is min, 0x7f,0x7f is max.
	//	We store the value in a 16 bit signed: -8192 to +8191

	//	Make d=00hhhhhhhlllllll
	ReWire::ReWire_int16_t d=static_cast<ReWire::ReWire_int16_t>((inMidiData2 << 7) | inMidiData1);

	d -= 8192;

	return d;
}

void CMixerApp::ISimpleAudioRenderCallback_Render(NSSimpleAudioAPI::CSimpleAudioInstance& /*iInstance*/, NSSimpleAudioAPI::TFrameCount frames, const NSSimpleAudioAPI::TFloatSampleBuffers& outputBuffers)
{
	NSSampleUtils::CScopedLocker scopedLocker(fAssignedDevicesLock);

	if (frames > static_cast<TSamplePos>(kMaxAudioChunkToRender)) {
		// Just to make sure we don't render outside our capability
		for (unsigned int i = 0; i < outputBuffers.size(); i++) {
			float* buffer = outputBuffers[i];
			if (buffer != NULL) {
				for(TSamplePos frame = 0; frame < static_cast<TSamplePos>(frames); frame++) {
					buffer[frame] = 0.f;
				}
			}
		}
	}
	else {
		assert(frames <= static_cast<ReWire::ReWire_uint32_t>(kMaxAudioChunkToRender));

		bool deviceRequestPlay = false;
		bool deviceRequestStop = false;
		bool deviceRequestReposition = false;
		bool deviceRequestTempo = false;
		bool deviceRequestLoop = false;
		bool deviceRequestSign = false;

		TPPQPos deviceRequestedPosition = 0;
		TTempo deviceRequestedTempo = 0;
		TPPQPos deviceRequestLoopStartPosition = 0;
		TPPQPos deviceRequestLoopEndPosition = 0;
		bool deviceRequestLoopOn = false;
		unsigned int deviceRequestNumerator = 0;
		unsigned int deviceRequestDenominator = 0;

		// Clear output buffer.
		for (unsigned int i = 0; i < outputBuffers.size(); i++) {
			float* buffer = outputBuffers[i];
			if (buffer != NULL) {
				for (TSamplePos frame = 0; frame < static_cast<TSamplePos>(frames); frame++) {
					buffer[frame] = 0.f;
				}
			}
		}

		bool repositionFlag = false;

		if (fRequestedPPQPos != -1) {
			fPPQPos = fRequestedPPQPos;
			fRequestedPPQPos = -1;
			repositionFlag = true;
		}

		for (TReWireDeviceList::iterator i = fReWireDeviceList.begin(); i != fReWireDeviceList.end(); i++) {
			CReWireDevice* device = *i;
			//	Setup audio params for device.
			if (device->IsOpen()) {
				ReWire::ReWirePrepareDriveAudioInputParams(&fDriveAudioInputParams, kEventInputBufferSize, fEventInputBuffer);
				ReWire::ReWirePrepareDriveAudioOutputParams(&fDriveAudioOutputParams, kEventOutputBufferSize, fEventOutputBuffer);

				//	Let each mixer channel mark the channels of the device that it wants.
				for (unsigned int mixerChannel = 0; mixerChannel < kMixerChannelCount; mixerChannel++) {
					CReWireDevice* selectedDevice = fAssignedDevices[mixerChannel];
					long selectedDeviceChannel = fAssignedDeviceChannels[mixerChannel];
					bool mixerChannelOnFlag = (selectedDevice != NULL);
					if (mixerChannelOnFlag && (selectedDevice == device)) {
						ReWire::ReWireSetBitInBitField(fDriveAudioInputParams.fRequestedChannelsBitField, static_cast<ReWire::ReWire_uint16_t>(selectedDeviceChannel));
						fDriveAudioInputParams.fAudioBuffers[selectedDeviceChannel] = &fTempReWireDeviceBuffer[mixerChannel][0];
					}
				}

				fDriveAudioInputParams.fFramesToRender = frames;
				fDriveAudioInputParams.fPPQ15360TickOfBatchStart = static_cast<TPPQPos>(fPPQPos);
				fDriveAudioInputParams.fTempo = fTempo; // bpm * 1000
				if (fParams.fPlayFlag) {
					if (repositionFlag) {
						fDriveAudioInputParams.fPlayMode=ReWire::kReWirePlayModeChaseAndPlay;
					}
					else {
						fDriveAudioInputParams.fPlayMode=ReWire::kReWirePlayModeKeepPlaying;
					}
				}
				else {
					fDriveAudioInputParams.fPlayMode=ReWire::kReWirePlayModeStop;
				}

				fDriveAudioInputParams.fLoopStartPPQ15360Pos = static_cast<ReWire::ReWire_int32_t>(fLoopStartPPQPos);
				fDriveAudioInputParams.fLoopEndPPQ15360Pos = static_cast<ReWire::ReWire_int32_t>(fLoopEndPPQPos);
				fDriveAudioInputParams.fLoopOn = fIsLooping;
				fDriveAudioInputParams.fSignatureNumerator = fSignatureNumerator;
				fDriveAudioInputParams.fSignatureDenominator = fSignatureDenominator;

				if (fEventDestinationSelected) {
					if (fCurrentEventDevice == device) {
						static ReWire::ReWireEventTarget target;
						ReWire::ReWirePrepareEventTarget(&target, fCurrentEventBusDestination, fCurrentEventChannelDestination);

						CMIDIEvent event;
						while (fMIDIServer.PopEvent(event)) {
							if (fDriveAudioInputParams.fEventInBuffer.fCount < fDriveAudioInputParams.fEventInBuffer.fMaxEventCount) {
								ReWire::ReWireEvent* reWireEvent = &fDriveAudioInputParams.fEventInBuffer.fEventBuffer[fDriveAudioInputParams.fEventInBuffer.fCount];
								ReWire::ReWireMIDIEvent* midiEvent=ReWire::ReWireConvertToMIDIEvent(reWireEvent, &target);

								switch (event.GetType()) {
									case CMIDIEvent::MIDI_NOTEON:
										midiEvent->fMIDIEventType = ReWire::kReWireMIDINoteOn;
										midiEvent->fData1 = event.fData1;
										midiEvent->fData2 = event.fData2;
										fDriveAudioInputParams.fEventInBuffer.fCount++;
										break;
									case CMIDIEvent::MIDI_NOTEOFF:
										midiEvent->fMIDIEventType = ReWire::kReWireMIDINoteOff;
										midiEvent->fData1 = event.fData1;
										midiEvent->fData2 = event.fData2;
										fDriveAudioInputParams.fEventInBuffer.fCount++;
										break;
									case CMIDIEvent::MIDI_CONTROL:
										midiEvent->fMIDIEventType = ReWire::kReWireMIDIControl;
										midiEvent->fData1 = event.fData1;
										midiEvent->fData2 = event.fData2;
										fDriveAudioInputParams.fEventInBuffer.fCount++;
										break;
									case CMIDIEvent::MIDI_POLYPRESS:
										midiEvent->fMIDIEventType = ReWire::kReWireMIDIPolyPress;
										midiEvent->fData1 = event.fData1;
										midiEvent->fData2 = event.fData2;
										fDriveAudioInputParams.fEventInBuffer.fCount++;
										break;
									case CMIDIEvent::MIDI_PROGRAM:
										midiEvent->fMIDIEventType = ReWire::kReWireMIDIProgram;
										midiEvent->fData1 = event.fData1;
										midiEvent->fData2 = event.fData2;
										fDriveAudioInputParams.fEventInBuffer.fCount++;
										break;
									case CMIDIEvent::MIDI_MONOPRESS:
										midiEvent->fMIDIEventType = ReWire::kReWireMIDIMonopress;
										midiEvent->fData1 = event.fData1;
										midiEvent->fData2 = event.fData2;
										fDriveAudioInputParams.fEventInBuffer.fCount++;
										break;
									case CMIDIEvent::MIDI_PITCHBEND:
										midiEvent->fMIDIEventType = ReWire::kReWireMIDIPitchbend;
										midiEvent->fData1 = event.fData1;
										midiEvent->fData2 = event.fData2;
										fDriveAudioInputParams.fEventInBuffer.fCount++;
										break;
									case CMIDIEvent::MIDI_OTHER:
										//  do nothing
										break;
									default:
										assert(false);
								}
							}
						}
					}
				}

				device->DriveAudio(&fDriveAudioInputParams, &fDriveAudioOutputParams);

				/* Check for requests from device, last device requesting anything is the ruler! */
				for (unsigned int eventIndex = 0; eventIndex < fDriveAudioOutputParams.fEventOutBuffer.fCount; eventIndex++) {
					ReWire::ReWireEvent* reWireEvent = &fDriveAudioOutputParams.fEventOutBuffer.fEventBuffer[eventIndex];
					switch (reWireEvent->fEventType) {
						case ReWire::kReWireMIDIEvent:
							{
								const ReWire::ReWireMIDIEvent* event = ReWire::ReWireCastToMIDIEvent(reWireEvent);
								switch ((event->fEventType)) {
									case ReWire::kReWireMIDINoteOff:
										break;
									case ReWire::kReWireMIDINoteOn:
										break;
									case ReWire::kReWireMIDIPolyPress:
										break;
									case ReWire::kReWireMIDIControl:
										break;
									case ReWire::kReWireMIDIProgram:
										break;
									case ReWire::kReWireMIDIMonopress:
										break;
									case ReWire::kReWireMIDIPitchbend:
										{
											/* ReWire::ReWire_int16_t pitchBend = */ ConvertMIDIPitchBendToValue(static_cast<ReWire::ReWire_uint8_t>(event->fData1), static_cast<ReWire::ReWire_uint8_t>(event->fData2));
										}
										break;
								}
							}
							break;
						case ReWire::kReWireRequestRepositionEvent:
							{
								const ReWire::ReWireRequestRepositionEvent* event = ReWire::ReWireCastToRequestRepositionEvent(reWireEvent);
								deviceRequestReposition = true;
								deviceRequestedPosition = event->fPPQ15360Pos;
							}
							break;
						case ReWire::kReWireRequestSignatureEvent:
							{
								const ReWire::ReWireRequestSignatureEvent* event = ReWire::ReWireCastToRequestSignatureEvent(reWireEvent);
								deviceRequestSign = true;
								deviceRequestNumerator = event->fSignatureNumerator;
								deviceRequestDenominator = event->fSignatureDenominator;
							}
							break;
						case ReWire::kReWireRequestTempoEvent:
							{
								const ReWire::ReWireRequestTempoEvent* event = ReWire::ReWireCastToRequestTempoEvent(reWireEvent);
								deviceRequestTempo = true;
								deviceRequestedTempo = event->fTempo;
							}
							break;
						case ReWire::kReWireRequestLoopEvent:
							{
								const ReWire::ReWireRequestLoopEvent* event = ReWire::ReWireCastToRequestLoopEvent(reWireEvent);
								deviceRequestLoop = true;
								deviceRequestLoopStartPosition = event->fLoopStartPPQ15360Pos;
								deviceRequestLoopEndPosition = event->fLoopEndPPQ15360Pos;
								deviceRequestLoopOn = (event->fLoopOn != 0);
							}
							break;
						case ReWire::kReWireRequestPlayEvent:
							{
								deviceRequestStop = false;
								deviceRequestPlay = true;
							}
							break;
						case ReWire::kReWireRequestStopEvent:
							{
								deviceRequestStop = true;
								deviceRequestPlay = false;
							}
							break;
						default:
							break;
					}
				}

				//	Let each mixer channel pick out the data it wants and mix it to stereo output.
				{
					for (unsigned int mixerChannel = 0; mixerChannel < kMixerChannelCount; mixerChannel++) {
						CReWireDevice* selectedDevice = fAssignedDevices[mixerChannel];
						long selectedDeviceChannel = fAssignedDeviceChannels[mixerChannel];

						bool mixerChannelOnFlag = (selectedDevice != NULL);

						if (mixerChannelOnFlag && (selectedDevice == device)) {
							if (ReWire::ReWireIsBitInBitFieldSet(fDriveAudioOutputParams.fServedChannelsBitField, static_cast<ReWire::ReWire_uint16_t>(selectedDeviceChannel))) {
								ReWire::ReWire_uint8_t mixerLevelIndex = fParams.fMixerLevel[mixerChannel];
								assert(mixerLevelIndex <= 127);
								float mixerLevel = gGainToKArray[mixerLevelIndex];// / 127.0;

								bool isLeft = true;
								bool isRight = true;
								const ReWire::ReWireDeviceInfo* deviceInfo = selectedDevice->GetDeviceInfo();
								if (ReWire::ReWireIsBitInBitFieldSet(deviceInfo->fStereoPairsBitField, static_cast<ReWire::ReWire_uint16_t>(selectedDeviceChannel / 2))) {
									if ((1 & selectedDeviceChannel) != 0) {
										isLeft = false;
									}
									else {
										isRight = false;
									}
								}
								if (outputBuffers.size() > 0) {
									{
										float* buffer = outputBuffers[0];
										if (buffer != NULL) {
											if (isLeft) {
												float sampleSum = 0.0f;
												for (TSamplePos c = 0; c < static_cast<TSamplePos>(frames); c++) {
													if (fTempReWireDeviceBuffer[mixerChannel][c] != 0.0) {
														sampleSum += fTempReWireDeviceBuffer[mixerChannel][c];
													}
													buffer[c] += fTempReWireDeviceBuffer[mixerChannel][c] * mixerLevel;
												}
											}
										}
									}
									if (outputBuffers.size() > 1) {
										float* buffer = outputBuffers[1];
										if (buffer != NULL) {
											if (isRight) {
												for (TSamplePos c = 0; c < static_cast<TSamplePos>(frames); c++) {
													buffer[c] += fTempReWireDeviceBuffer[mixerChannel][c] * mixerLevel;
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		fSamplePos += frames;

		bool drivePosFlag = true;

		//	Act on device transport requests.
		if (deviceRequestPlay) {
			fParams.fPlayFlag = true;
			drivePosFlag = false;
			fDeviceChangePending = true;
		}
		if (deviceRequestStop) {
			fParams.fPlayFlag = false;
			drivePosFlag = false;
			fDeviceChangePending = true;
		}
		if (deviceRequestReposition) {
			RequestNewPosition(deviceRequestedPosition);
			drivePosFlag = false;
			fDeviceChangePending = true;
		}
		//	Drive the position this time?
		if (drivePosFlag) {
			if (fParams.fPlayFlag) {
				double tempo = fTempo;
				double ppq = kReWirePPQ;
				double dFrames = frames;
				double sampleRate = fSampleRate;
				TDecimalPPQPos posInc = tempo * (ppq * dFrames) / (60000.0 * sampleRate);
				TDecimalPPQPos prevPos = fPPQPos;
				fPPQPos += posInc;

				//	Is looping on and we just passed the loop end?
				if (fIsLooping && (prevPos < fLoopEndPPQPos) && (fPPQPos >= fLoopEndPPQPos)) {
					fPPQPos -= (fLoopEndPPQPos - fLoopStartPPQPos);
				}
			}
		}
		if (deviceRequestTempo) {
			assert(deviceRequestedTempo != 0);
			fTempo = deviceRequestedTempo;
			fDeviceChangePending = true;
		}
		if (deviceRequestLoop) {
			fLoopStartPPQPos = deviceRequestLoopStartPosition;
			fLoopEndPPQPos = deviceRequestLoopEndPosition;
			fIsLooping = deviceRequestLoopOn;
			fDeviceChangePending = true;
		}
		if (deviceRequestSign) {
			fSignatureNumerator = deviceRequestNumerator;
			fSignatureDenominator = deviceRequestDenominator;
			fDeviceChangePending = true;
		}
	}
}


void CMixerApp::OnIdle() {
	if (fReWireOpenCount > 0) {
		ReWire::RWMIdle();
	}
	if (fDeviceChangePending) {
		fDeviceChangePending = false;
	}
}


void CMixerApp::OpenSound() {
	NSSimpleAudioAPI::TAudioDriverID defaultDriverID = NSSimpleAudioAPI::GetDefaultAudioDriverID();
#if WINDOWS
	fAudioInstance = new NSSimpleAudioAPI::CSimpleAudioInstance(NULL, this, defaultDriverID);	
#endif // WINDOWS
#if MAC
	fAudioInstance = new NSSimpleAudioAPI::CSimpleAudioInstance(this, defaultDriverID);	
#endif // MAC

	fSampleRate = fAudioInstance->GetSampleRate();
}

const TReWireDeviceList* CMixerApp::GetDeviceList(){
	return &fReWireDeviceList;
}

void CMixerApp::OpenReWire(){
	if(0==fReWireOpenCount){

		ReWire::ReWireOpenInfo openInfo;
		ReWire::ReWirePrepareOpenInfo(&openInfo, fSampleRate, kMaxAudioChunkToRender);

		ReWire::ReWireError result=ReWire::RWMOpen(&openInfo);
		if (result == ReWire::kReWireError_NoError){
				fReWireOpenCount=1;
				//	Create all available devices and put in list
				{
					ReWire::ReWire_int32_t count=0;
					result=ReWire::RWMGetDeviceCount(&count);
					if(result!=ReWire::kReWireError_NoError){
						// Something seriously wrong - just close and quit
						ReWire::RWMClose();
						throw std::exception();
					}
					for(ReWire::ReWire_int32_t deviceIndex=0; deviceIndex < count; deviceIndex++){
						CReWireDevice* device=NULL;
						try{
							device=new CReWireDevice(deviceIndex);
							fReWireDeviceList.push_back(device);
						}catch(...){
							// Something went wrong when getting devce info - device shouldn't be open yet
							assert(!device->IsOpen());
							delete device;
							device=NULL;
							std::cerr << kCouldNotGetDeviceInfo << std::endl;
						}
					}
				}
				return;
		}
		else {
			std::cerr << "RWMOpen() failed -> " <<  NSSampleUtils::GetErrorString(result) << std::endl;
			throw std::exception();
		}
	}else{
		fReWireOpenCount++;
	}
}

bool CMixerApp::TryCloseReWire(){
	assert(fReWireOpenCount>0);
	bool closeSuccessful = false;
	if(1==fReWireOpenCount){
		//	Delete all devices - they should already be closed
		while(fReWireDeviceList.size() != 0){
			CReWireDevice* device = fReWireDeviceList.front();
			assert(!device->IsOpen());
			fReWireDeviceList.erase(fReWireDeviceList.begin());
			delete device;
			device=NULL;
		}
		ReWire::ReWire_char_t okFlag = 0;
		ReWire::ReWireError error = ReWire::RWMIsCloseOK(&okFlag);
		if (error == ReWire::kReWireError_NoError && okFlag != 0) {
			error = ReWire::RWMClose();
			assert(error == ReWire::kReWireError_NoError);
			closeSuccessful = true;
		}
		else {
			closeSuccessful = false;
		}

	}
	if (closeSuccessful) {
		fReWireOpenCount--;
	}
	return closeSuccessful;
}

bool CMixerApp::IsReWireOpen(){
	return fReWireOpenCount>0;
}


// Start of real-time code

void CMixerApp::RequestNewPosition(TPPQPos newPos){
	fRequestedPPQPos=newPos;
}

TPPQPos CMixerApp::GetPosition(){
	return static_cast<TPPQPos>(fPPQPos);
}

TPPQPos CMixerApp::GetShownPosition(){
	if(fUpdateShownPPQPosFromRealFlag){
		return static_cast<TPPQPos>(fPPQPos);
	}
	else{
		return fShownPPQPos;
	}
}

void CMixerApp::SetShownPosition(TPPQPos newPos){
	fShownPPQPos=newPos;
}

// End of real-time code


void CMixerApp::SetShowPosMode(bool updateFromRealPPQFlag){
	fUpdateShownPPQPosFromRealFlag=updateFromRealPPQFlag;
}

TTempo CMixerApp::GetTempo() const{
	return fTempo;
}

void CMixerApp::SetTempo(TTempo tempo){
	fTempo=tempo;
}

bool CMixerApp::IsLoopOn() const{
	return fIsLooping;
}

void CMixerApp::SetLoopOn(){
	fIsLooping=true;
}

void CMixerApp::SetLoopOff(){
	fIsLooping=false;
}

unsigned int CMixerApp::GetSignatureNumerator() const {
	return fSignatureNumerator;
}

unsigned int CMixerApp::GetSignatureDenominator() const {
	return fSignatureDenominator;
}

void CMixerApp::SetSignatureNumerator(unsigned int numerator) {
	fSignatureNumerator = numerator;
}

void CMixerApp::SetSignatureDenominator(unsigned int denominator) {
	fSignatureDenominator = denominator;
}

TPPQPos CMixerApp::GetLoopStartPosition() const{
	return static_cast<TPPQPos>(fLoopStartPPQPos);
}

TPPQPos CMixerApp::GetLoopEndPosition() const{
	return static_cast<TPPQPos>(fLoopEndPPQPos);
}

void CMixerApp::SetLoopStartPosition(TPPQPos ppqPos){
	fLoopStartPPQPos=ppqPos;
}

void CMixerApp::SetLoopEndPosition(TPPQPos ppqPos){
	fLoopEndPPQPos=ppqPos;
}

void CMixerApp::AssignDeviceAndChannelToReWireChannel(CReWireDevice* device,long deviceChannelIndex,unsigned int mixerChannelIndex){
	assert(mixerChannelIndex<kMixerChannelCount);
	NSSampleUtils::CScopedLocker scopedLocker(fAssignedDevicesLock);

	if(fAssignedDevices[mixerChannelIndex]==device){
		fAssignedDeviceChannels[mixerChannelIndex]=deviceChannelIndex;
	}
	else{
		if(NULL!=fAssignedDevices[mixerChannelIndex]){
			bool closeOK = fAssignedDevices[mixerChannelIndex]->TryClose();
			if (closeOK) {
				fAssignedDevices[mixerChannelIndex]=NULL;
				fAssignedDeviceChannels[mixerChannelIndex]=0;
			}
			else {
				std::cerr << "Failed to close device" << std::endl;
				throw std::exception();
			}
		}
		assert(fAssignedDevices[mixerChannelIndex]==NULL);
		assert(fAssignedDeviceChannels[mixerChannelIndex]==0);
		if(NULL!=device){
			ReWire::ReWireError error = device->Open();
			if (error != ReWire::kReWireError_NoError) {
				std::cerr << "device open failed -> " <<  NSSampleUtils::GetErrorString(error) << std::endl;
				throw std::exception();
			}
			
			fAssignedDevices[mixerChannelIndex]=device;
			fAssignedDeviceChannels[mixerChannelIndex]=deviceChannelIndex;
			
			ReWire::ReWireAudioInfo info;
			ReWire::ReWirePrepareAudioInfo(&info, fSampleRate, fBufferSizes.back());
			error = RWMSetAudioInfo(&info);
			if (error != ReWire::kReWireError_NoError) {
				std::cerr << "RWMSetAudioInfo() failed -> " <<  NSSampleUtils::GetErrorString(error) << std::endl;
				throw std::exception();
			}
		}
	}
}

void CMixerApp::GetAssignedDeviceAndChannelToReWireChannel(unsigned int mixerChannelIndex,CReWireDevice*& device,long& deviceChannelIndex){
	assert(mixerChannelIndex<kMixerChannelCount);
	NSSampleUtils::CScopedLocker scopedLocker(fAssignedDevicesLock);
	device=fAssignedDevices[mixerChannelIndex];
	deviceChannelIndex=fAssignedDeviceChannels[mixerChannelIndex];
}

CReWireDevice* CMixerApp::GetEventDestinationDevice() const{
	return fCurrentEventDevice;
}

void CMixerApp::SetEventDestinationDevice(CReWireDevice* device){
	if(NULL!=device){
		assert(device->IsOpen());
	}
	fCurrentEventDevice=device;
}

unsigned short CMixerApp::GetEventBusDestination() const{
	return fCurrentEventBusDestination;
}

void CMixerApp::SetEventBusDestination(unsigned short destination){
	fCurrentEventBusDestination=destination;
}

unsigned short CMixerApp::GetEventChannelDestination() const{
	return fCurrentEventChannelDestination;
}

void CMixerApp::SetEventChannelDestination(unsigned short destination){
	fCurrentEventChannelDestination=destination;
}

bool CMixerApp::IsEventDestinationSelected() const{
	return fEventDestinationSelected;
}

void CMixerApp::SetEventDestinationSelected(bool isSelected){
	fEventDestinationSelected=isSelected;
	if(!fEventDestinationSelected){
		fCurrentEventDevice=NULL;
		fCurrentEventBusDestination=255;
		fCurrentEventChannelDestination=255;
	}
}


const CMixerParams& CMixerApp::GetParams() const
{
	return fParams;
}


void CMixerApp::SetParams(const CMixerParams& params)
{
	fParams = params;
}
