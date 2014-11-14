#ifndef __AX_AUDIO_ENVELOPE__
#define __AX_AUDIO_ENVELOPE__

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <vector>
#include <string>
#include "axUtils.h"
#include "axC++.h"

//-----------------------------------------------------------------------------
//  Audio_Filter
//-----------------------------------------------------------------------------
class axAudioEnvelope
{
public:
	axAudioEnvelope();
	axFloat Process();

	void TriggerNote();
	void ReleaseNote();

	void SetAttack(const axFloat& value);
	void SetDecay(const axFloat& value);

private:
	axFloat _attack, _sustain, _decay, _release;
	unsigned int _sr, _buffePos;
	unsigned int _nAttackSample, _nDecaySample, _nReleaseSample;
	bool _active;

	unsigned int SecToSample(double seconde);
	double LineInterpolation(double y1, double y2, double mu);
};


#endif // __AX_AUDIO_ENVELOPE__