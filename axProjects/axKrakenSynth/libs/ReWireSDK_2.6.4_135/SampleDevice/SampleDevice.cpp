#include "ReWireSDK/ReWireDeviceAPI.h"

#include "ReWireSDK/RWDEFAPI.h"

#include "SampleDeviceProtocol.h"

#include <cassert>
#include <cmath>
#include <cstring>


#if REWIRE_DEVICE_DLL_API_VERSION != REWIRE_BUILD_VERSION(1, 0, 0)
#error This version requires above version of ReWire.
#endif // REWIRE_DEVICE_DLL_API_VERSION != REWIRE_BUILD_VERSION(1, 0, 0)



typedef ReWire::ReWire_int32_t TSamplePos;
typedef ReWire::ReWire_int32_t TPPQPos;
typedef ReWire::ReWire_int32_t TSampleRate;
typedef double TDecimalPPQPos;

static const float kHighOutSignalStart = 1.0f;
static const float kLowOutSignalStart = 0.5f;

static const TPPQPos kPPQResolution = kReWirePPQ;

const ReWire::ReWire_uint32_t kPipeBufferSize = 8192;





/////////////////////////			CMetronome


/*
	This little class synthesizes a metronome sound that is sent out on the Sample Device output.
*/



class CMetronome{
	public: CMetronome();
	public: void Render(TPPQPos inNewPos, ReWire::ReWire_uint32_t inNumerator, ReWire::ReWire_uint32_t inDenominator, float* outBuffer, long framesToRender, TSampleRate inSampleRate);
	public: void ChaseTo(TPPQPos inPos);


	////////////		Data.
	private: TPPQPos fCurrentPos;
	private: float fOutSignal;
	private: float fFlipCnt;
	private: ReWire::ReWire_int32_t fResetIncValue;
	private: ReWire::ReWire_int32_t fFlipResetValue;
	private: ReWire::ReWire_int32_t fHPModulo;
	private: ReWire::ReWire_int32_t fLPModulo;
	private: ReWire::ReWire_int32_t fResetValueModulo;
};








//////////////////////////		CReWireSampleDevice



/*
	This is an internal object that contains most of the functionallity and state of the ReWire sample device.
	Only one or none instances ever exists of this class.
*/



class CReWireSampleDevice  {
	public: CReWireSampleDevice();
	public: virtual ~CReWireSampleDevice();
	public: ReWire::ReWireError Open(const ReWire::ReWireOpenInfo* openDeviceInfo);
	public: bool IsCloseOK();
	public: void Close();
	public: void DriveAudio(const ReWire::ReWireDriveAudioInputParams* inputParams, ReWire::ReWireDriveAudioOutputParams* outputParams);
	public: void SetAudioInfo(const ReWire::ReWireAudioInfo* audioInfo);
	public: bool IsPanelConnected();
	public: bool QuitPanelApp();



	/////////////////	Internals.
	private: void ParseMessages();
	private: void MakePitchTab(float inSampleRate);


	/////////////////	Data.
	private: ReWire::TRWDPortHandle fDevicePortHandle;

	private: float fVoiceCurrentValue[4];
	private: ReWire::ReWire_uint16_t fFreqs[4];
	private: bool fPlayFlag;
	private: ReWire::ReWire_uint32_t fVoiceTriggered[4];
	private: ReWire::ReWire_uint32_t fRequestedTempo;
	private: ReWire::ReWire_uint32_t fCurrentTempo;
	private: bool fIsTempoChangeRequested;
	private: bool fIsPlayRequested;
	private: bool fIsStopRequested;
	private: TPPQPos fLoopStart;
	private: TPPQPos fLoopEnd;
	private: bool fIsLooping;
	private: ReWire::ReWire_uint8_t fSignatureNumerator;
	private: ReWire::ReWire_uint8_t fSignatureDenominator;
	private: TSampleRate fCurrentSampleRate;
	private: CMetronome fMetronome;
};

static CReWireSampleDevice* gSampleDevice=NULL;






//////////////////////////			CMetronome







CMetronome::CMetronome():
	fCurrentPos(0),
	fOutSignal(0),
	fFlipResetValue(0),
	fFlipCnt(0.0f),
	fResetIncValue(6),
	fHPModulo(29),
	fLPModulo(55),
	fResetValueModulo(fLPModulo)
{
}

