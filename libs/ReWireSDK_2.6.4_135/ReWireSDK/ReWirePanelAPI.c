/*
	ReWire by Propellerhead, (C) Copyright Propellerhead Software AB.
	ReWire and Propellerhead are trademarks of Propellerhead Software
	AB. email: rewire@propellerheads.se
*/

#if MAC
#include <Carbon/Carbon.h>
#endif /* MAC */

#include "ReWirePanelAPI.h"
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

typedef ReWireError (REWIRECALL *TRWPOpenProc)(ReWire_int32_t reWireLibVersion);
typedef ReWireError (REWIRECALL *TRWPIsCloseOKProc)(ReWire_char_t* okFlag);
typedef ReWireError (REWIRECALL *TRWPCloseProc)(void);
typedef ReWireError (REWIRECALL *TRWPIsReWireMixerAppRunning)(ReWire_char_t* isRunningFlag);
typedef ReWireError (REWIRECALL *TRWPLoadDeviceProc)(const ReWire_char_t deviceName[]);
typedef ReWireError (REWIRECALL *TRWPUnloadDeviceProc)(const ReWire_char_t deviceName[]);
typedef ReWireError (REWIRECALL *TRWPComConnectProc)(const ReWire_char_t signature[], TRWPPortHandle* panelPortHandle);
typedef ReWireError (REWIRECALL *TRWPComCheckConnectionProc)(TRWPPortHandle panelPortHandle);
typedef ReWireError (REWIRECALL *TRWPComDisconnectProc)(TRWPPortHandle panelPortHandle);
typedef ReWireError (REWIRECALL *TRWPComDoesMessageFitProc)(TRWPPortHandle panelPortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t messageSize);
typedef ReWireError (REWIRECALL *TRWPComSendProc)(TRWPPortHandle panelPortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t messageSize, ReWire_uint8_t* messageBody);
typedef ReWireError (REWIRECALL *TRWPComReadProc)(TRWPPortHandle panelPortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t* messageSize, ReWire_uint8_t* messageBody);
typedef ReWireError (REWIRECALL *TRWPComBytesAvailableProc)(TRWPPortHandle panelPortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t* bytesInBufferCount);
typedef ReWireError (REWIRECALL *TRWPRegisterReWireDeviceProc)(TReWireFileSpec deviceFilePath);
typedef ReWireError (REWIRECALL *TRWPUnregisterReWireDeviceProc)(TReWireFileSpec deviceFilePath);

static const char kRWPOpenProcName[] = "RWPOpenImp";
static const char kRWPIsCloseOKProcName[] = "RWPIsCloseOKImp";
static const char kRWPCloseProcName[] = "RWPCloseImp";
static const char kRWPIsReWireMixerAppRunning[] = "RWIsReWireMixerAppRunningImp";
static const char kRWPLoadDeviceName[] = "RWPLoadDeviceImp";
static const char kRWPUnloadDeviceName[] = "RWPUnloadDeviceImp";
static const char kRWPComConnectProcName[] = "RWPComConnectImp";
static const char kRWPComCheckConnectionProcName[] = "RWPComCheckConnectionImp";
static const char kRWPComDisconnectProcName[] = "RWPComDisconnectImp";
static const char kRWPComDoesMessageFitProcName[] = "RWPComDoesMessageFitImp";
static const char kRWPComSendProcName[] = "RWPComSendImp";
static const char kRWPComReadProcName[] = "RWPComReadImp";
static const char kRWPComBytesAvailableProcName[] = "RWPComBytesAvailableImp";
static const char kRWPRegisterReWireDeviceProcName[] = "RWPRegisterReWireDeviceImp";
static const char kRWPUnregisterReWireDeviceProcName[] = "RWPUnregisterReWireDeviceImp";

