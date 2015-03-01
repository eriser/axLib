#include "axAudioUtils.h"

int axAudio::SecondeToSample(const double& seconde,
                                  const int& samplingRate)
{
    return seconde * samplingRate;
}