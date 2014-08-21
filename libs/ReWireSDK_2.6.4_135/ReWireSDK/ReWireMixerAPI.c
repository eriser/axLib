/*
	ReWire by Propellerhead, (C) Copyright Propellerhead Software AB.
	ReWire and Propellerhead are trademarks of Propellerhead Software
	AB. email: rewire@propellerheads.se
*/

#include "ReWireMixerAPI.h"
#include "ReWireAPI.h"

#include <stddef.h>

/*
 *
 * Local assertion macro.
 *
 */
#include <assert.h>
#define REWIRE_ASSERT(e) assert(e)

#ifdef __cplusplus
namespace ReWire {
#endif /* def __cplusplus */

	/* ReWireDLL functions */
typedef ReWireError (REWIRECALL *TReWireOpenProc)(ReWire_int32_t mixerAppReWireVersion, const ReWireOpenInfo* openInfo);
typedef ReWireError (REWIRECALL *TReWireIsCloseOKProc)(ReWire_char_t* okFlag);
typedef ReWireError (REWIRECALL *TReWireCloseProc)(void);
typedef ReWireError (REWIRECALL *TReWireGetDeviceCountProc)(ReWire_int32_t* deviceCount);
typedef ReWireError (REWIRECALL *TReWireGetDeviceInfo2Proc)(ReWire_int32_t deviceIndex, ReWireDeviceInfo* info);
typedef ReWireError (REWIRECALL *TReWireGetDeviceInfoByHandleProc)(TRWMDeviceHandle device, ReWireDeviceInfo* info);
typedef ReWireError (REWIRECALL *TReWireOpenDeviceProc)(ReWire_int32_t deviceIndex, TRWMDeviceHandle* device);
typedef ReWireError (REWIRECALL *TReWireIsCloseDeviceOKProc)(TRWMDeviceHandle device, ReWire_char_t* okFlag);
typedef ReWireError (REWIRECALL *TReWireCloseDeviceProc)(TRWMDeviceHandle device);
typedef ReWireError (REWIRECALL *TReWireDriveAudio2Proc)(TRWMDeviceHandle device, const ReWireDriveAudioInputParams* inputParams, ReWireDriveAudioOutputParams* outputParams);
typedef ReWireError (REWIRECALL *TReWireIdleProc)(void);
typedef ReWireError (REWIRECALL *TLaunchPanelAppProc)(TRWMDeviceHandle device);
typedef ReWireError (REWIRECALL *TIsPanelAppLaunchedProc)(TRWMDeviceHandle device, ReWire_char_t* isRunningFlag);
typedef ReWireError (REWIRECALL *TQuitPanelAppProc)(TRWMDeviceHandle device);
typedef ReWireError (REWIRECALL *TGetEventInfoProc)(TRWMDeviceHandle device, ReWireEventInfo* eventInfo);
typedef ReWireError (REWIRECALL *TGetEventBusInfoProc)(TRWMDeviceHandle device, ReWire_uint16_t busIndex, ReWireEventBusInfo* eventBusInfo);
typedef ReWireError (REWIRECALL *TGetEventChannelInfoProc)(TRWMDeviceHandle device, const ReWireEventTarget* eventTarget, ReWireEventChannelInfo* eventChannelInfo);
typedef ReWireError (REWIRECALL *TGetControllerInfoProc)(TRWMDeviceHandle device, const ReWireEventTarget* eventTarget, ReWire_uint16_t controllerIndex, ReWireEventControllerInfo* controllerInfo);
typedef ReWireError (REWIRECALL *TGetNoteInfoProc)(TRWMDeviceHandle device, const ReWireEventTarget* eventTarget, ReWire_uint16_t noteIndex, ReWireEventNoteInfo* noteInfo);
typedef ReWireError (REWIRECALL *TSetAudioInfoProc)(const ReWireAudioInfo* audioInfo);

