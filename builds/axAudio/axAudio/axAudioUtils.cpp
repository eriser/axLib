#include "axAudioUtils.h"

int axAudioUtils::SecondeToSample(const double& seconde,
                                  const int& samplingRate)
{
    return seconde * samplingRate;
}