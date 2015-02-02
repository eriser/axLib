#pragma once

#include <vector>

#if WINDOWS

#ifdef DIRECTSOUND_VERSION
	#if (DIRECTSOUND_VERSION < REQUIRED_DIRECTX_VERSION)
		#pragma message("DIRECTSOUND_VERSION already defined! May cause problems on WinNT!");
	#endif
#else
	#define DIRECTSOUND_VERSION REQUIRED_DIRECTX_VERSION
#endif

#include <dsound.h>

#endif // WINDOWS

#if MAC
#include <CoreAudio/CoreAudio.h>
#endif // MAC

namespace NSSimpleAudioAPI {

#if WINDOWS

typedef UINT32 TSampleRate;
typedef INT32 TFrameCount;
typedef UINT32 TChannelCount;

typedef GUID TAudioDriverID;

typedef struct TAudioDriverCaps_tag {
	DSCAPS fCaps;
	TCHAR lpszDesc[511 + 1];
	TCHAR lpszDrvName[511 + 1];
} TAudioDriverCaps;


#endif // WINDOWS

#if MAC
	typedef UInt32 TSampleRate;
	typedef UInt32 TFrameCount;
	typedef UInt32 TChannelCount;
	
	typedef AudioDeviceID TAudioDriverID;

	typedef struct TAudioDriverCaps_tag {
		char fName[511 + 1];
	} TAudioDriverCaps;
	
		
#endif // MAC


typedef float TFloatSample;
typedef std::vector<TFloatSample*> TFloatSampleBuffers;

typedef std::vector<TAudioDriverID> TAudioDriverIDs;


TAudioDriverID GetDefaultAudioDriverID();
TAudioDriverIDs GetAudioDriverIDs();
bool GetAudioDriverCaps(const TAudioDriverID& iAudioDriverID, TAudioDriverCaps& oAudioDriverCaps);






namespace NSInternals {
	class CStreamingInstance;
} // NSInternals


class ISimpleAudioRenderCallback;
	
class CSimpleAudioInstance {
#if WINDOWS
	public: CSimpleAudioInstance(HWND iHWnd, ISimpleAudioRenderCallback* iSimpleAudioRenderCallback, TAudioDriverID	iAudioDriverID);
#endif // WINDOWS
#if MAC
	public: CSimpleAudioInstance(ISimpleAudioRenderCallback* iSimpleAudioRenderCallback, TAudioDriverID	iAudioDriverID);
#endif // MAC
						
	public: ~CSimpleAudioInstance();

	public: const TAudioDriverID& GetAudioDriverID() const;

	public: TSampleRate GetSampleRate() const;
	public: TChannelCount GetChannelCount() const;
	public: TFrameCount GetBufferFrames() const;

	private: NSInternals::CStreamingInstance* fStreamingInstance;
};
	



class ISimpleAudioRenderCallback {
	protected: virtual ~ISimpleAudioRenderCallback() {}

	public: virtual void ISimpleAudioRenderCallback_Render(
				CSimpleAudioInstance& iInstance,
				TFrameCount frames,
				const TFloatSampleBuffers& outputBuffers) = 0;
};

} // NSSimpleAudioAPI
