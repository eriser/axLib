#include "SimpleAudioAPI.h"
#include <stdexcept>

#if WINDOWS
#include <mmsystem.h>
#endif // WINDOWS


namespace NSSimpleAudioAPI {




namespace NSInternals {

static volatile unsigned int gIsAudioRunning = 0;

class CStreamingInstance
{
	public: virtual const TAudioDriverID& GetAudioDriverID() const = 0;
		
	public: virtual TSampleRate GetSampleRate() const = 0;
	public: virtual TChannelCount GetChannelCount() const = 0;
	public: virtual TFrameCount GetBufferFrames() const = 0;
};

#if WINDOWS

const char kDirectSoundDLLName[] = "dsound.dll";
const char kDirectSoundCreateProcName[] = "DirectSoundCreate";
const char kDirectSoundEnumerateWProcName[] = "DirectSoundEnumerateW";

typedef INT16 TInt16Sample;
typedef UINT32 TByteCount;
typedef INT32 TSampleCount;

class CDXAPI {
	public: CDXAPI();
	public: ~CDXAPI();

	public: HRESULT DirectSoundCreate(const GUID& iGUID, LPDIRECTSOUND * ppDS, IUnknown FAR *pUnkOuter);
	public: HRESULT DirectSoundEnumerateW(LPDSENUMCALLBACKW lpCallback, LPVOID lpContext);

	private: void Dispose();

	private: typedef HRESULT (WINAPI* DIRECT_SOUND_CREATE)(const GUID * lpGUID, LPDIRECTSOUND * ppDS, IUnknown FAR *pUnkOuter);
	private: typedef HRESULT (WINAPI* DIRECT_SOUND_ENUMERATE_W)(LPDSENUMCALLBACKW lpCallback, LPVOID lpContext);

	private: HINSTANCE fDirectSoundInstance;
	private: DIRECT_SOUND_CREATE fDirectSoundCreateFuncPtr;
	private: DIRECT_SOUND_ENUMERATE_W fDirectSoundEnumerateWFuncPtr;
};

CDXAPI::CDXAPI()
	: fDirectSoundInstance(NULL)
	, fDirectSoundCreateFuncPtr(NULL)
	, fDirectSoundEnumerateWFuncPtr(NULL)
{
	try{
		fDirectSoundInstance = ::LoadLibraryA(kDirectSoundDLLName);
		if(fDirectSoundInstance == NULL){
			throw std::runtime_error("LoadLibrary failed to load dsound dll");
		}
		fDirectSoundCreateFuncPtr = (DIRECT_SOUND_CREATE)GetProcAddress(fDirectSoundInstance, kDirectSoundCreateProcName);
		if(fDirectSoundCreateFuncPtr == NULL){
			throw std::runtime_error("GetProcAddress failed to locate DirectSoundCreate function in dsound dll");
		}
		fDirectSoundEnumerateWFuncPtr = (DIRECT_SOUND_ENUMERATE_W)GetProcAddress(fDirectSoundInstance, kDirectSoundEnumerateWProcName);
		if(fDirectSoundEnumerateWFuncPtr == NULL){
			throw std::runtime_error("GetProcAddress failed to locate DirectSoundEnumerateW function in dsound dll");
		}
	}
	catch(...){
		Dispose();
		throw;
	}
}

CDXAPI::~CDXAPI(){
	Dispose();
}

void CDXAPI::Dispose(){
	fDirectSoundEnumerateWFuncPtr = NULL;
	fDirectSoundCreateFuncPtr = NULL;
	if(fDirectSoundInstance != NULL){
		FreeLibrary(fDirectSoundInstance);
		fDirectSoundInstance = NULL;
	}
}

HRESULT CDXAPI::DirectSoundCreate(const GUID& iGUID, LPDIRECTSOUND * ppDS, IUnknown FAR *pUnkOuter){
	const GUID* audioDriverGUID = NULL;
	TAudioDriverID defaultID = GetDefaultAudioDriverID();
	if(std::memcmp(&iGUID, &defaultID, sizeof(GUID)) != 0){
		audioDriverGUID = &iGUID;
	}

	if(fDirectSoundCreateFuncPtr != 0){
		return (*fDirectSoundCreateFuncPtr)(audioDriverGUID, ppDS, pUnkOuter);
	}
	else{
		return DSERR_NODRIVER;
	}
}

HRESULT CDXAPI::DirectSoundEnumerateW(LPDSENUMCALLBACKW lpCallback, LPVOID lpContext){
	if(fDirectSoundEnumerateWFuncPtr != 0){
		return (*fDirectSoundEnumerateWFuncPtr)(lpCallback, lpContext);
	}
	else{
		return DSERR_NODRIVER;
	}
}

const TChannelCount kDirectSoundOutSoundChannelCount = 2;
const TFrameCount kDirectSoundBufferFrameCount = 16384;


class CDXStreamingInstance : public CStreamingInstance
{
	public: CDXStreamingInstance(CSimpleAudioInstance& iInstance, const TAudioDriverID& iAudioDriverID, HWND iHWnd, ISimpleAudioRenderCallback& iSimpleAudioRenderCallback);
	public: ~CDXStreamingInstance();

