#ifndef MIXER_H_
#define MIXER_H_

#include "ReWireSDK/ReWire.h"

const unsigned int kMixerChannelCount=8;

class CMixerParams {
	public: CMixerParams();
	public: ReWire::ReWire_uint8_t fMixerLevel[kMixerChannelCount];
	public: bool fSelectIx[kMixerChannelCount];
	public: bool fPlayFlag;
};

#endif // MIXER_H_
