//
//  DrumUtils.h
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-06.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __DrumNBass__DrumUtils__
#define __DrumNBass__DrumUtils__

#include "axColor.h"
#include <vector>

enum ColorChoice
{
    CHOICE_RED,
    CHOICE_GREEN,
    CHOICE_BLUE,
    CHOICE_NUM
};

struct MidiNoteParams
{
    MidiNoteParams():
    velocities(3, 0.7),
    actives(3, false),
    colors(3, axColor(0.8, 0.0, 0.0))
    {
        active = false;
        velocity = 0.0;
    }
    
    bool active;
    float velocity;
    
    std::vector<bool> actives;
    std::vector<float> velocities;
    std::vector<axColor> colors;
};

#endif /* defined(__DrumNBass__DrumUtils__) */
