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


namespace axAudioUtils
{
    int SecondeToSample(const double& seconde,
                        const int& samplingRate);
    
//    double c5 = 220.0 * pow(ST_RATIO, 3);
//    c0 = c5 * pow(0.5, 5);
}

namespace axAudioConstant
{
    const double C0 = 8.175798915643709;
    const double SemiToneRatio = 1.0594630943592952;
}


#endif /* defined(__MidiSequencer__axAudioUtils__) */
