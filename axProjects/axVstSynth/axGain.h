//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : again.h
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#ifndef __again__
#define __again__

#include "axLib.h"

#include "axAudioWaveTable.h"
#include "axAudioFilter.h"
#include "axAudioEnvelope.h"
#include "axVst.h"

class AGain : public axVst
{
public:
    AGain(audioMasterCallback audioMaster);

    VstInt32 processEvents(VstEvents* ev);
    
    virtual void processReplacing(float** inputs,
                                  float** outputs,
                                  VstInt32 sampleFrames);
    
    virtual void processDoubleReplacing(double** inputs,
                                        double** outputs,
                                        VstInt32 sampleFrames);
    
    axEVENT_ACCESSOR(axVstParameterMsg, OnVstParameterValueChange);
    
private:
//    float fGain;
//    float _filterFreq;
    
    axAudioWaveTable* _waveTable;
    axAudioFilter* _filter;
    axAudioEnvelope* _env;
    
    double c0;
    
    
    void OnVstParameterValueChange(const axVstParameterMsg& msg)
    {
        if(msg.GetIndex() == 0)
        {

        }
        else if(msg.GetIndex() == 1)
        {
            std::cout << "Filter value " << msg.GetValue() << std::endl;
            axRange<float> filterRange(50.0, 5000.0);
            _filter->SetFreq(filterRange.GetValueFromZeroToOne(msg.GetValue()));
        }
    }
};

#endif