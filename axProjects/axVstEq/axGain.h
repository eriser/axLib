#ifndef __again__
#define __again__

#include "axLib.h"

#include <axAudio/axAudioWaveTable.h>
#include <axAudio/axAudioFilter.h>
#include <axAudio/axAudioEnvelope.h>
#include "axVst.h"

#include <mutex>

class AudioEqualizer : public axVst
{
public:
    AudioEqualizer(audioMasterCallback audioMaster);

    virtual void open();
//    virtual bool getProductString (char* text);
//    virtual void processReplacing(float** inputs,
//                                  float** outputs,
//                                  VstInt32 sampleFrames);
//    
//    virtual void processDoubleReplacing(double** inputs,
//                                        double** outputs,
//                                        VstInt32 sampleFrames);

private:
    axEVENT_ACCESSOR(axVstParameterMsg, OnVstParameterValueChange);
    
//    std::mutex _eqMutex;
    
    void OnVstParameterValueChange(const axVstParameterMsg& msg)
    {
        if(msg.GetIndex() == 0)
        {

        }
        else if(msg.GetIndex() == 1)
        {

        }
    }
};

#endif