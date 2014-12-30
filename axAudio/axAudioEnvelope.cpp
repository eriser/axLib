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

//double axAudioEnvelope::LineInterpolation(double y1, double y2, double mu)
//{
//	return y1 + mu * (y2 - y1);
//}

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

void axAudioEnvelope::SetRelease(const axFloat& value)
{
    _release = value;
}

axFloat axAudioEnvelope::Process()
{
    
    // Attack.
	if (_buffePos < _nAttackSample)
	{
		_buffePos++;
        double mu = _buffePos / double(_nAttackSample);
        return axLineairInterpole<axFloat>(0.0, _sustain, mu);
//		return LineInterpolation(0.0, _sustain, 
//								 _buffePos / double(_nAttackSample));
	}
    
    // Decay.
	else if (_buffePos < _nDecaySample)
	{
		_buffePos++;
        
        double mu = (_buffePos - _nAttackSample) / double(_nDecaySample);
        return axLineairInterpole<axFloat>(1.0, 0.0, mu);

//		return LineInterpolation(1.0, 0.0,
//			(_buffePos - _nAttackSample) / double(_nDecaySample));
	}
	
	return 0.0;
}