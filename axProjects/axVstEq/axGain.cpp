//------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : again.cpp
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//------------------------------------------------------------------------------
#include "axGain.h"
#include "axLib.h"
#include <iostream>
#include <axAudio/axAudioBuffer.h>
#include "FilterParametric.h"

//******************************************************************************
// AGain.
//******************************************************************************
AudioEqualizer::AudioEqualizer(audioMasterCallback audioMaster):
axVst(audioMaster, 2)
{
//    _eqMutex.lock();
    AddParameter(axParameterInfo("Gain", "dB", 1.0));
    AddParameter(axParameterInfo("Filter", "Hz", 5000.0));
    
    // Default program name
    vst_strncpy(programName, "Gain", kVstMaxProgNameLen);
    
    axEventManager* evtManager = axEventManager::GetInstance();
    
    evtManager->AddConnection(10000000 + GetPluginId(),
                              0,
                              GetOnVstParameterValueChange());
    
//    _eqMutex.unlock();
}

void AudioEqualizer::open()
{
//    _eqMutex.lock();

    
//    _eqMutex.unlock();
}

//bool AudioEqualizer::getProductString (char* text)
//{
//    vst_strncpy(text, "Gain", kVstMaxProductStrLen);
//    return true;
//}

//void AudioEqualizer::processReplacing(float** inputs,
//                             float** outputs,
//                             VstInt32 sampleFrames)
//{
////    _eqMutex.lock();
//    (void)inputs;
//    
//    float* out1 = outputs[0];
//    float* out2 = outputs[1];
//    
////    _eqMutex.unlock();
//}
//
//void AudioEqualizer::processDoubleReplacing(double** inputs,
//                                   double** outputs,
//                                   VstInt32 sampleFrames)
//{
////    _eqMutex.lock();
//    (void)inputs;
//    
//    double* out1 = outputs[0];
//    double* out2 = outputs[1];
//
////    _eqMutex.unlock();
//}

//******************************************************************************
// Vst entry point.
//******************************************************************************
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    axEventManager::GetInstance();
    axApp* app = axApp::CreateApp(axSize(300, 130));
    
    app->AddMainEntry([]()
                      {
                          new FilterParametric(nullptr, axRect(0, 0, 300, 130));
                      });

    return new AudioEqualizer(audioMaster);
}
