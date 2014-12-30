//
//  axAudioUtils.h
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-14.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __MidiSequencer__axAudioUtils__
#define __MidiSequencer__axAudioUtils__

#define axINVTWOPI (0.159154943091895)


struct axOuputStereo
{
    float left, right;
};


namespace axAudioUtils
{
    int SecondeToSample(const double& seconde,
                        const int& samplingRate);
}


#endif /* defined(__MidiSequencer__axAudioUtils__) */
