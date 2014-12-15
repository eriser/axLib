#ifndef __AX_AUDIO_FILTER__
#define __AX_AUDIO_FILTER__

#define _USE_MATH_DEFINES
#include <cmath>

#include <iostream>
#include <vector>
#include <string>
//#include <random>
//#include <fstream>
#include "axUtils.h"
#include "axC++.h"

//#include "portaudio.h" 

#ifndef TWOPI
#define TWOPI (6.2831853071795864)
#endif

#ifndef ST_RATIO
#define ST_RATIO (1.0594630943592952)
#endif

#define CLIP(x, min, max) if(x > max){ x = max; }if(x < min){ x = min;}
#define MIN(x, min) if(x < min) x = min;
#define MAX(x, max) if(x > max) x = max;


enum axAudioFilterType
{
	axAUDIO_FILTER_LPF,
	axAUDIO_FILTER_HPF,
	axAUDIO_FILTER_BPF
};

struct axAudioStereoOutput
{
	axFloat left, right;
};

typedef struct _out //STEREO
{
	axFloat l, r;
} t_out;

//-----------------------------------------------------------------------------
//  Audio_Filter
//-----------------------------------------------------------------------------
class axAudioFilter
{
public:
	axAudioFilter();
	axFloat Process(axFloat in);
    void ProcessMonoBlock(float* out,
                          unsigned long frameCount);
    void ProcessStereoBlock(float* out,
                          unsigned long frameCount);
	t_out ProcessStereo(t_out in);
	void SetFreq(axFloat f);
	void SetQ(axFloat f);
	void SetGain(axFloat f);
    
    axFloat GetFreq() const;
    axFloat GetQ() const;
    axFloat GetGain() const;

	void SetFreqEnvelopePtr(axFloat* ptr);
	void SetFreqEnvelopeAmountPtr(axFloat* ptr);

	axFloat* lfo[2];
	axFloat* lfoAmnt[2];
	axFloat* env[2];
	axFloat* envAmnt[2];

	//private:
	axFloat freq, q, gain;
	void Compute_Variables(axFloat freq, axFloat q);
	void Biquad_Coeff(int fType);
	axFloat b0, b1, b2, a0, a1, a2;
	axFloat x1, x2, y1, y2;
	axFloat rx1, rx2, ry1, ry2;
	axFloat c, w0, alpha;
	int filterType;
	bool init;
	long sr;
};


#endif // __AX_AUDIO_FILTER__