void CMetronome::ChaseTo(TPPQPos inPos) {
	fCurrentPos = inPos;
}

static ReWire::ReWire_int32_t AlignValueUpwards(ReWire::ReWire_int32_t value, ReWire::ReWire_int32_t alignment){
	assert(alignment > 0);
	ReWire::ReWire_int32_t rem = value % alignment;
	if (rem != 0) {
		if (value >= 0) {
#if WINDOWS
			__int64 tmp = value;
#endif // WINDOWS
#if MAC
			int64_t tmp = value;
#endif // MAC
			tmp += alignment;
			tmp -= rem;
			if(tmp > 0x7fffffff){
				tmp = 0x7fffffff;
			}
			return static_cast<ReWire::ReWire_int32_t>(tmp);
		} else {
			return value - rem;
		}
	}
	return value;
}

void CMetronome::Render(TPPQPos inNewPos, ReWire::ReWire_uint32_t inNumerator, ReWire::ReWire_uint32_t inDenominator, float* outBuffer, long framesToRender, TSampleRate inSampleRate) {
	assert(inDenominator != 0);
	assert(inNumerator != 0);
	assert(inSampleRate != 0);

	TPPQPos ppqPerBeat = (kPPQResolution * 4) / inDenominator;
	TPPQPos nextTrigPos = AlignValueUpwards(fCurrentPos, ppqPerBeat);
	//	Trigg metronome sound.
	while (inNewPos >= nextTrigPos) {
		TPPQPos ppqPerBar = ppqPerBeat * inNumerator;

		//	Big bleep?
		if ((nextTrigPos % ppqPerBar) == 0) {
			fOutSignal = kHighOutSignalStart;
			fFlipResetValue = fResetIncValue;
			fFlipCnt = static_cast<float>(fResetIncValue);
			fResetValueModulo = fHPModulo;
		}
		//	Small bleep...
		else {
			fOutSignal = kLowOutSignalStart;
			fFlipResetValue = fResetIncValue;
			fFlipCnt = static_cast<float>(fResetIncValue);
			fResetValueModulo = fLPModulo;
		}
		fCurrentPos += ppqPerBeat;
		nextTrigPos = AlignValueUpwards(fCurrentPos, ppqPerBeat);
	}

	//	Generate audio.
	float decrementValue = 44100.0f / inSampleRate;
	float decay = std::pow(0.997f, decrementValue);
	for (TSamplePos pos = 0; pos < framesToRender; pos++) {
		fFlipCnt -= decrementValue;
		if (fFlipCnt < 0.0f) {
			fFlipCnt = static_cast<float>(fFlipResetValue);
			fOutSignal = -fOutSignal;
			fFlipResetValue += fResetIncValue;
			fFlipResetValue %= fResetValueModulo;
		}
		outBuffer[pos] = fOutSignal;
		fOutSignal = fOutSignal * decay;
	}
}











//////////////////////			CReWireSampleDevice


bool CReWireSampleDevice::IsPanelConnected() {
	if (fDevicePortHandle != 0) {
		if (ReWire::RWDComCheckConnection(fDevicePortHandle) == ReWire::kReWireError_PortConnected) {
			return false;
		}
	}
	return false;
}

bool CReWireSampleDevice::QuitPanelApp() {
	if (0 != fDevicePortHandle) {
		ReWire::ReWire_uint8_t message[] = {kRequestQuit};
		ReWire::ReWireError result = ReWire::RWDComSend(fDevicePortHandle, 0, sizeof(message), message);
		if ((ReWire::kReWireError_NoError == result) || (ReWire::kReWireError_PortNotConnected == result)) {
			return true;
		}
		else {
			return false;
		}
	}
	return true;
}

CReWireSampleDevice::CReWireSampleDevice() :
	fDevicePortHandle(0),
	fPlayFlag(false),
	fIsTempoChangeRequested(false),
	fIsPlayRequested(false),
	fIsStopRequested(false),
	fLoopStart(0),
	fLoopEnd(0),
	fIsLooping(false),
	fSignatureNumerator(4),
	fSignatureDenominator(4),
	fCurrentTempo(140 * 1000),
	fCurrentSampleRate(44100)
{
	fFreqs[0] = 40 * 6 + 400;
	fVoiceTriggered[0] = false;
	fFreqs[1] = 50 * 6 + 400;
	fVoiceTriggered[1] = false;
	fFreqs[2] = 60 * 6 + 400;
	fVoiceTriggered[2] = false;
	fFreqs[3] = 80 * 6 + 400;
	fVoiceTriggered[3] = false;

	fVoiceCurrentValue[0] = 0.0f;
	fVoiceCurrentValue[1] = 0.0f;
	fVoiceCurrentValue[2] = 0.0f;
	fVoiceCurrentValue[3] = 0.0f;
}