	public: const TAudioDriverID& GetAudioDriverID() const;

	public: TSampleRate GetSampleRate() const;
	public: TChannelCount GetChannelCount() const;
	public: TFrameCount GetBufferFrames() const;

	private: void Create();
	private: void Dispose();

	private: ISimpleAudioRenderCallback& fSimpleAudioRenderCallback;

			/** @brief Calls ISimpleAudioRenderCallback and copies frames to DX buffer */
	private: void Render(TFrameCount iStartPos, TFrameCount iFrameCount);

			 /** @brief Checks if we need to render frames and calls Render as necessary, returns true if Render was called */
	private: bool Stream();

	private: TAudioDriverID fAudioDriverID;
	private: TSampleRate fSampleRate;
	private: TChannelCount fChannelCount;
	private: TFrameCount fBufferFrames;

	private: CSimpleAudioInstance& fInstance;

			 /** @brief Window handle required to set SetCooperativeLevel in DirectX */
	private: HWND fHWnd;

			 /** @brief Internal class that manage direct access to the dsound.dll */
	private: CDXAPI* fDXAPI;

			 /** @brief The polling rendering thread */
	private: HANDLE fThread;

			 /** @brief The mutex used to tell fThread to exit */
	private: HANDLE fExitMutex;

			 /** @brief The last detected frame position inside the direct sound buffer */
	private: TFrameCount fPrevPlayFrameIndex;

			 /** @brief The number of frames written to direct sound buffer since start */
	private: TFrameCount fTotalWrittenFrames;

			 /** @brief The number of played frames played since start */
	private: TFrameCount fTotalPlayFrames;

			 /** @brief The DirectSound instance */
	private: IDirectSound* fDirectSound;

			/** @brief The primary buffer for direct sound - used to set playback format */
	private: IDirectSoundBuffer* fPrimaryDirectSoundBuffer;

			/** @brief The secondary buffer for direct sound - used to poll playback position and frame writing */
	private: IDirectSoundBuffer* fSecondaryDirectSoundBuffer;

			/** @brief Raw float output buffers used for ISimpleAudioRenderCallback_Render callback */
	private: TFloatSampleBuffers fOutputBuffers;

			/** @brief Temporary buffer used for converting to 16 bit and interleaving from fOutputBuffers */
	private: TInt16Sample fTmpFrameBuffer[kDirectSoundBufferFrameCount*kDirectSoundOutSoundChannelCount];

	private: static DWORD WINAPI ThreadCallback(LPVOID param);
};


const TFrameCount kRenderBatchFrameCount = 64;
const TByteCount kFrameByteSize = sizeof(TInt16Sample) * kDirectSoundOutSoundChannelCount;
const DWORD kStreamThreadSleepMS = 1;
const TSampleRate kDefaultSampleRate = 44100;
const TFrameCount kDefaultBufferSize = 4096;

class CDXLockedRange {
	public:
		TByteCount fFrameByteSize;
		void* fPtr1;
		void* fPtr2;
		DWORD fLen1;
		DWORD fLen2;


		CDXLockedRange(TByteCount frameByteSize) :
			fFrameByteSize(frameByteSize),
				fPtr1(0),
				fPtr2(0),
				fLen1(0),
				fLen2(0)
			{
			}
};

class CDXLockedBuffer {
	public: CDXLockedBuffer(IDirectSoundBuffer& iDirectSoundBuffer, TFrameCount iStartPos, TFrameCount iFrameCount);
	public: ~CDXLockedBuffer();

	public: void Clear();
	public: void Write(const TInt16Sample iInterleavedFrames[]);

