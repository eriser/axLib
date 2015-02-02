#ifndef REWIREDEVICEAPI_H_
#define REWIREDEVICEAPI_H_

/*
	ReWire by Propellerhead, (C) Copyright Propellerhead Software AB.
	ReWire and Propellerhead are trademarks of Propellerhead Software
	AB. email: rewire@propellerheads.se
*/

/*	Header file for stub/utility functions for implementing a ReWire device
	It is optional for device to use these functions and include this header. */

#include "ReWire.h"

#ifdef __cplusplus
namespace ReWire {
#endif /* def __cplusplus */

typedef void* TRWDPortHandle;

#ifdef __cplusplus
extern "C" {
#endif /* def __cplusplus */

extern ReWireError RWDOpen(void);
extern ReWireError RWDIsCloseOK(ReWire_char_t* okFlag);
extern ReWireError RWDClose(void);
extern ReWireError RWDIsReWireMixerAppRunning(ReWire_char_t* isRunningFlag);
extern ReWireError RWDComCreate(const ReWire_char_t signature[], ReWire_uint16_t pipeCount, const ReWirePipeInfo pipeInfo[], TRWDPortHandle* devicePortHandle);
extern ReWireError RWDComCheckConnection(TRWDPortHandle devicePortHandle);
extern ReWireError RWDComDestroy(TRWDPortHandle devicePortHandle);
extern ReWireError RWDComDoesMessageFit(TRWDPortHandle devicePortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t messageSize);
extern ReWireError RWDComSend(TRWDPortHandle devicePortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t messageSize, ReWire_uint8_t* messageBody);
extern ReWireError RWDComRead(TRWDPortHandle devicePortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t* messageSize, ReWire_uint8_t* messageBody);
extern ReWireError RWDComBytesAvailable(TRWDPortHandle devicePortHandle, ReWire_uint16_t pipeIndex, ReWire_uint16_t* bytesInBufferCount);

#ifdef __cplusplus
}
#endif /* def __cplusplus */

#ifdef __cplusplus
}
#endif /* def __cplusplus */

#endif /* REWIREDEVICEAPI_H_ */
