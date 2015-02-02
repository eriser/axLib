#ifndef REWIREPANELAPI_H_
#define REWIREPANELAPI_H_

/*
	ReWire by Propellerhead, (C) Copyright Propellerhead Software AB.
	ReWire and Propellerhead are trademarks of Propellerhead Software
	AB. email: rewire@propellerheads.se
*/

/*	Header file for stub/utility functions for writing a ReWire panel application
	It is optional to use this API/header - panel can chose to communicate
	with its device dll some other way. */

#include "ReWire.h"

#ifdef __cplusplus
namespace ReWire {
#endif /* def __cplusplus */

typedef void* TRWPPortHandle;


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern ReWireError RWPOpen(void);
extern ReWireError RWPIsCloseOK(ReWire_char_t* okFlag);
extern ReWireError RWPClose(void);
extern ReWireError RWPIsReWireMixerAppRunning(ReWire_char_t* isRunningFlag);
extern ReWireError RWPLoadDevice(const ReWire_char_t deviceName[]);
extern ReWireError RWPUnloadDevice(const ReWire_char_t deviceName[]);
extern ReWireError RWPComConnect(const ReWire_char_t signature[],TRWPPortHandle* panelPortHandle);
extern ReWireError RWPComCheckConnection(TRWPPortHandle panelPortHandle);
extern ReWireError RWPComDisconnect(TRWPPortHandle panelPortHandle);
extern ReWireError RWPComDoesMessageFit(TRWPPortHandle panelPortHandle,ReWire_uint16_t pipeIndex,ReWire_uint16_t messageSize);
extern ReWireError RWPComSend(TRWPPortHandle panelPortHandle,ReWire_uint16_t pipeIndex,ReWire_uint16_t messageSize,ReWire_uint8_t* messageBody);
extern ReWireError RWPComRead(TRWPPortHandle panelPortHandle,ReWire_uint16_t pipeIndex,ReWire_uint16_t* messageSize,ReWire_uint8_t* messageBody);
extern ReWireError RWPComBytesAvailable(TRWPPortHandle panelPortHandle,ReWire_uint16_t pipeIndex,ReWire_uint16_t* bytesInBufferCount);
extern ReWireError RWPRegisterReWireDevice(TReWireFileSpec deviceFilePath);
extern ReWireError RWPUnregisterReWireDevice(TReWireFileSpec deviceFilePath);

#ifdef __cplusplus
} /* extern "C" */
#endif /*def __cplusplus */

#ifdef __cplusplus
} /* namespace ReWire */
#endif /*def __cplusplus */

#endif /* REWIREPANELAPI_H_ */