	/* Function names as exported by ReWire.DLL */
static const char kRWMOpenProcName[] = "RWM2OpenImp";
static const char kRWMIsCloseOKProcName[] = "RWM2IsCloseOKImp";
static const char kRWMCloseProcName[] = "RWM2CloseImp";
static const char kRWMGetDeviceCountProcName[] = "RWM2GetDeviceCountImp";
static const char kRWMGetDeviceInfo2ProcName[] = "RWM2GetDeviceInfoImp";
static const char kRWMGetDeviceInfoByHandleProcName[] = "RWM2GetDeviceInfoByHandleImp";
static const char kRWMOpenDeviceProcName[] = "RWM2OpenDeviceImp";
static const char kRWMIsCloseDeviceOKProcName[] = "RWM2IsCloseDeviceOKImp";
static const char kRWMCloseDeviceProcName[] = "RWM2CloseDeviceImp";
static const char kRWMDriveAudio2ProcName[] = "RWM2DriveAudioImp";
static const char kRWMIdleProcName[] = "RWM2IdleImp";
static const char kRWMLaunchPanelAppProcName[] = "RWM2LaunchPanelAppImp";
static const char kRWMIsPanelAppLaunchedProcName[] = "RWM2IsPanelAppLaunchedImp";
static const char kRWMQuitPanelAppProcName[] = "RWM2QuitPanelAppImp";
static const char kRWMGetEventInfoName[] = "RWM2GetEventInfoImp";
static const char kRWMGetEventBusInfoName[] = "RWM2GetEventBusInfoImp";
static const char kRWMGetEventChannelInfoName[] = "RWM2GetEventChannelInfoImp";
static const char kRWMGetControllerInfoProcName[] = "RWM2GetControllerInfoImp";
static const char kRWMGetNoteInfoProcName[] = "RWM2GetNoteInfoImp";
static const char kRWMSetAudioInfoProcName[] = "RWM2SetAudioInfoImp";

static TReWireOpenProc gReWireOpenProc = NULL;
static TReWireIsCloseOKProc gReWireIsCloseOKProc = NULL;
static TReWireCloseProc gReWireCloseProc = NULL;
static TReWireGetDeviceCountProc gReWireGetDeviceCountProc = NULL;
static TReWireGetDeviceInfo2Proc gReWireGetDeviceInfo2Proc = NULL;
static TReWireGetDeviceInfoByHandleProc gReWireGetDeviceInfoByHandleProc = NULL;
static TReWireOpenDeviceProc gReWireOpenDeviceProc = NULL;
static TReWireIsCloseDeviceOKProc gReWireIsCloseDeviceOKProc = NULL;
static TReWireCloseDeviceProc gReWireCloseDeviceProc = NULL;
static TReWireDriveAudio2Proc gReWireDriveAudio2Proc = NULL;
static TReWireIdleProc gReWireIdleProc = NULL;
static TLaunchPanelAppProc gLaunchPanelAppProc = NULL;
static TIsPanelAppLaunchedProc gIsPanelAppLaunchedProc = NULL;
static TQuitPanelAppProc gQuitPanelAppProc = NULL;
static TGetEventInfoProc gGetEventInfoProc = NULL;
static TGetEventBusInfoProc gGetEventBusInfoProc = NULL;
static TGetEventChannelInfoProc gGetEventChannelInfoProc = NULL;
static TGetControllerInfoProc gGetControllerInfoProc = NULL;
static TGetNoteInfoProc gGetNoteInfoProc = NULL;
static TSetAudioInfoProc gSetAudioInfoProc = NULL;

static char GetDLLFunctionPointers() {
	gReWireOpenProc = (TReWireOpenProc)ReWireFindReWireSharedLibraryFunction(kRWMOpenProcName);
	gReWireIsCloseOKProc = (TReWireIsCloseOKProc)ReWireFindReWireSharedLibraryFunction(kRWMIsCloseOKProcName);
	gReWireCloseProc = (TReWireCloseProc)ReWireFindReWireSharedLibraryFunction(kRWMCloseProcName);
	gReWireGetDeviceCountProc = (TReWireGetDeviceCountProc)ReWireFindReWireSharedLibraryFunction(kRWMGetDeviceCountProcName);
	gReWireGetDeviceInfo2Proc = (TReWireGetDeviceInfo2Proc)ReWireFindReWireSharedLibraryFunction(kRWMGetDeviceInfo2ProcName);
	gReWireGetDeviceInfoByHandleProc = (TReWireGetDeviceInfoByHandleProc)ReWireFindReWireSharedLibraryFunction(kRWMGetDeviceInfoByHandleProcName);
	gReWireOpenDeviceProc = (TReWireOpenDeviceProc)ReWireFindReWireSharedLibraryFunction(kRWMOpenDeviceProcName);
	gReWireIsCloseDeviceOKProc = (TReWireIsCloseDeviceOKProc)ReWireFindReWireSharedLibraryFunction(kRWMIsCloseDeviceOKProcName);
	gReWireCloseDeviceProc = (TReWireCloseDeviceProc)ReWireFindReWireSharedLibraryFunction(kRWMCloseDeviceProcName);
	gReWireDriveAudio2Proc = (TReWireDriveAudio2Proc)ReWireFindReWireSharedLibraryFunction(kRWMDriveAudio2ProcName);
	gReWireIdleProc = (TReWireIdleProc)ReWireFindReWireSharedLibraryFunction(kRWMIdleProcName);
	gLaunchPanelAppProc = (TLaunchPanelAppProc)ReWireFindReWireSharedLibraryFunction(kRWMLaunchPanelAppProcName);
	gIsPanelAppLaunchedProc = (TIsPanelAppLaunchedProc)ReWireFindReWireSharedLibraryFunction(kRWMIsPanelAppLaunchedProcName);
	gQuitPanelAppProc = (TQuitPanelAppProc)ReWireFindReWireSharedLibraryFunction(kRWMQuitPanelAppProcName);
	gGetEventInfoProc = (TGetEventInfoProc)ReWireFindReWireSharedLibraryFunction(kRWMGetEventInfoName);
	gGetEventBusInfoProc = (TGetEventBusInfoProc)ReWireFindReWireSharedLibraryFunction(kRWMGetEventBusInfoName);
	gGetEventChannelInfoProc = (TGetEventChannelInfoProc)ReWireFindReWireSharedLibraryFunction(kRWMGetEventChannelInfoName);
	gGetControllerInfoProc = (TGetControllerInfoProc)ReWireFindReWireSharedLibraryFunction(kRWMGetControllerInfoProcName);
	gGetNoteInfoProc = (TGetNoteInfoProc)ReWireFindReWireSharedLibraryFunction(kRWMGetNoteInfoProcName);
	gSetAudioInfoProc = (TSetAudioInfoProc)ReWireFindReWireSharedLibraryFunction(kRWMSetAudioInfoProcName);

	if (	(NULL == gReWireOpenProc) ||
		(NULL == gReWireIsCloseOKProc) ||
		(NULL == gReWireCloseProc) ||
		(NULL == gReWireGetDeviceCountProc) ||
		(NULL == gReWireGetDeviceInfo2Proc) ||
		(NULL == gReWireGetDeviceInfoByHandleProc) ||
		(NULL == gReWireOpenDeviceProc) ||
		(NULL == gReWireIsCloseDeviceOKProc) ||
		(NULL == gReWireCloseDeviceProc) ||
		(NULL == gReWireDriveAudio2Proc) ||
		(NULL == gLaunchPanelAppProc) ||
		(NULL == gIsPanelAppLaunchedProc) ||
		(NULL == gQuitPanelAppProc) ||
		(NULL == gGetEventInfoProc) ||
		(NULL == gGetEventBusInfoProc) ||
		(NULL == gGetEventChannelInfoProc) ||
		(NULL == gGetControllerInfoProc) ||
		(NULL == gGetNoteInfoProc) ||
		(NULL == gSetAudioInfoProc))
	{
		return 0;
	}
	else {
		return 1;
	}
}

#if DEBUG
static char VerifyReWireDLLFunctions() {
	if (NULL == gReWireOpenProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gReWireIsCloseOKProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gReWireCloseProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gReWireGetDeviceCountProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gReWireGetDeviceInfo2Proc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gReWireGetDeviceInfoByHandleProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gReWireOpenDeviceProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gReWireIsCloseDeviceOKProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gReWireCloseDeviceProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gReWireDriveAudio2Proc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gReWireIdleProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gLaunchPanelAppProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gIsPanelAppLaunchedProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gQuitPanelAppProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gGetEventInfoProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gGetEventBusInfoProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gGetEventChannelInfoProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gGetControllerInfoProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gGetNoteInfoProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gSetAudioInfoProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	return 1;
}
#endif /* DEBUG */

static void CleanUp() {
	gReWireOpenProc = NULL;
	gReWireIsCloseOKProc = NULL;
	gReWireCloseProc = NULL;
	gReWireGetDeviceCountProc = NULL;
	gReWireGetDeviceInfo2Proc = NULL;
	gReWireGetDeviceInfoByHandleProc = NULL;
	gReWireOpenDeviceProc = NULL;
	gReWireIsCloseDeviceOKProc = NULL;
	gReWireCloseDeviceProc = NULL;
	gReWireDriveAudio2Proc = NULL;
	gReWireIdleProc = NULL;
	gLaunchPanelAppProc = NULL;
	gIsPanelAppLaunchedProc = NULL;
	gQuitPanelAppProc = NULL;
	gGetEventInfoProc = NULL;
	gGetEventBusInfoProc = NULL;
	gGetEventChannelInfoProc = NULL;
	gGetControllerInfoProc = NULL;
	gGetNoteInfoProc = NULL;
	ReWireUnloadReWireSharedLibrary();
}

ReWireError RWMOpen(const ReWireOpenInfo* openInfo) {
	if (gReWireOpenProc != NULL) {
		REWIRE_ASSERT(VerifyReWireDLLFunctions());
		return kReWireImplError_ReWireAlreadyOpen;
	}
	else {
		ReWireError result = ReWireLoadReWireSharedLibrary();
		if (result == kReWireError_NoError) {
			if (GetDLLFunctionPointers()) {
				REWIRE_ASSERT(VerifyReWireDLLFunctions());
				result = (*gReWireOpenProc)(REWIRE_MIXER_APP_API_VERSION, openInfo);
				if (result == kReWireError_NoError) {
					return kReWireError_NoError;
				}
			}
			else {
				result = kReWireError_DLLTooOld;
			}
			CleanUp();
		}
		return result;
	}
}

ReWireError RWMIsCloseOK(ReWire_char_t* okFlag) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gReWireIsCloseOKProc)(okFlag);
}

