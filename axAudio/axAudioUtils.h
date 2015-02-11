//
//  axAudioUtils.h
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-14.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __MidiSequencer__axAudioUtils__
#define __MidiSequencer__axAudioUtils__

#ifndef axINVTWOPI
#define axINVTWOPI (0.159154943091895)
#endif

#ifndef ST_RATIO
#define ST_RATIO (1.0594630943592952)
#endif

struct axOuputStereo
{
    float left, right;
};

//template<typename T>
//typedef T axStereoSample[2];
//
//typedef axStereoSample<double> axDoubleStereoSample;
//typedef axStereoSample<float> axFloatStereoSample;

namespace axAudioUtils
{
    int SecondeToSample(const double& seconde,
                        const int& samplingRate);
    
    template <typename T>
    T LineairInterpole(const T& y1,
                       const T& y2,
                       const T& mu)
    {
        return y1 + mu * (y2 - y1);
    }
    
    template<typename T>
    T Clamp(T d, T min, T max)
    {
        const T t = d < min ? min : d;
        return t > max ? max : t;
    }
}

namespace axAudioConstant
{
    const double C0 = 8.175798915643709;
    const double SemiToneRatio = 1.0594630943592952;
}


#endif /* defined(__MidiSequencer__axAudioUtils__) */