CReWireSampleDevice::~CReWireSampleDevice() {
}

ReWire::ReWireError CReWireSampleDevice::Open(const ReWire::ReWireOpenInfo* openDeviceInfo) {
	assert(0 == fDevicePortHandle);
	fCurrentSampleRate = openDeviceInfo->fAudioInfo.fSampleRate;

	ReWire::ReWireError result=ReWire::RWDOpen();
	if (ReWire::kReWireError_NoError == result) {
		ReWire::ReWirePipeInfo pipeInfo[1];
		ReWire::ReWirePreparePipeInfo(&pipeInfo[0], kPipeBufferSize, kPipeBufferSize);
		result = ReWire::RWDComCreate(kSampleDeviceSignature, 1, pipeInfo, &fDevicePortHandle);
		if (ReWire::kReWireError_NoError == result) {
			if (ReWire::RWDComCreate(kSampleDeviceSignature, 1, pipeInfo, &fDevicePortHandle) == ReWire::kReWireError_AlreadyExists) {
				return ReWire::kReWireError_NoError;
			}
			else {
				assert(false);
			}
			ReWire::RWDComDestroy(fDevicePortHandle);
			fDevicePortHandle = 0;
		}
		ReWire::RWDClose();
	}
	return result;
}

bool CReWireSampleDevice::IsCloseOK() {
	if (ReWire::RWDComCheckConnection(fDevicePortHandle) == ReWire::kReWireError_PortConnected) {
		return false;
	}
	else {
		return true;
	}
}

void CReWireSampleDevice::Close() {
	ReWire::RWDComDestroy(fDevicePortHandle);
	fDevicePortHandle = 0;
	ReWire::RWDClose();
}


static ReWire::ReWire_uint32_t Unpack32BitUnsignedBig(const ReWire::ReWire_uint8_t binary[]){
	return (static_cast<ReWire::ReWire_uint32_t>(binary[3]) ) |
		(static_cast<ReWire::ReWire_uint32_t>(binary[2]) << 8) |
		(static_cast<ReWire::ReWire_uint32_t>(binary[1]) << 16) |
		(static_cast<ReWire::ReWire_uint32_t>(binary[0]) << 24);
}

void CReWireSampleDevice::ParseMessages() {
	ReWire::ReWire_uint16_t bytesAvailable = 0;
	ReWire::ReWireError result = ReWire::RWDComBytesAvailable(fDevicePortHandle, 0, &bytesAvailable);
	while ((ReWire::kReWireError_NoError == result) && (bytesAvailable > 0)) {
		ReWire::ReWire_uint8_t sMessageBody[16];	// We know for this sample device that no messages are longer than a few bytes
		ReWire::ReWire_uint16_t messageSize = 0;
		result = ReWire::RWDComRead(fDevicePortHandle, 0, &messageSize, sMessageBody);
		assert(ReWire::kReWireError_NoError == result);
		switch (sMessageBody[0]) {
			case kSetTempo:
				{
					assert(messageSize == 5);
					fRequestedTempo = Unpack32BitUnsignedBig(&sMessageBody[1]);
					fIsTempoChangeRequested = true;
				}
				break;
			case kPlay:
				fIsPlayRequested = true;
				fIsStopRequested = false;
				break;
			case kStop:
				fIsStopRequested = true;
				fIsPlayRequested = false;
				break;
			default:
				assert(false);
				break;
		}
		bytesAvailable = 0;
		result = ReWire::RWDComBytesAvailable(fDevicePortHandle, 0, &bytesAvailable);
	}
}

void CReWireSampleDevice::SetAudioInfo(const ReWire::ReWireAudioInfo* audioInfo) {
	fCurrentSampleRate = audioInfo->fSampleRate;
}