ReWireError RWMClose(void) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	{
		ReWireError result = (*gReWireCloseProc)();
		if (result == kReWireError_NoError) {
			CleanUp();
		}
		return result;
	}
}

ReWireError RWMGetDeviceCount(ReWire_int32_t* deviceCount) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gReWireGetDeviceCountProc)(deviceCount);
}

ReWireError RWMGetDeviceInfo(ReWire_int32_t deviceIndex, ReWireDeviceInfo* info) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gReWireGetDeviceInfo2Proc)(deviceIndex, info);
}

ReWireError RWMGetDeviceInfoByHandle(TRWMDeviceHandle device, ReWireDeviceInfo* info) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gReWireGetDeviceInfoByHandleProc)(device, info);
}

ReWireError RWMOpenDevice(ReWire_int32_t deviceIndex, TRWMDeviceHandle* device) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gReWireOpenDeviceProc)(deviceIndex, device);
}

ReWireError RWMIsCloseDeviceOK(TRWMDeviceHandle device, ReWire_char_t* okFlag) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gReWireIsCloseDeviceOKProc)(device, okFlag);
}

ReWireError RWMCloseDevice(TRWMDeviceHandle device) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gReWireCloseDeviceProc)(device);
}

ReWireError RWMDriveAudio(TRWMDeviceHandle device, const ReWireDriveAudioInputParams* inputParams, ReWireDriveAudioOutputParams* outputParams) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gReWireDriveAudio2Proc)(device, inputParams, outputParams);
}

