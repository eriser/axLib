#include "axAudioEnvelope.h"

axAudioEnvelope::axAudioEnvelope()
{
	_sr = 44100;
	_attack = 0.0;
	_sustain = 1.0;
	_decay = 0.0; 
	_release = 0.0;

	_nAttackSample = _nDecaySample = _nReleaseSample = 0;

	_buffePos = 0;
	_active = false;
}

void axAudioEnvelope::TriggerNote()
{
	_buffePos = 0;
	_active = true;
}

void axAudioEnvelope::ReleaseNote()
{

}

unsigned int axAudioEnvelope::SecToSample(double seconde)
{
	return seconde * _sr;
}
double axAudioEnvelope::LineInterpolation(double y1, double y2, double mu)
{
	return y1 + mu * (y2 - y1);
}

void axAudioEnvelope::SetAttack(const axFloat& value)
{
	_attack = value;
	_nAttackSample = SecToSample(_attack);
}

void axAudioEnvelope::SetDecay(const axFloat& value)
{
	_decay= value;
	_nDecaySample = SecToSample(_decay);
}

axFloat axAudioEnvelope::Process()
{
	if (_buffePos < _nAttackSample)
	{
		_buffePos++;
		return LineInterpolation(0.0, _sustain, 
								 _buffePos / double(_nAttackSample));
	}
	else if (_buffePos < _nDecaySample)
	{
		_buffePos++;

		return LineInterpolation(1.0, 0.0,
			(_buffePos - _nAttackSample) / double(_nDecaySample));
	}
	
	return 0.0;
	//else if (_buffePos < _nDecaySample)
	//{

	//}
}