	private: IDirectSoundBuffer& fDirectSoundBuffer;
	private: CDXLockedRange fLockedRange;
};

CDXLockedBuffer::CDXLockedBuffer(IDirectSoundBuffer& iDirectSoundBuffer, TFrameCount iStartPos, TFrameCount iFrameCount)
	: fDirectSoundBuffer(iDirectSoundBuffer)
	, fLockedRange(kFrameByteSize)
{
	HRESULT result;

	result = fDirectSoundBuffer.Lock(iStartPos * kFrameByteSize,
		iFrameCount * kFrameByteSize,
		&fLockedRange.fPtr1,
		&fLockedRange.fLen1,
		&fLockedRange.fPtr2,
		&fLockedRange.fLen2,
		0);

	//	Restore buffer if lost and try again (one try only).
	if(result == DSERR_BUFFERLOST){
		result = fDirectSoundBuffer.Restore();
		if(result == DS_OK){
			result = fDirectSoundBuffer.Lock(iStartPos * kFrameByteSize,
				iFrameCount * kFrameByteSize,
				&fLockedRange.fPtr1,
				&fLockedRange.fLen1,
				&fLockedRange.fPtr2,
				&fLockedRange.fLen2,
				0);
		}
	}
}
CDXLockedBuffer::~CDXLockedBuffer(){
	//	Unlock the buffer.
	if(fLockedRange.fPtr1 != NULL || fLockedRange.fPtr2 != NULL){
		fDirectSoundBuffer.Unlock(fLockedRange.fPtr1,fLockedRange.fLen1,fLockedRange.fPtr2,fLockedRange.fLen2);
		fLockedRange = CDXLockedRange(kFrameByteSize);
	}
}

void CDXLockedBuffer::Clear(){
	if(fLockedRange.fPtr1 != NULL){
		TFrameCount frameCount = fLockedRange.fLen1 / kFrameByteSize;
		TSampleCount sampleCount = frameCount * kDirectSoundOutSoundChannelCount;
		TInt16Sample* samplePtr = reinterpret_cast<TInt16Sample*>(fLockedRange.fPtr1);
		std::fill(samplePtr, &samplePtr[frameCount], 0);
	}
	if(fLockedRange.fPtr2 != NULL){
		TFrameCount frameCount = fLockedRange.fLen2 / kFrameByteSize;
		TSampleCount sampleCount = frameCount * kDirectSoundOutSoundChannelCount;
		TInt16Sample* samplePtr = reinterpret_cast<TInt16Sample*>(fLockedRange.fPtr2);
		std::fill(samplePtr, &samplePtr[frameCount], 0);
	}

}
void CDXLockedBuffer::Write(const TInt16Sample iInterleavedFrames[]){
	const TInt16Sample* sourceSamples = iInterleavedFrames;
	if(fLockedRange.fPtr1 != NULL){
		TFrameCount frameCount = fLockedRange.fLen1 / kFrameByteSize;
		TSampleCount sampleCount = frameCount * kDirectSoundOutSoundChannelCount;
		TInt16Sample* samplePtr = reinterpret_cast<TInt16Sample*>(fLockedRange.fPtr1);
		memcpy(samplePtr, sourceSamples, sizeof(TInt16Sample) * sampleCount);
		sourceSamples += sampleCount;
	}
	if(fLockedRange.fPtr2 != NULL){
		TFrameCount frameCount = fLockedRange.fLen2 / kFrameByteSize;
		TSampleCount sampleCount = frameCount * kDirectSoundOutSoundChannelCount;
		TInt16Sample* samplePtr = reinterpret_cast<TInt16Sample*>(fLockedRange.fPtr2);
		memcpy(samplePtr, sourceSamples, sizeof(TInt16Sample) * sampleCount);
	}
}

TFrameCount GetCurrentHWFramePlayPos(IDirectSoundBuffer& iDirectSoundBuffer){
	DWORD playBytePosition = 0;
	DWORD dummyWriteBytePosition = 0;
	TFrameCount playFrameIndex = 0;

	//////	WARNING! writeBytePosition doesn't seem to work!!! AWE32 970112 DirectSound3 secondary buffer.
	HRESULT result=iDirectSoundBuffer.GetCurrentPosition(&playBytePosition,&dummyWriteBytePosition);
	if(result != DS_OK){
		return 0;
	}

	playFrameIndex = playBytePosition / kFrameByteSize;

	/////////	AWE32 returns playBytePosition==8192 for a buffer that is 8192 DirectSound2, primary buffer!?!?
	if(playFrameIndex == kDirectSoundBufferFrameCount){
		playFrameIndex=0;
	}

	return playFrameIndex;
}



void ScaleAndInterleaveTo16BitStereo(
	const TFloatSample* inputBufferL,
	const TFloatSample* inputBufferR,
	TInt16Sample* outputBuffer,
	TFrameCount frames)
{
	static const float kConvertTo16BitFactor=0x7fff;

	TInt16Sample* outputData=outputBuffer;
	const TFloatSample* leftInput = inputBufferL;
	const TFloatSample* rightInput = inputBufferR;

	TFrameCount count = frames;

	while(count>0){
		count--;
		INT32 left=static_cast<INT32>((*leftInput) * kConvertTo16BitFactor);
		leftInput++;
		INT32 right=static_cast<INT32>((*rightInput) * kConvertTo16BitFactor);
		rightInput++;
		*outputData=static_cast<TInt16Sample>(left);

		outputData++;

		*outputData=static_cast<TInt16Sample>(right);

		outputData++;
	}
}






CDXStreamingInstance::CDXStreamingInstance(
		CSimpleAudioInstance& iInstance,
		const TAudioDriverID& iAudioDriverID,
		HWND iHWnd,
		ISimpleAudioRenderCallback& iSimpleAudioRenderCallback)
	: fSimpleAudioRenderCallback(iSimpleAudioRenderCallback)
	, fAudioDriverID(iAudioDriverID)
	, fSampleRate(0)
	, fChannelCount(0)
	, fBufferFrames(0)
	, fInstance(iInstance)
	, fHWnd(iHWnd)
	, fDXAPI(NULL)
	, fThread(INVALID_HANDLE_VALUE)
	, fExitMutex(INVALID_HANDLE_VALUE)
	, fPrevPlayFrameIndex(0)
	, fTotalWrittenFrames(0)
	, fTotalPlayFrames(0)
	, fDirectSound(NULL)
	, fPrimaryDirectSoundBuffer(NULL)
	, fSecondaryDirectSoundBuffer(NULL)
	, fOutputBuffers()
{
	try{
		Create();
	}
	catch(...){
		Dispose();
		throw;
	}
}

CDXStreamingInstance::~CDXStreamingInstance(){
	Dispose();
}

const TAudioDriverID& CDXStreamingInstance::GetAudioDriverID() const{
	return fAudioDriverID;
}

TSampleRate CDXStreamingInstance::GetSampleRate() const{
	return fSampleRate;
}

TChannelCount CDXStreamingInstance::GetChannelCount() const{
	return fChannelCount;
}

TFrameCount CDXStreamingInstance::GetBufferFrames() const{
	return fBufferFrames;
}

void CDXStreamingInstance::Create(){
	TFloatSample* buffer = 0;
	try{
		fDXAPI = new CDXAPI();

		HRESULT result = fDXAPI->DirectSoundCreate(fAudioDriverID, &fDirectSound, NULL);
		if(result !=DS_OK || fDirectSound == NULL){
			throw std::runtime_error("fDXAPI->DirectSoundCreate failed");
		}

		HWND hWnd = fHWnd;
		if(hWnd == NULL){
			hWnd = GetDesktopWindow();
		}

		result = fDirectSound->SetCooperativeLevel(hWnd,DSSCL_PRIORITY);
		if(result !=DS_OK){
			throw std::runtime_error("fDXAPI->SetCooperativeLevel failed");
		}

		WAVEFORMATEX waveFormatEx;
		waveFormatEx.wFormatTag = WAVE_FORMAT_PCM;
		waveFormatEx.nChannels = kDirectSoundOutSoundChannelCount;
		waveFormatEx.nSamplesPerSec = kDefaultSampleRate;
		waveFormatEx.nAvgBytesPerSec = kDefaultSampleRate * kFrameByteSize;
		waveFormatEx.nBlockAlign = kFrameByteSize;
		waveFormatEx.wBitsPerSample = 16;
		waveFormatEx.cbSize=0;
		{
			DSBUFFERDESC bufferDesc;

			memset(&bufferDesc, 0,sizeof(DSBUFFERDESC));
			bufferDesc.dwSize = sizeof(DSBUFFERDESC);
			bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_GETCURRENTPOSITION2;

			//	Must be 0 for primary buffers. We are setting output format explicitly below.
			bufferDesc.dwBufferBytes = 0;
			bufferDesc.dwReserved = 0;
			bufferDesc.lpwfxFormat = 0;

			result = fDirectSound->CreateSoundBuffer(&bufferDesc, &fPrimaryDirectSoundBuffer, 0);
			if(result !=DS_OK || fPrimaryDirectSoundBuffer == NULL){
				throw std::runtime_error("fDirectSound->CreateSoundBuffer (primary buffer) failed");
			}

			DSBCAPS caps;
			fPrimaryDirectSoundBuffer->GetCaps(&caps);

			result = fPrimaryDirectSoundBuffer->SetFormat(&waveFormatEx);
			if(result !=DS_OK){
				throw std::runtime_error("fPrimaryDirectSoundBuffer->SetFormat failed");
			}

			result = fPrimaryDirectSoundBuffer->GetFormat(&waveFormatEx,sizeof(WAVEFORMATEX),0);
			if(result !=DS_OK){
				throw std::runtime_error("fPrimaryDirectSoundBuffer->GetFormat failed");
			}
		}

		{
			DSBUFFERDESC bufferDesc;

			memset(&bufferDesc, 0, sizeof(DSBUFFERDESC));
			bufferDesc.dwSize = sizeof(DSBUFFERDESC);
			bufferDesc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_STATIC | DSBCAPS_GETCURRENTPOSITION2;
			bufferDesc.dwBufferBytes = kDirectSoundBufferFrameCount * kFrameByteSize;
			bufferDesc.dwReserved = 0;
			bufferDesc.lpwfxFormat = &waveFormatEx;

			result = fDirectSound->CreateSoundBuffer(&bufferDesc, &fSecondaryDirectSoundBuffer, 0);

			//	If DirectSound complains about bad params - try without GLOBAL_FOCUS. DirectX3 is required for that.
			if(result == E_INVALIDARG){
				bufferDesc.dwFlags &= ~DSBCAPS_GLOBALFOCUS;
				result=fDirectSound->CreateSoundBuffer(&bufferDesc, &fSecondaryDirectSoundBuffer, 0);
			}

			if(result !=DS_OK || fSecondaryDirectSoundBuffer == NULL){
				throw std::runtime_error("fPrimaryDirectSoundBuffer->GetFormat failed");
			}
		}

		for(TChannelCount channel = 0; channel < kDirectSoundOutSoundChannelCount; ++channel){
			buffer = new TFloatSample[kDirectSoundBufferFrameCount];
			std::fill(buffer, &buffer[kDirectSoundBufferFrameCount], 0.f);
			fOutputBuffers.push_back(buffer);
			buffer=NULL;
		}

		{
			CDXLockedBuffer lock(*fSecondaryDirectSoundBuffer, 0, kDirectSoundBufferFrameCount);
			lock.Clear();
		}

		fExitMutex = CreateMutex(0,TRUE,0);
		if(NULL == fExitMutex){
			throw std::runtime_error("CreateMutex failed");
		}

		{
			DWORD threadID=0;
			fThread = CreateThread(0,0,ThreadCallback,(void*)this,CREATE_SUSPENDED,&threadID);
			if(fThread == NULL){
				throw std::runtime_error("CreateThread failed");
			}
			SetThreadPriority(fThread,THREAD_PRIORITY_TIME_CRITICAL);
		}

		fSampleRate = waveFormatEx.nSamplesPerSec;
		fChannelCount = static_cast<TChannelCount>(fOutputBuffers.size());
		fBufferFrames = kDefaultBufferSize;

		Render(0, fBufferFrames);

		ResumeThread(fThread);

		result = fSecondaryDirectSoundBuffer->Play(0,0,DSBPLAY_LOOPING);
		if(result !=DS_OK){
			throw std::runtime_error("fSecondaryDirectSoundBuffer->Play failed");
		}

	}
	catch(...){
		delete [] buffer;
		buffer = NULL;
		throw;
	}
}

void CDXStreamingInstance::Dispose(){
	if(INVALID_HANDLE_VALUE != fExitMutex){
		ReleaseMutex(fExitMutex);
		if(fThread != INVALID_HANDLE_VALUE){
			WaitForSingleObject(fThread, 10000);
			CloseHandle(fThread);
			fThread = INVALID_HANDLE_VALUE;
		}
		CloseHandle(fExitMutex);
		fExitMutex = INVALID_HANDLE_VALUE;
	}

	if(NULL != fSecondaryDirectSoundBuffer){
		fSecondaryDirectSoundBuffer->Stop();
		fSecondaryDirectSoundBuffer->Release();
		fSecondaryDirectSoundBuffer = NULL;
	}
	if(NULL != fPrimaryDirectSoundBuffer){
		fPrimaryDirectSoundBuffer->Release();
		fPrimaryDirectSoundBuffer = NULL;
	}

	while(fOutputBuffers.size() > 0){
		TFloatSample* buffer = fOutputBuffers.back();
		delete [] buffer;
		buffer = NULL;
		fOutputBuffers.pop_back();
	}

	delete fDXAPI;
	fDXAPI = NULL;
}

void CDXStreamingInstance::Render(TFrameCount iStartPos, TFrameCount iFrameCount){
	try{
		fSimpleAudioRenderCallback.ISimpleAudioRenderCallback_Render(fInstance, iFrameCount, fOutputBuffers);

		ScaleAndInterleaveTo16BitStereo(fOutputBuffers[0], fOutputBuffers[1], fTmpFrameBuffer, iFrameCount);

		{
			CDXLockedBuffer bufferLock(*fSecondaryDirectSoundBuffer, iStartPos, iFrameCount);
			bufferLock.Write(fTmpFrameBuffer);
		}

		fTotalWrittenFrames += iFrameCount;
	}
	catch(...){

	}
}

bool CDXStreamingInstance::Stream(){
	if(fSecondaryDirectSoundBuffer != NULL){
		TFrameCount playFrameIndex = GetCurrentHWFramePlayPos(*fSecondaryDirectSoundBuffer);//*fPrimaryDirectSoundBuffer);
		TFrameCount framesSinceLast = playFrameIndex - fPrevPlayFrameIndex;
		//	Did play pos wrap in buffer?
		if(framesSinceLast < 0){
			framesSinceLast += kDirectSoundBufferFrameCount;
		}

		fTotalPlayFrames += framesSinceLast;
		fPrevPlayFrameIndex = playFrameIndex;

		TFrameCount wantedNumberOfFramesWaitingToPlay = fBufferFrames;
		TFrameCount numberOfFramesWaitingToPlay = fTotalWrittenFrames - fTotalPlayFrames;

		if(numberOfFramesWaitingToPlay < wantedNumberOfFramesWaitingToPlay){
			//	Limit how much audio we generate ahead of the sound driver's play position.

			TFrameCount numberOfFramesToWrite = wantedNumberOfFramesWaitingToPlay - numberOfFramesWaitingToPlay;

			// Make the count a multiple of kRenderBatchFrameCount and only render if we need a full block
			TFrameCount batchCountToWrite = numberOfFramesToWrite / kRenderBatchFrameCount;
			if(batchCountToWrite > 0){
				numberOfFramesToWrite = batchCountToWrite * kRenderBatchFrameCount;

				TFrameCount startPos = fTotalWrittenFrames & (kDirectSoundBufferFrameCount-1);

				Render(startPos, numberOfFramesToWrite);

				return true;
			}
		}
	}
	return false;
}

DWORD WINAPI CDXStreamingInstance::ThreadCallback(LPVOID param){
	try{
		CDXStreamingInstance* driver=(CDXStreamingInstance*)param;

		bool didStream=false;
		do{
			try{
				didStream=driver->Stream();
			}
			catch(...){
				//	No exceptions can leave thru the timer callback. Stop all here.
			}
		}while(WAIT_TIMEOUT==WaitForSingleObject(driver->fExitMutex,kStreamThreadSleepMS));

		ReleaseMutex(driver->fExitMutex);

		return 0;
	}
	catch(const std::exception& /*e*/){
	}
	catch(...){
	}
	return -1;
}

bool GetAudioDriverCaps(CDXAPI& iDCAPI, const TAudioDriverID& iAudioDriverID, TAudioDriverCaps& oAudioDriverCaps){
	std::memset(&oAudioDriverCaps.fCaps, 0, sizeof(TAudioDriverCaps));
	oAudioDriverCaps.fCaps.dwSize = sizeof(DSCAPS);

	IDirectSound* directSound=0;
	HRESULT result = iDCAPI.DirectSoundCreate(iAudioDriverID, &directSound, NULL);
	if((result == DS_OK) && (directSound != NULL)){
		result = directSound->GetCaps(&oAudioDriverCaps.fCaps);

		directSound->Release();
		directSound = NULL;

		if(result == DS_OK){
			return true;
		}
	}
	return false;
}

class VEnumCallbackItem {
	public: TAudioDriverID fAudioDriverID;
	public: TCHAR lpszDesc[511 + 1];
	public: TCHAR lpszDrvName[511 + 1];
};

BOOL CALLBACK DSEnumCallbackW(GUID* lpGUID, LPCWSTR lpszDesc, LPCWSTR lpszDrvName, LPVOID lpContext){
	std::vector<VEnumCallbackItem>* driverItems = reinterpret_cast<std::vector<VEnumCallbackItem>*>(lpContext);
	VEnumCallbackItem item;
	if(lpGUID != NULL){
		item.fAudioDriverID = *lpGUID;
	}
	else{
		item.fAudioDriverID = GetDefaultAudioDriverID();
	}
	wcsncpy_s(item.lpszDesc, 511 + 1, lpszDesc, 511);
	item.lpszDesc[511] = 0;

	wcsncpy_s(item.lpszDrvName, 511 + 1, lpszDrvName, 511);
	item.lpszDesc[511] = 0;

	driverItems->push_back(item);
	return TRUE;
}

	
TAudioDriverID kDefaultAudioDriverID = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0} };
TAudioDriverID GetDefaultAudioDriverID()
{
	return kDefaultAudioDriverID;
}
	