void CReWireSampleDevice::DriveAudio(const ReWire::ReWireDriveAudioInputParams* inputParams,ReWire::ReWireDriveAudioOutputParams* outputParams) {
	assert(0 != fCurrentSampleRate);
	assert(inputParams != NULL);
	assert(outputParams != NULL);

	fCurrentTempo = inputParams->fTempo;
	ParseMessages();

	if (fIsTempoChangeRequested) {
		ReWire::ReWireEvent* event = &outputParams->fEventOutBuffer.fEventBuffer[outputParams->fEventOutBuffer.fCount];
		ReWire::ReWireRequestTempoEvent* tempoEvent = ReWire::ReWireConvertToRequestTempoEvent(event);
		tempoEvent->fTempo = fRequestedTempo;
		outputParams->fEventOutBuffer.fCount++;
		fIsTempoChangeRequested = false;
	}
	if (fIsPlayRequested) {
		ReWire::ReWireEvent* event = &outputParams->fEventOutBuffer.fEventBuffer[outputParams->fEventOutBuffer.fCount];
		/*ReWire::ReWireRequestPlayEvent* playEvent =*/ ReWire::ReWireConvertToRequestPlayEvent(event);
		outputParams->fEventOutBuffer.fCount++;
		fIsPlayRequested = false;
		assert(false == fIsStopRequested);
	}
	else if (fIsStopRequested) {
		ReWire::ReWireEvent* event = &outputParams->fEventOutBuffer.fEventBuffer[outputParams->fEventOutBuffer.fCount];
		/*ReWire::ReWireRequestStopEvent* stopEvent =*/ ReWire::ReWireConvertToRequestStopEvent(event);
		outputParams->fEventOutBuffer.fCount++;
		fIsStopRequested = false;
	}

	// Verify validity of input parameters

	fLoopStart = inputParams->fLoopStartPPQ15360Pos;
	fLoopEnd = inputParams->fLoopEndPPQ15360Pos;
	fIsLooping = inputParams->fLoopOn ? 1 : 0;
	fSignatureNumerator = inputParams->fSignatureNumerator;
	fSignatureDenominator = inputParams->fSignatureDenominator;

	bool doReposition = false;
	bool doPlay = false;
	switch (inputParams->fPlayMode) {
		case ReWire::kReWirePlayModeStop:
			doReposition = true;
			doPlay = false;
			break;
		case ReWire::kReWirePlayModeKeepPlaying:
			doReposition = false;
			doPlay = true;
			break;
		case ReWire::kReWirePlayModeChaseAndPlay:
			doReposition = true;
			doPlay = true;
			break;
		default:
			assert(false);
			break;
	}
	if (doReposition) {
		fMetronome.ChaseTo(inputParams->fPPQ15360TickOfBatchStart);
	}

	// Parse MIDI
	ReWire::ReWire_uint32_t eventCount = inputParams->fEventInBuffer.fCount;
	if (eventCount > 0) {
		for (ReWire::ReWire_uint32_t index = 0; index < eventCount; index++) {
			ReWire::ReWireEvent* event = &inputParams->fEventInBuffer.fEventBuffer[index];
			switch (event->fEventType) {
				case ReWire::kReWireMIDIEvent:
					{
						const ReWire::ReWireMIDIEvent* midiEvent = ReWire::ReWireCastToMIDIEvent(event);
						if (midiEvent->fEventTarget.fMIDIBusIndex == 1) {
							ReWire::ReWire_uint16_t freq = 0;
							ReWire::ReWire_uint32_t voice = 0;
							switch (midiEvent->fEventTarget.fChannel) {
								case 0:
									voice = 0;
									freq = static_cast<ReWire::ReWire_uint16_t>(std::exp(std::log(2.f)*(midiEvent->fData1-60)/12)*440*65535.f/fCurrentSampleRate);
									break;
								case 1:
									voice = 1;
									freq = static_cast<ReWire::ReWire_uint16_t>(std::exp(std::log(2.f)*(midiEvent->fData1-60)/12)*440*65535.f/fCurrentSampleRate);
									break;
								case 2:
									voice = 2;
									freq = static_cast<ReWire::ReWire_uint16_t>(std::exp(std::log(2.f)*(midiEvent->fData1-60)/12)*440*65535.f/fCurrentSampleRate);
									break;
								default:
									continue;	// Not valid...
									break;
							}
							switch (midiEvent->fMIDIEventType) {
								case ReWire::kReWireMIDINoteOn:
									fVoiceTriggered[voice]++;
									fFreqs[voice] = freq;
									break;
								case ReWire::kReWireMIDINoteOff:
									if (fVoiceTriggered[voice] > 0) {
										fVoiceTriggered[voice]--;
									}
									break;
							}
						}
					}
					break;
				default:
					continue;
					break;
			}
		}
	}

	// Sawtooths
	for (unsigned int i = 0; i < 3; i++) {
		float  add = static_cast<float>(fFreqs[i]);
		float* buf = inputParams->fAudioBuffers[i];

		if (ReWire::ReWireIsBitInBitFieldSet(inputParams->fRequestedChannelsBitField, i)) {
			for (ReWire::ReWire_uint32_t c = 0; c < inputParams->fFramesToRender; c++) {
				if (fVoiceTriggered[i] > 0) {
					fVoiceCurrentValue[i] += add;
					if (fVoiceCurrentValue[i] > 32767.0f) {
						fVoiceCurrentValue[i] -= 65535.0f;
					}
					buf[c] = fVoiceCurrentValue[i] * (1.0f / 32767.0f);
				}
				else {
					buf[c] = 0.0f;
				}
			}
			// Don't serve the third channel. Checks that mixers respect the served flag.
			if (i!=2) {
				ReWire::ReWireSetBitInBitField(outputParams->fServedChannelsBitField, i);
			}
		}
	}

	// Metronome
	{
		fPlayFlag = doPlay;

		if (fPlayFlag) {
			if (ReWire::ReWireIsBitInBitFieldSet(inputParams->fRequestedChannelsBitField, 3)){
				TDecimalPPQPos posInc = static_cast<TDecimalPPQPos>(inputParams->fTempo) * (static_cast<TDecimalPPQPos>(kReWirePPQ) * static_cast<TDecimalPPQPos>(inputParams->fFramesToRender)) / (60000.0 * static_cast<TDecimalPPQPos>(fCurrentSampleRate));
				TPPQPos newPos = static_cast<TPPQPos>(inputParams->fPPQ15360TickOfBatchStart + posInc);
				float* buf = inputParams->fAudioBuffers[3];
				fMetronome.ChaseTo(inputParams->fPPQ15360TickOfBatchStart);
				fMetronome.Render(newPos, inputParams->fSignatureNumerator, inputParams->fSignatureDenominator, buf, inputParams->fFramesToRender, fCurrentSampleRate);
				if (fIsLooping && (inputParams->fPPQ15360TickOfBatchStart >= fLoopStart)) {
					if (newPos >= fLoopEnd) {
						newPos -= (fLoopEnd - fLoopStart);
						fMetronome.ChaseTo(fLoopStart);
					}
				}
				ReWire::ReWireSetBitInBitField(outputParams->fServedChannelsBitField, 3);
			}
		}
	}
}






