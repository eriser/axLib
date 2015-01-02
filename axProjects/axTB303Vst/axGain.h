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

#include "axAudioWaveTable.h"
#include "axAudioFilter.h"
#include "axAudioEnvelope.h"
#include "axVst.h"

class AGain : public axVst
{
public:
    AGain(audioMasterCallback audioMaster);
    
//    void getParameterName (VstInt32 index, char* label);
//    
//    void getParameterDisplay (VstInt32 index, char* text);
//    
//    void getParameterLabel (VstInt32 index, char* label);
    
    VstInt32 processEvents(VstEvents* ev);
    
    virtual void processReplacing(float** inputs,
                                  float** outputs,
                                  VstInt32 sampleFrames);
    
    virtual void processDoubleReplacing(double** inputs,
                                        double** outputs,
                                        VstInt32 sampleFrames);
private:
    float fGain;
    float _filterFreq;
    
    axAudioWaveTable* _waveTable;
    axAudioFilter* _filter;
    axAudioEnvelope* _env;
    
    double c0;
};

#endif