TAudioDriverIDs GetAudioDriverIDs(){
	TAudioDriverIDs audioDriverIDs;

	NSInternals::CDXAPI dxAPI;
	std::vector<VEnumCallbackItem> driverItems;
	HRESULT result = dxAPI.DirectSoundEnumerateW(DSEnumCallbackW, &driverItems);
	if(result == DS_OK){
		audioDriverIDs.reserve(driverItems.size());
		for(std::vector<VEnumCallbackItem>::const_iterator it = driverItems.begin(); it != driverItems.end(); ++it){
			const VEnumCallbackItem& driverItem(*it);
			TAudioDriverCaps caps;
			if(NSInternals::GetAudioDriverCaps(dxAPI, driverItem.fAudioDriverID, caps)){
				audioDriverIDs.push_back(driverItem.fAudioDriverID);
			}
		}
	}
	return audioDriverIDs;
}

bool GetAudioDriverCaps(const TAudioDriverID& iAudioDriverID, TAudioDriverCaps& oAudioDriverCaps){
	NSInternals::CDXAPI dxAPI;
	std::vector<VEnumCallbackItem> driverItems;
	HRESULT result = dxAPI.DirectSoundEnumerateW(DSEnumCallbackW, &driverItems);
	if(result == DS_OK){
		for(std::vector<VEnumCallbackItem>::const_iterator it = driverItems.begin(); it != driverItems.end(); ++it){
			const VEnumCallbackItem& driverItem(*it);
			if(std::memcmp(&driverItem.fAudioDriverID, &iAudioDriverID, sizeof(TAudioDriverID)) == 0){
				wcsncpy_s(oAudioDriverCaps.lpszDesc, 511 + 1, driverItem.lpszDesc, 511);
				oAudioDriverCaps.lpszDesc[511] = 0;

				wcsncpy_s(oAudioDriverCaps.lpszDrvName, 511 + 1, driverItem.lpszDrvName, 511);
				oAudioDriverCaps.lpszDrvName[511] = 0;

				if(NSInternals::GetAudioDriverCaps(dxAPI, driverItem.fAudioDriverID, oAudioDriverCaps)){
					return true;
				}
			}
		}
	}
	return false;
}