static bool IsSampleDeviceInitialized() {
	if (gSampleDevice!=NULL) {
		return true;
	}
	return false;
}

static void SampleDeviceInitialize() {
	assert(gSampleDevice == NULL);
	gSampleDevice = new CReWireSampleDevice();
}

static void SampleDeviceTerminate() {
	delete gSampleDevice;
	gSampleDevice = NULL;
}





/////////////////////		Implementation of the device's ReWire API

/*
	Most of these functions just call thru to the CReWireSampleDevice instance, which does the real work.
*/


void REWIRECALL RWDEFGetDeviceNameAndVersion(ReWire::ReWire_int32_t* codedForReWireVersion, ReWire::ReWire_char_t name[]) {
	assert(codedForReWireVersion != NULL);

	*codedForReWireVersion = REWIRE_DEVICE_DLL_API_VERSION;
	std::strcpy(name, kSampleDeviceName);
}

void REWIRECALL RWDEFGetDeviceInfo(ReWire::ReWireDeviceInfo* info){
	assert(info != NULL);
	RWDEFGetDeviceNameAndVersion(&info->fCodedForReWireVersion, info->fName);
	info->fChannelCount = 4;
	std::strcpy(info->fChannelNames[0], "Saw 1");
	std::strcpy(info->fChannelNames[1], "Saw 2");
	std::strcpy(info->fChannelNames[2], "Saw 3");
	std::strcpy(info->fChannelNames[3], "Metronome");
	ReWire::ReWireSetBitInBitField(info->fSuggestedDefaultOnChannelsBitField, 0);
	ReWire::ReWireSetBitInBitField(info->fSuggestedDefaultOnChannelsBitField, 1);
	ReWire::ReWireSetBitInBitField(info->fStereoPairsBitField, 0);
	info->fMaxEventOutputBufferSize = 5;
}

