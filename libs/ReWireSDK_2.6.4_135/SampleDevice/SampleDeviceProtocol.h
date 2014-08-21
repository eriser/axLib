#ifndef SAMPLEDEVICEPROTOCOL_H_
#define SAMPLEDEVICEPROTOCOL_H_

#include "ReWireSDK/ReWire.h"

const ReWire::ReWire_char_t kSampleDeviceName[] = "ReWire sample device";

const ReWire::ReWire_char_t kSampleDeviceSignature[]="SDsg";

enum EMessage {
	kSetTempo,	// ReWire_uint32_t tempoBPM*1000
	kPlay,
	kStop,
	kRequestQuit
};

#endif // SAMPLEDEVICEPROTOCOL_H_
