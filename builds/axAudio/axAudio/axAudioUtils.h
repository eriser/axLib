#ifndef __MidiSequencer__axAudioUtils__
#define __MidiSequencer__axAudioUtils__

struct axOuputStereo
{
    float left, right;
};

namespace axAudioUtils
{
    int SecondeToSample(const double& seconde, const int& samplingRate);
                        
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
    const double InvTwoPi = 0.159154943091895;
    const double TwoPi = 6.2831853071795864;
}


#endif /* defined(__MidiSequencer__axAudioUtils__) */