ReWire::ReWireError REWIRECALL RWDEFOpenDevice(const ReWire::ReWireOpenInfo* openInfo){
	if (!IsSampleDeviceInitialized()) {
		SampleDeviceInitialize();
	}

	assert(gSampleDevice != NULL);
	return gSampleDevice->Open(openInfo);
}

char REWIRECALL RWDEFIsCloseOK(void){
	assert(gSampleDevice != NULL);
	return gSampleDevice->IsCloseOK();
}

void REWIRECALL RWDEFCloseDevice(void){
	assert(gSampleDevice != NULL);
	gSampleDevice->Close();
	SampleDeviceTerminate();
}

void REWIRECALL RWDEFDriveAudio(const ReWire::ReWireDriveAudioInputParams* inputParams,ReWire::ReWireDriveAudioOutputParams* outputParams){
	assert(gSampleDevice != NULL);
	gSampleDevice->DriveAudio(inputParams, outputParams);
}

void REWIRECALL RWDEFIdle(void){
	// The sample device does nothing in its idle time...
}

void REWIRECALL RWDEFSetAudioInfo(const ReWire::ReWireAudioInfo* audioInfo){
	assert(gSampleDevice != NULL);
	gSampleDevice->SetAudioInfo(audioInfo);
}



#if WINDOWS
#if TARGET_DEBUGGING
const char kSamplePanelFileName[]="SamplePanelDebugging.exe";
#endif // TARGET_DEBUGGING
#if TARGET_TESTING
const char kSamplePanelFileName[]="SamplePanelTesting.exe";
#endif // TARGET_TESTING
#if TARGET_DEPLOYMENT
const char kSamplePanelFileName[]="SamplePanelDeployment.exe";
#endif // TARGET_DEPLOYMENT
#endif // WINDOWS

#if MAC
const char kSamplePanelFileName[]="SamplePanel.app";
#endif // MAC

ReWire::ReWireError REWIRECALL RWDEFLaunchPanelApp(){
	assert(gSampleDevice != NULL);
	// The sample device does not implement this.
	return ReWire::kReWireError_NoError;
}

char REWIRECALL RWDEFIsPanelAppLaunched(){
	assert(gSampleDevice != NULL);
	if (gSampleDevice->IsPanelConnected()) {
		return 1;
	}
	else {
		return 0;
	}
}

ReWire::ReWireError REWIRECALL RWDEFQuitPanelApp(){
	assert(gSampleDevice != NULL);
	if (gSampleDevice->QuitPanelApp()) {
		return ReWire::kReWireError_NoError;
	}
	else {
		return ReWire::kReWireError_Undefined;
	}
}

void REWIRECALL RWDEFGetEventInfo(ReWire::ReWireEventInfo* eventInfo){
	ReWire::ReWireSetBitInBitField(eventInfo->fUsedBusBitField, 0);
	ReWire::ReWireSetBitInBitField(eventInfo->fUsedBusBitField, 1);
}

void REWIRECALL RWDEFGetEventBusInfo(ReWire::ReWire_uint16_t busIndex,ReWire::ReWireEventBusInfo* eventBusInfo){
	switch (busIndex) {
		case 0:
			ReWire::ReWireSetBitInBitField(eventBusInfo->fUsedChannelBitField, 0);
			break;
		case 1:
			std::strcpy(eventBusInfo->fBusName,"Main bus");
			ReWire::ReWireSetBitRangeInBitField(eventBusInfo->fUsedChannelBitField, 0, 3);
			break;
	}
}