static TRWPOpenProc gRWPOpenProc = NULL;
static TRWPIsCloseOKProc gRWPIsCloseOKProc = NULL;
static TRWPCloseProc gRWPCloseProc = NULL;
static TRWPIsReWireMixerAppRunning gRWPIsReWireMixerAppRunning = NULL;
static TRWPLoadDeviceProc gRWPLoadDeviceProc = NULL;
static TRWPUnloadDeviceProc gRWPUnloadDeviceProc = NULL;
static TRWPComConnectProc gRWPComConnectProc = NULL;
static TRWPComCheckConnectionProc gRWPComCheckConnectionProc = NULL;
static TRWPComDisconnectProc gRWPComDisconnectProc = NULL;
static TRWPComDoesMessageFitProc gRWPComDoesMessageFitProc = NULL;
static TRWPComSendProc gRWPComSendProc = NULL;
static TRWPComReadProc gRWPComReadProc = NULL;
static TRWPComBytesAvailableProc gRWPComBytesAvailableProc = NULL;
static TRWPRegisterReWireDeviceProc gRWPRegisterReWireDeviceProc = NULL;
static TRWPUnregisterReWireDeviceProc gRWPUnregisterReWireDeviceProc = NULL;



static char GetDLLFunctionPointers() {
	gRWPOpenProc = (TRWPOpenProc)ReWireFindReWireSharedLibraryFunction(kRWPOpenProcName);
	gRWPIsCloseOKProc = (TRWPIsCloseOKProc)ReWireFindReWireSharedLibraryFunction(kRWPIsCloseOKProcName);
	gRWPCloseProc = (TRWPCloseProc)ReWireFindReWireSharedLibraryFunction(kRWPCloseProcName);
	gRWPIsReWireMixerAppRunning = (TRWPIsReWireMixerAppRunning)ReWireFindReWireSharedLibraryFunction(kRWPIsReWireMixerAppRunning);
	gRWPLoadDeviceProc = (TRWPLoadDeviceProc)ReWireFindReWireSharedLibraryFunction(kRWPLoadDeviceName);
	gRWPUnloadDeviceProc = (TRWPUnloadDeviceProc)ReWireFindReWireSharedLibraryFunction(kRWPUnloadDeviceName);
	gRWPComConnectProc = (TRWPComConnectProc)ReWireFindReWireSharedLibraryFunction(kRWPComConnectProcName);
	gRWPComCheckConnectionProc = (TRWPComCheckConnectionProc)ReWireFindReWireSharedLibraryFunction(kRWPComCheckConnectionProcName);
	gRWPComDisconnectProc = (TRWPComDisconnectProc)ReWireFindReWireSharedLibraryFunction(kRWPComDisconnectProcName);
	gRWPComDoesMessageFitProc = (TRWPComDoesMessageFitProc)ReWireFindReWireSharedLibraryFunction(kRWPComDoesMessageFitProcName);
	gRWPComSendProc = (TRWPComSendProc)ReWireFindReWireSharedLibraryFunction(kRWPComSendProcName);
	gRWPComReadProc = (TRWPComReadProc)ReWireFindReWireSharedLibraryFunction(kRWPComReadProcName);
	gRWPComBytesAvailableProc = (TRWPComBytesAvailableProc)ReWireFindReWireSharedLibraryFunction(kRWPComBytesAvailableProcName);
	gRWPRegisterReWireDeviceProc = (TRWPRegisterReWireDeviceProc)ReWireFindReWireSharedLibraryFunction(kRWPRegisterReWireDeviceProcName);
	gRWPUnregisterReWireDeviceProc = (TRWPUnregisterReWireDeviceProc)ReWireFindReWireSharedLibraryFunction(kRWPUnregisterReWireDeviceProcName);
	if (	(NULL==gRWPOpenProc) ||
		(NULL==gRWPIsCloseOKProc) ||
		(NULL==gRWPCloseProc) ||
		(NULL==gRWPIsReWireMixerAppRunning) ||
		(NULL==gRWPLoadDeviceProc) ||
		(NULL==gRWPUnloadDeviceProc) ||
		(NULL==gRWPComConnectProc) ||
		(NULL==gRWPComCheckConnectionProc) ||
		(NULL==gRWPComDisconnectProc) ||
		(NULL==gRWPComDoesMessageFitProc) ||
		(NULL==gRWPComSendProc) ||
		(NULL==gRWPComReadProc) ||
		(NULL==gRWPComBytesAvailableProc)  ||
		(NULL==gRWPRegisterReWireDeviceProc)  ||
		(NULL==gRWPUnregisterReWireDeviceProc))
	{
		return 0;
	}
	else {
		return 1;
	}
}