ReWireError RWMIdle(void) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gReWireIdleProc)();
}

ReWireError RWMLaunchPanelApp(TRWMDeviceHandle device) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gLaunchPanelAppProc)(device);
}

ReWireError RWMIsPanelAppLaunched(TRWMDeviceHandle device, ReWire_char_t* isRunningFlag) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gIsPanelAppLaunchedProc)(device, isRunningFlag);
}

ReWireError RWMQuitPanelApp(TRWMDeviceHandle device) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gQuitPanelAppProc)(device);
}

ReWireError RWMGetEventInfo(TRWMDeviceHandle device, ReWireEventInfo* eventInfo) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gGetEventInfoProc)(device, eventInfo);
}

ReWireError RWMGetEventBusInfo(TRWMDeviceHandle device, ReWire_uint16_t busIndex, ReWireEventBusInfo* eventBusInfo) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gGetEventBusInfoProc)(device, busIndex, eventBusInfo);
}

ReWireError RWMGetEventChannelInfo(TRWMDeviceHandle device, const ReWireEventTarget* eventTarget, ReWireEventChannelInfo* eventChannelInfo) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gGetEventChannelInfoProc)(device, eventTarget, eventChannelInfo);
}

ReWireError RWMGetControllerInfo(TRWMDeviceHandle device, const ReWireEventTarget* eventTarget, ReWire_uint16_t controllerIndex, ReWireEventControllerInfo* controllerInfo) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gGetControllerInfoProc)(device, eventTarget, controllerIndex, controllerInfo);
}

ReWireError RWMGetNoteInfo(TRWMDeviceHandle device, const ReWireEventTarget* eventTarget, ReWire_uint16_t noteIndex, ReWireEventNoteInfo* noteInfo) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gGetNoteInfoProc)(device, eventTarget, noteIndex, noteInfo);
}

ReWireError RWMSetAudioInfo(const ReWireAudioInfo* audioInfo) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gSetAudioInfoProc)(audioInfo);
}

#ifdef __cplusplus
} /* namespace ReWire */
#endif  /* def __cplusplus */
