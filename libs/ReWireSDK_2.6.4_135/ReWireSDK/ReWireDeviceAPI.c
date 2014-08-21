/*
	ReWire by Propellerhead, (C) Copyright Propellerhead Software AB.
	ReWire and Propellerhead are trademarks of Propellerhead Software
	AB. email: rewire@propellerheads.se
*/

#include "ReWireDeviceAPI.h"
#include "ReWireAPI.h"

/*
 *
 * Local assertion macro.
 *
 */
#include <assert.h>
#define REWIRE_ASSERT(e) assert(e)

#ifndef NULL
	#define NULL    0
#endif /* NULL */

#ifdef __cplusplus
namespace ReWire {
#endif /* def __cplusplus */

typedef ReWireError (REWIRECALL *TRWDOpenProc)(ReWire_int32_t reWireLibVersion);
typedef ReWireError (REWIRECALL *TRWDIsCloseOKProc)(ReWire_char_t* okFlag);
typedef ReWireError (REWIRECALL *TRWDCloseProc)(void);
typedef ReWireError (REWIRECALL *TRWDIsReWireMixerAppRunning)(ReWire_char_t* isRunningFlag);
typedef ReWireError (REWIRECALL *TRWDComCreateProc)(const ReWire_char_t signature[], ReWire_uint16_t pipeCount, const ReWirePipeInfo pipeInfo[], TRWDPortHandle* devicePortHandle);
typedef ReWireError (REWIRECALL *TRWDComCheckConnectionProc)(TRWDPortHandle devicePortHandle);
typedef ReWireError (REWIRECALL *TRWDComDestroyProc)(TRWDPortHandle devicePortHandle);
typedef ReWireError (REWIRECALL *TRWDComDoesMessageFitProc)(TRWDPortHandle devicePortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t messageSize);
typedef ReWireError (REWIRECALL *TRWDComSendProc)(TRWDPortHandle devicePortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t messageSize, ReWire_uint8_t* messageBody);
typedef ReWireError (REWIRECALL *TRWDComReadProc)(TRWDPortHandle devicePortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t* messageSize, ReWire_uint8_t* messageBody);
typedef ReWireError (REWIRECALL *TRWDComBytesAvailableProc)(TRWDPortHandle devicePortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t* bytesInBufferCount);

static const char kRWDOpenProcName[] = "RWDOpenImp";
static const char kRWDIsCloseOKProcName[] = "RWDIsCloseOKImp";
static const char kRWDCloseProcName[] = "RWDCloseImp";
static const char kRWDIsReWireMixerAppRunning[] = "RWIsReWireMixerAppRunningImp";
static const char kRWDComCreateProcName[] = "RWDComCreateImp";
static const char kRWDComCheckConnectionProcName[] = "RWDComCheckConnectionImp";
static const char kRWDComDestroyProcName[] = "RWDComDestroyImp";
static const char kRWDComDoesMessageFitProcName[] = "RWDComDoesMessageFitImp";
static const char kRWDComSendProcName[] = "RWDComSendImp";
static const char kRWDComReadProcName[] = "RWDComReadImp";
static const char kRWDComBytesAvailableProcName[] = "RWDComBytesAvailableImp";

static TRWDOpenProc gRWDOpenProc = NULL;
static TRWDIsCloseOKProc gRWDIsCloseOKProc = NULL;
static TRWDCloseProc gRWDCloseProc = NULL;
static TRWDIsReWireMixerAppRunning gRWDIsReWireMixerAppRunningProc = NULL;
static TRWDComCreateProc gRWDComCreateProc = NULL;
static TRWDComCheckConnectionProc gRWDComCheckConnectionProc = NULL;
static TRWDComDestroyProc gRWDComDestroyProc = NULL;
static TRWDComDoesMessageFitProc gRWDComDoesMessageFitProc = NULL;
static TRWDComSendProc gRWDComSendProc = NULL;
static TRWDComReadProc gRWDComReadProc = NULL;
static TRWDComBytesAvailableProc gRWDComBytesAvailableProc = NULL;








static char GetDLLFunctionPointers() {
	gRWDOpenProc = (TRWDOpenProc)ReWireFindReWireSharedLibraryFunction(kRWDOpenProcName);
	gRWDIsCloseOKProc = (TRWDIsCloseOKProc)ReWireFindReWireSharedLibraryFunction(kRWDIsCloseOKProcName);
	gRWDCloseProc = (TRWDCloseProc)ReWireFindReWireSharedLibraryFunction(kRWDCloseProcName);
	gRWDIsReWireMixerAppRunningProc = (TRWDIsReWireMixerAppRunning)ReWireFindReWireSharedLibraryFunction(kRWDIsReWireMixerAppRunning);
	gRWDComCreateProc = (TRWDComCreateProc)ReWireFindReWireSharedLibraryFunction(kRWDComCreateProcName);
	gRWDComCheckConnectionProc = (TRWDComCheckConnectionProc)ReWireFindReWireSharedLibraryFunction(kRWDComCheckConnectionProcName);
	gRWDComDestroyProc = (TRWDComDestroyProc)ReWireFindReWireSharedLibraryFunction(kRWDComDestroyProcName);
	gRWDComDoesMessageFitProc = (TRWDComDoesMessageFitProc)ReWireFindReWireSharedLibraryFunction(kRWDComDoesMessageFitProcName);
	gRWDComSendProc = (TRWDComSendProc)ReWireFindReWireSharedLibraryFunction(kRWDComSendProcName);
	gRWDComReadProc = (TRWDComReadProc)ReWireFindReWireSharedLibraryFunction(kRWDComReadProcName);
	gRWDComBytesAvailableProc = (TRWDComBytesAvailableProc)ReWireFindReWireSharedLibraryFunction(kRWDComBytesAvailableProcName);
	if (	(NULL == gRWDOpenProc) ||
		(NULL == gRWDIsCloseOKProc) ||
		(NULL == gRWDCloseProc) ||
		(NULL == gRWDIsReWireMixerAppRunningProc) ||
		(NULL == gRWDComCreateProc) ||
		(NULL == gRWDComCheckConnectionProc) ||
		(NULL == gRWDComDestroyProc) ||
		(NULL == gRWDComDoesMessageFitProc) ||
		(NULL == gRWDComSendProc) ||
		(NULL == gRWDComReadProc) ||
		(NULL == gRWDComBytesAvailableProc) )
	{
		return 0;
	}
	else {
		return 1;
	}
}

#if DEBUG
static char VerifyReWireDLLFunctions() {
	if (NULL == gRWDOpenProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWDIsCloseOKProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWDCloseProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWDIsReWireMixerAppRunningProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWDComCreateProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWDComCheckConnectionProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWDComDestroyProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWDComDoesMessageFitProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWDComSendProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWDComReadProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	if (NULL == gRWDComBytesAvailableProc) {
		REWIRE_ASSERT(0);
		return 0;
	}
	return 1;
}
#endif /* DEBUG */

static void CleanUp() {
	gRWDOpenProc = NULL;
	gRWDIsCloseOKProc = NULL;
	gRWDCloseProc = NULL;
	gRWDIsReWireMixerAppRunningProc = NULL;
	gRWDComCreateProc = NULL;
	gRWDComCheckConnectionProc = NULL;
	gRWDComDestroyProc = NULL;
	gRWDComDoesMessageFitProc = NULL;
	gRWDComSendProc = NULL;
	gRWDComReadProc = NULL;
	gRWDComBytesAvailableProc = NULL;
	ReWireUnloadReWireSharedLibrary();
}

ReWireError RWDOpen() {
	if (gRWDOpenProc != NULL) {
		REWIRE_ASSERT(VerifyReWireDLLFunctions());
		return kReWireImplError_ReWireAlreadyOpen;
	}
	else {
		ReWireError result=ReWireLoadReWireSharedLibrary();
		if (result == kReWireError_NoError) {
			if (GetDLLFunctionPointers()) {
				REWIRE_ASSERT(VerifyReWireDLLFunctions());
				result = (gRWDOpenProc)(REWIRE_DEVICE_DLL_API_VERSION);
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

ReWireError RWDClose() {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	{
		ReWireError result = (*gRWDCloseProc)();
		if (result == kReWireError_NoError) {
			CleanUp();
		}
		return result;
	}
}

ReWireError RWDIsCloseOK(ReWire_char_t* okFlag) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWDIsCloseOKProc)(okFlag);
}

ReWireError RWDIsReWireMixerAppRunning(ReWire_char_t* isRunningFlag) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (gRWDIsReWireMixerAppRunningProc)(isRunningFlag);
}

ReWireError RWDComCreate(const char signature[],ReWire_uint16_t pipeCount, const ReWirePipeInfo pipeInfo[], TRWDPortHandle* devicePortHandle) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWDComCreateProc)(signature, pipeCount, pipeInfo, devicePortHandle);
}

ReWireError RWDComCheckConnection(TRWDPortHandle devicePortHandle) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWDComCheckConnectionProc)(devicePortHandle);
}

ReWireError RWDComDestroy(TRWDPortHandle devicePortHandle) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWDComDestroyProc)(devicePortHandle);
}

ReWireError RWDComDoesMessageFit(TRWDPortHandle devicePortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t messageSize) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWDComDoesMessageFitProc)(devicePortHandle, pipeIndex, messageSize);
}

ReWireError RWDComSend(TRWDPortHandle devicePortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t messageSize, ReWire_uint8_t* messageBody) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWDComSendProc)(devicePortHandle, pipeIndex, messageSize, messageBody);
}

ReWireError RWDComRead(TRWDPortHandle devicePortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t* messageSize, ReWire_uint8_t* messageBody) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWDComReadProc)(devicePortHandle, pipeIndex, messageSize, messageBody);
}

ReWireError RWDComBytesAvailable(TRWDPortHandle devicePortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t* bytesInBufferCount) {
	REWIRE_ASSERT(VerifyReWireDLLFunctions());
	return (*gRWDComBytesAvailableProc)(devicePortHandle, pipeIndex, bytesInBufferCount);
}

#ifdef __cplusplus
} /* namespace ReWire */
#endif /* def __cplusplus */
