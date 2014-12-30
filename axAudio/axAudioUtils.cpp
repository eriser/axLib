//
//  axAudioUtils.cpp
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-14.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "axAudioUtils.h"


int axAudioUtils::SecondeToSample(const double& seconde,
                                  const int& samplingRate)
{
    return seconde * samplingRate;
}