#endif // WINDOWS

#if MAC
	
	class CCoreAudioDriver : public CStreamingInstance
	{
		public: CCoreAudioDriver(AudioDeviceID iAudioDriverID,
								 ISimpleAudioRenderCallback* iSimpleAudioRenderCallback,
								 CSimpleAudioInstance& iSimpleAudioInstance);
		public: ~CCoreAudioDriver();
		
		public: const TAudioDriverID& GetAudioDriverID() const;
			
		public: TSampleRate GetSampleRate() const;
		public: TChannelCount GetChannelCount() const;
		public: TFrameCount GetBufferFrames() const;
		
		private: void Init();
		private: void Dispose();
		
		private: static OSStatus Render(AudioDeviceID           inDevice,
										const AudioTimeStamp*   inNow,
										const AudioBufferList*  inInputData,
										const AudioTimeStamp*   inInputTime,
										AudioBufferList*        outOutputData,
										const AudioTimeStamp*   inOutputTime,
										void*                   inClientData);
		
		// data
		private: AudioDeviceID fAudioDeviceID;
		private: AudioDeviceIOProcID fAudioProcID;
		private: ISimpleAudioRenderCallback* fSimpleAudioRenderCallback;
		private: CSimpleAudioInstance& fSimpleAudioInstance;
	};
	
	CCoreAudioDriver::CCoreAudioDriver(AudioDeviceID iAudioDriverID, ISimpleAudioRenderCallback* iSimpleAudioRenderCallback, CSimpleAudioInstance& iSimpleAudioInstance)
		: fAudioDeviceID(iAudioDriverID)
		, fAudioProcID(NULL)
		, fSimpleAudioRenderCallback(iSimpleAudioRenderCallback)
		, fSimpleAudioInstance(iSimpleAudioInstance)
	{
		Init();
	}
	
	CCoreAudioDriver::~CCoreAudioDriver()
	{
		Dispose();
	}
	
	void CCoreAudioDriver::Init()
	{
		OSStatus status = AudioDeviceCreateIOProcID(fAudioDeviceID, &CCoreAudioDriver::Render, this, &fAudioProcID);
		if (status != noErr) {
			throw std::runtime_error("AudioDeviceCreateIOProcID() failed");
		}
		
		status = ::AudioDeviceStart(fAudioDeviceID, fAudioProcID);
		if (status != noErr) {
			throw std::runtime_error("AudioDeviceStart() failed");
		}
		
		gIsAudioRunning = 1;
	}
	
	void CCoreAudioDriver::Dispose()
	{
		gIsAudioRunning = 0;
		
		assert(fAudioDeviceID != 0);
		assert(fAudioProcID != NULL);
		
		OSStatus status = AudioDeviceStop(fAudioDeviceID, fAudioProcID);
		if (status != noErr) {
			throw std::runtime_error("AudioDeviceStop() failed");
		}
		
		status = AudioDeviceDestroyIOProcID(fAudioDeviceID, fAudioProcID);
		if (status != noErr) {
			throw std::runtime_error("AudioDeviceDestroyIOProcID() failed");
		}
		fAudioProcID = NULL;
	}

	OSStatus CCoreAudioDriver::Render(AudioDeviceID           /*inDevice*/,
										const AudioTimeStamp*   /*inNow*/,
										const AudioBufferList*  /*inInputData*/,
										const AudioTimeStamp*   /*inInputTime*/,
										AudioBufferList*        outOutputData,
										const AudioTimeStamp*   /*inOutputTime*/,
										void*                   inClientData)
	{
		if (gIsAudioRunning == 1) {
			assert(outOutputData != NULL);
			assert(outOutputData->mNumberBuffers == 1);
			
			
			CCoreAudioDriver* driver = reinterpret_cast<CCoreAudioDriver*>(inClientData);
			assert(driver != NULL);
			assert(driver->fAudioProcID != NULL);

			AudioBuffer audioBuffer = outOutputData->mBuffers[0];
			assert(audioBuffer.mNumberChannels >= 2);
			assert(audioBuffer.mDataByteSize % sizeof(float) == 0);
			assert(audioBuffer.mDataByteSize == 512 * audioBuffer.mNumberChannels * sizeof(float));

			static float outputBufferLeft[512];
			static float outputBufferRight[512];
			TFloatSampleBuffers outputBuffers;
			outputBuffers.push_back(outputBufferLeft);
			outputBuffers.push_back(outputBufferRight);
			
			driver->fSimpleAudioRenderCallback->ISimpleAudioRenderCallback_Render(driver->fSimpleAudioInstance, 512, outputBuffers);

			float* floatBuffer = reinterpret_cast<float*> (audioBuffer.mData);
			for (int i = 0; i < 1024; i++) {
				floatBuffer[i] = outputBuffers[i%2][i/2];
			}
		}
		
		return noErr;
	}
	
	
	const TAudioDriverID& CCoreAudioDriver::GetAudioDriverID() const
	{
		return fAudioDeviceID;
	}
	
	TSampleRate CCoreAudioDriver::GetSampleRate() const
	{
		Float64 sampleRate = 0;
		UInt32 sampleRateSize = sizeof(sampleRate);
		
		OSStatus status = AudioDeviceGetProperty(
												 fAudioDeviceID, 
												 0, /* master channel */
												 true, /* output */
												 kAudioDevicePropertyNominalSampleRate, 
												 &sampleRateSize, 
												 &sampleRate);
		
		assert(status == noErr);
		return static_cast<TSampleRate>(sampleRate);
	}
	
	
	TChannelCount CCoreAudioDriver::GetChannelCount() const
	{
		AudioBufferList bufferList;
		UInt32 bufferListSize = sizeof(bufferList);
		
		OSStatus status = AudioDeviceGetProperty(
												 fAudioDeviceID, 
												 0, /* master channel */
												 true, /* output */
												 kAudioDevicePropertyStreamConfiguration, 
												 &bufferListSize, 
												 &bufferList);
		
		assert(status == noErr);
		assert(bufferListSize == sizeof(bufferList));
		
		return bufferList.mBuffers[0].mNumberChannels;
		
		//	return 2;
	}
	
	
	TFrameCount CCoreAudioDriver::GetBufferFrames() const
	{
		return 512;
	}

	TAudioDriverID GetDefaultAudioDriverID();
	TAudioDriverIDs GetAudioDriverIDs();
	bool GetAudioDriverCaps(const TAudioDriverID& iAudioDriverID, TAudioDriverCaps& oAudioDriverCaps);

	TAudioDriverID GetDefaultAudioDriverID()
	{
		AudioDeviceID defaultDeviceID = 0;
		UInt32 size = sizeof(defaultDeviceID);
		
		OSStatus status = ::AudioHardwareGetProperty(kAudioHardwarePropertyDefaultOutputDevice,
													 &size,
													 &defaultDeviceID);
		assert(status == noErr);
		return defaultDeviceID;		
	}

	TAudioDriverIDs GetAudioDriverIDs(){
		std::vector<TAudioDriverID> driverList;
		driverList.push_back(GetDefaultAudioDriverID());
		return driverList;
	}
	
	bool GetAudioDriverCaps(const TAudioDriverID& /*iAudioDriverID*/, TAudioDriverCaps& oAudioDriverCaps){
		std::memset(&oAudioDriverCaps, 0, sizeof(TAudioDriverCaps));
		return true;
	}
	
	
	