#if DEBUG
static char VerifyReWireDLLFunctions() {
	if (NULL == gRWPOpenProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPIsCloseOKProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPCloseProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPIsReWireMixerAppRunning) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPLoadDeviceProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPUnloadDeviceProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPComConnectProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPComCheckConnectionProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPComDisconnectProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPComDoesMessageFitProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPComSendProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPComReadProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPComBytesAvailableProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPRegisterReWireDeviceProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWPUnregisterReWireDeviceProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	return 1;
}
#endif /* DEBUG */

static void CleanUp() {
	gRWPOpenProc = NULL;
	gRWPIsCloseOKProc = NULL;
	gRWPCloseProc = NULL;
	gRWPIsReWireMixerAppRunning = NULL;
	gRWPLoadDeviceProc = NULL;
	gRWPUnloadDeviceProc = NULL;
	gRWPComConnectProc = NULL;
	gRWPComCheckConnectionProc = NULL;
	gRWPComDisconnectProc = NULL;
	gRWPComDoesMessageFitProc = NULL;
	gRWPComSendProc = NULL;
	gRWPComReadProc = NULL;
	gRWPComBytesAvailableProc = NULL;
	gRWPRegisterReWireDeviceProc = NULL;
	gRWPUnregisterReWireDeviceProc = NULL;
	ReWireUnloadReWireSharedLibrary();
}

ReWireError RWPOpen(void) {
	if (gRWPOpenProc != NULL) {
		REWIRE_ASSERT(VerifyReWireDLLFunctions());
		return kReWireImplError_ReWireAlreadyOpen;
	}
	else {
		ReWireError result = ReWireLoadReWireSharedLibrary();
		if (result == kReWireError_NoError) {
			if (GetDLLFunctionPointers()) {
				REWIRE_ASSERT(VerifyReWireDLLFunctions());
				result = (gRWPOpenProc)(REWIRE_PANEL_APP_API_VERSION);
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

ReWireError RWPClose(void) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	{
		ReWireError result = (*gRWPCloseProc)();
		if (result == kReWireError_NoError) {
			CleanUp();
		}
		return result;
	}
}

ReWireError RWPIsCloseOK(ReWire_char_t* okFlag) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWPIsCloseOKProc)(okFlag);
}

ReWireError RWPIsReWireMixerAppRunning(ReWire_char_t* isRunningFlag) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (gRWPIsReWireMixerAppRunning)(isRunningFlag);
}

ReWireError RWPLoadDevice(const ReWire_char_t deviceName[]) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWPLoadDeviceProc)(deviceName);
}

ReWireError RWPUnloadDevice(const ReWire_char_t deviceName[]) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWPUnloadDeviceProc)(deviceName);
}

ReWireError RWPComConnect(const ReWire_char_t signature[],TRWPPortHandle* panelPortHandle) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWPComConnectProc)(signature,panelPortHandle);
}

ReWireError RWPComCheckConnection(TRWPPortHandle panelPortHandle) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWPComCheckConnectionProc)(panelPortHandle);
}

ReWireError RWPComDisconnect(TRWPPortHandle panelPortHandle) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWPComDisconnectProc)(panelPortHandle);
}

ReWireError RWPComDoesMessageFit(TRWPPortHandle panelPortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t messageSize) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWPComDoesMessageFitProc)(panelPortHandle, pipeIndex, messageSize);
}

ReWireError RWPComSend(TRWPPortHandle panelPortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t messageSize, ReWire_uint8_t* messageBody) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWPComSendProc)(panelPortHandle, pipeIndex, messageSize, messageBody);
}

ReWireError RWPComRead(TRWPPortHandle panelPortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t* messageSize, ReWire_uint8_t* messageBody) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWPComReadProc)(panelPortHandle, pipeIndex, messageSize, messageBody);
}

ReWireError RWPComBytesAvailable(TRWPPortHandle panelPortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t* bytesInBufferCount) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWPComBytesAvailableProc)(panelPortHandle, pipeIndex, bytesInBufferCount);
}

ReWireError RWPRegisterReWireDevice(TReWireFileSpec deviceFilePath) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWPRegisterReWireDeviceProc)(deviceFilePath);
}

ReWireError RWPUnregisterReWireDevice(TReWireFileSpec deviceFilePath) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWPUnregisterReWireDeviceProc)(deviceFilePath);
}

#ifdef __cplusplus
}
#endif /* def __cplusplus */

