#ifndef REWIREMIXERAPI_H_
#define REWIREMIXERAPI_H_

/*
	ReWire by Propellerhead, (C) Copyright Propellerhead Software AB.
	ReWire and Propellerhead are trademarks of Propellerhead Software
	AB. email: rewire@propellerheads.se
*/

/*	Header file for stub/utility functions for a mixer application
	This is the API you use when writing a ReWire mixer application.	*/

#include "ReWire.h"

#ifdef __cplusplus
namespace ReWire {
#endif  /* def __cplusplus */

typedef void* TRWMDeviceHandle;

#ifdef __cplusplus
extern "C" {
#endif /* def __cplusplus */

extern ReWireError RWMOpen(const ReWireOpenInfo* openInfo);
extern ReWireError RWMIsCloseOK(ReWire_char_t* okFlag);
extern ReWireError RWMClose(void);
extern ReWireError RWMSetAudioInfo(const ReWireAudioInfo* audioInfo);
extern ReWireError RWMGetDeviceCount(ReWire_int32_t* deviceCount);
extern ReWireError RWMGetDeviceInfo(ReWire_int32_t deviceIndex, ReWireDeviceInfo* info);
extern ReWireError RWMGetDeviceInfoByHandle(TRWMDeviceHandle device, ReWireDeviceInfo* info);
extern ReWireError RWMOpenDevice(ReWire_int32_t deviceIndex, TRWMDeviceHandle* device);
extern ReWireError RWMIsCloseDeviceOK(TRWMDeviceHandle device, ReWire_char_t* okFlag);
extern ReWireError RWMCloseDevice(TRWMDeviceHandle device);
extern ReWireError RWMDriveAudio(TRWMDeviceHandle device, const ReWireDriveAudioInputParams* inputParams, ReWireDriveAudioOutputParams* outputParams);
extern ReWireError RWMIdle(void);
extern ReWireError RWMLaunchPanelApp(TRWMDeviceHandle device);
extern ReWireError RWMIsPanelAppLaunched(TRWMDeviceHandle device, ReWire_char_t* isRunningFlag);
extern ReWireError RWMQuitPanelApp(TRWMDeviceHandle device);
extern ReWireError RWMGetEventInfo(TRWMDeviceHandle device, ReWireEventInfo* eventInfo);
extern ReWireError RWMGetEventBusInfo(TRWMDeviceHandle device, ReWire_uint16_t busIndex, ReWireEventBusInfo* eventBusInfo);
extern ReWireError RWMGetEventChannelInfo(TRWMDeviceHandle device, const ReWireEventTarget* eventTarget, ReWireEventChannelInfo* eventChannelInfo);
extern ReWireError RWMGetControllerInfo(TRWMDeviceHandle device, const ReWireEventTarget* eventTarget, ReWire_uint16_t controllerIndex, ReWireEventControllerInfo* controllerInfo);
extern ReWireError RWMGetNoteInfo(TRWMDeviceHandle device, const ReWireEventTarget* eventTarget, ReWire_uint16_t noteIndex, ReWireEventNoteInfo* noteInfo);

#ifdef __cplusplus
}
#endif /* def __cplusplus */

#ifdef __cplusplus
}
#endif  /* def __cplusplus */

#endif /* REWIREMIXERAPI_H_ */