#endif // MAC
	
	
} // NSInternals






TAudioDriverID GetDefaultAudioDriverID()
{
	return NSInternals::GetDefaultAudioDriverID();
}
	
TAudioDriverIDs GetAudioDriverIDs(){
	return NSInternals::GetAudioDriverIDs();
}

bool GetAudioDriverCaps(const TAudioDriverID& iAudioDriverID, TAudioDriverCaps& oAudioDriverCaps){
	return NSInternals::GetAudioDriverCaps(iAudioDriverID, oAudioDriverCaps);
}








#if WINDOWS
TAudioDriverID kDefaultAudioDriverID = {0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0} };
#endif // WINDOWS
	
//#if MAC
//TAudioDriverID kDefaultAudioDriverID = kAudioHardwarePropertyDefaultOutputDevice;
//#endif // MAC
//
//	
//	

	
#if WINDOWS
	CSimpleAudioInstance::CSimpleAudioInstance(HWND iHWnd, ISimpleAudioRenderCallback* iSimpleAudioRenderCallback, TAudioDriverID iAudioDriverID)
	: fStreamingInstance(NULL)
	{
		fStreamingInstance = new NSInternals::CDXStreamingInstance(
			*this,
			iAudioDriverID,
			iHWnd,
			*iSimpleAudioRenderCallback);
	}		
#endif // WINDOWS
#if MAC
	CSimpleAudioInstance::CSimpleAudioInstance(ISimpleAudioRenderCallback* iSimpleAudioRenderCallback, TAudioDriverID iAudioDriverID)
	
	: fStreamingInstance(NULL)
{
	fStreamingInstance = new NSInternals::CCoreAudioDriver(iAudioDriverID, iSimpleAudioRenderCallback, *this);
}
#endif // MAC

CSimpleAudioInstance::~CSimpleAudioInstance(){
	delete fStreamingInstance;
	fStreamingInstance = NULL;
}

const TAudioDriverID& CSimpleAudioInstance::GetAudioDriverID() const{
	return fStreamingInstance->GetAudioDriverID();
}

TSampleRate CSimpleAudioInstance::GetSampleRate() const{
	return fStreamingInstance->GetSampleRate();
}

TChannelCount CSimpleAudioInstance::GetChannelCount() const{
	return fStreamingInstance->GetChannelCount();
}

TFrameCount CSimpleAudioInstance::GetBufferFrames() const{
	return fStreamingInstance->GetBufferFrames();
}




} // NSSimpleAudioAPI