void REWIRECALL RWDEFGetEventChannelInfo(const ReWire::ReWireEventTarget* eventTarget,ReWire::ReWireEventChannelInfo* eventChannelInfo){
	switch (eventTarget->fMIDIBusIndex) {
		case 0:
			switch (eventTarget->fChannel) {
				case 0:
					std::strcpy(eventChannelInfo->fChannelName,"Metronome");
					break;
			}
			break;
		case 1:
			{
				switch (eventTarget->fChannel) {
					case 0:
						ReWire::ReWireSetBitInBitField(eventChannelInfo->fUsedControllerBitField, 7);

						ReWire::ReWireSetBitInBitField(eventChannelInfo->fUsedControllerBitField, 72);
						ReWire::ReWireSetBitInBitField(eventChannelInfo->fUsedControllerBitField, 74);
						ReWire::ReWireSetBitInBitField(eventChannelInfo->fUsedControllerBitField, 75);
						ReWire::ReWireSetBitRangeInBitField(eventChannelInfo->fUsedNoteBitField, 0, 128);

						std::strcpy(eventChannelInfo->fChannelName,"Saw 1 synth");
						break;
					case 1:
						ReWire::ReWireSetBitInBitField(eventChannelInfo->fUsedControllerBitField, 7);

						ReWire::ReWireSetBitInBitField(eventChannelInfo->fUsedControllerBitField, 72);
						ReWire::ReWireSetBitInBitField(eventChannelInfo->fUsedControllerBitField, 74);
						ReWire::ReWireSetBitInBitField(eventChannelInfo->fUsedControllerBitField, 75);
						ReWire::ReWireSetBitRangeInBitField(eventChannelInfo->fUsedNoteBitField, 0, 128);

						std::strcpy(eventChannelInfo->fChannelName,"Saw 2 synth");
						return;
					case 2:
						ReWire::ReWireSetBitInBitField(eventChannelInfo->fUsedControllerBitField, 7);

						ReWire::ReWireSetBitInBitField(eventChannelInfo->fUsedControllerBitField, 72);
						ReWire::ReWireSetBitInBitField(eventChannelInfo->fUsedControllerBitField, 74);
						ReWire::ReWireSetBitInBitField(eventChannelInfo->fUsedControllerBitField, 75);
						ReWire::ReWireSetBitRangeInBitField(eventChannelInfo->fUsedNoteBitField, 0, 128);

						std::strcpy(eventChannelInfo->fChannelName,"Saw 3 synth");
						return;
					break;
				}
			}
			break;
	}
}

void REWIRECALL RWDEFGetEventControllerInfo(const ReWire::ReWireEventTarget* eventTarget,ReWire::ReWire_uint16_t controllerIndex,ReWire::ReWireEventControllerInfo* controllerInfo){
	if (eventTarget->fMIDIBusIndex == 1) {
		switch (eventTarget->fChannel) {
			case 0:
			case 1:
			case 2:
				switch (controllerIndex) {
					case 7:
						controllerInfo->fType = ReWire::kReWireEventControllerTypeContinous;
						controllerInfo->fMinValue = 0;
						controllerInfo->fMaxValue = 127;
						std::strcpy(controllerInfo->fControllerName, "Main Volume");
						break;
					case 72:
						controllerInfo->fType = ReWire::kReWireEventControllerTypeBipolar;
						controllerInfo->fMinValue = 0;
						controllerInfo->fMaxValue = 127;
						std::strcpy(controllerInfo->fControllerName, "Pan");
						break;
					case 74:
						controllerInfo->fType = ReWire::kReWireEventControllerTypeContinous;
						controllerInfo->fMinValue = 0;
						controllerInfo->fMaxValue = 127;
						std::strcpy(controllerInfo->fControllerName, "Filter freq");
						break;
					case 75:
						controllerInfo->fType = ReWire::kReWireEventControllerTypeStep;
						controllerInfo->fMinValue = 0;
						controllerInfo->fMaxValue = 4;
						std::strcpy(controllerInfo->fControllerName, "Filter type");
						break;
				}
				break;
			case 3:
				break;
		}
	}
}

void REWIRECALL RWDEFGetEventNoteInfo(const ReWire::ReWireEventTarget* eventTarget,ReWire::ReWire_uint16_t noteIndex,ReWire::ReWireEventNoteInfo* noteInfo){
	if (eventTarget->fMIDIBusIndex == 1) {
		switch (eventTarget->fChannel) {
			case 0:
			case 1:
			case 2:
				if (noteIndex < 128) {
					noteInfo->fType = ReWire::kReWireEventNoteTypeNote;
				}
				break;
		}
	}
}
