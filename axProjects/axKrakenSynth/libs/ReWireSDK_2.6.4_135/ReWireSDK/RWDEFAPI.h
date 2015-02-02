#ifndef RWDEFAPI_H_
#define RWDEFAPI_H_
/*
	ReWire by Propellerhead, (C) Copyright Propellerhead Software AB.
	ReWire and Propellerhead are trademarks of Propellerhead Software
	AB. email: rewire@propellerheads.se
*/

/*	Prototypes for the functions a ReWire device DLL/bundle must export. */

#include "ReWire.h"

#if WINDOWS
#define REWIREEXPORT __declspec( dllexport )
#endif /* WINDOWS */

#if MAC
#define REWIREEXPORT
#endif /* MAC */

#if WINDOWS && !_WIN64
#define REWIRECALL _cdecl
#else
#define REWIRECALL
#endif /* WINDOWS && !_WIN64 */


#ifdef __cplusplus

extern "C" {

REWIREEXPORT void REWIRECALL RWDEFGetDeviceNameAndVersion(ReWire::ReWire_int32_t* codedForReWireVersion, ReWire::ReWire_char_t name[]);

REWIREEXPORT void REWIRECALL RWDEFGetDeviceInfo(ReWire::ReWireDeviceInfo* info);
REWIREEXPORT ReWire::ReWireError REWIRECALL RWDEFOpenDevice(const ReWire::ReWireOpenInfo* openInfo);
REWIREEXPORT ReWire::ReWire_char_t REWIRECALL RWDEFIsCloseOK(void);
REWIREEXPORT void REWIRECALL RWDEFCloseDevice(void);
REWIREEXPORT void REWIRECALL RWDEFDriveAudio(const ReWire::ReWireDriveAudioInputParams* inputParams, ReWire::ReWireDriveAudioOutputParams* outputParams);
REWIREEXPORT void REWIRECALL RWDEFIdle(void);
REWIREEXPORT void REWIRECALL RWDEFSetAudioInfo(const ReWire::ReWireAudioInfo* audioInfo);
REWIREEXPORT ReWire::ReWireError REWIRECALL RWDEFLaunchPanelApp();
REWIREEXPORT ReWire::ReWire_char_t REWIRECALL RWDEFIsPanelAppLaunched();
REWIREEXPORT ReWire::ReWireError REWIRECALL RWDEFQuitPanelApp();
REWIREEXPORT void REWIRECALL RWDEFGetEventInfo(ReWire::ReWireEventInfo* eventInfo);
REWIREEXPORT void REWIRECALL RWDEFGetEventBusInfo(ReWire::ReWire_uint16_t busIndex, ReWire::ReWireEventBusInfo* eventBusInfo);
REWIREEXPORT void REWIRECALL RWDEFGetEventChannelInfo(const ReWire::ReWireEventTarget* eventTarget, ReWire::ReWireEventChannelInfo* eventChannelInfo);
REWIREEXPORT void REWIRECALL RWDEFGetEventControllerInfo(const ReWire::ReWireEventTarget* eventTarget, ReWire::ReWire_uint16_t controllerIndex, ReWire::ReWireEventControllerInfo* controllerInfo);
REWIREEXPORT void REWIRECALL RWDEFGetEventNoteInfo(const ReWire::ReWireEventTarget* eventTarget, ReWire::ReWire_uint16_t noteIndex, ReWire::ReWireEventNoteInfo* noteInfo);

} /* extern "C" */

#else /* __cplusplus */

REWIREEXPORT void REWIRECALL RWDEFGetDeviceNameAndVersion(ReWire_int32_t* codedForReWireVersion, ReWire_char_t name[]);

REWIREEXPORT void REWIRECALL RWDEFGetDeviceInfo(ReWireDeviceInfo* info);
REWIREEXPORT ReWireError REWIRECALL RWDEFOpenDevice(const ReWireOpenInfo* openInfo);
REWIREEXPORT ReWire_char_t REWIRECALL RWDEFIsCloseOK(void);
REWIREEXPORT void REWIRECALL RWDEFCloseDevice(void);
REWIREEXPORT void REWIRECALL RWDEFDriveAudio(const ReWireDriveAudioInputParams* inputParams, ReWireDriveAudioOutputParams* outputParams);
REWIREEXPORT void REWIRECALL RWDEFIdle(void);
REWIREEXPORT void REWIRECALL RWDEFSetAudioInfo(const ReWireAudioInfo* audioInfo);
REWIREEXPORT ReWireError REWIRECALL RWDEFLaunchPanelApp();
REWIREEXPORT ReWire_char_t REWIRECALL RWDEFIsPanelAppLaunched();
REWIREEXPORT ReWireError REWIRECALL RWDEFQuitPanelApp();
REWIREEXPORT void REWIRECALL RWDEFGetEventInfo(ReWireEventInfo* eventInfo);
REWIREEXPORT void REWIRECALL RWDEFGetEventBusInfo(ReWire_uint16_t busIndex, ReWireEventBusInfo* eventBusInfo);
REWIREEXPORT void REWIRECALL RWDEFGetEventChannelInfo(const ReWireEventTarget* eventTarget, ReWireEventChannelInfo* eventChannelInfo);
REWIREEXPORT void REWIRECALL RWDEFGetEventControllerInfo(const ReWireEventTarget* eventTarget, ReWire_uint16_t controllerIndex, ReWireEventControllerInfo* controllerInfo);
REWIREEXPORT void REWIRECALL RWDEFGetEventNoteInfo(const ReWireEventTarget* eventTarget, ReWire_uint16_t noteIndex, ReWireEventNoteInfo* noteInfo);

#endif /* __cplusplus */

#endif /* RWDEFAPI_H_ */
