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
#include "axTB303Vst.h"

#ifndef ST_RATIO
#define ST_RATIO (1.0594630943592952)
#endif


AGain::AGain(audioMasterCallback audioMaster):
axVst(audioMaster, 2)
{
    AddParameter(axParameterInfo("Gain", "dB", 1.0));
    AddParameter(axParameterInfo("Filter", "dB", 5000.0));
    
//    fGain = 1.f;			// default to 0 dB
    vst_strncpy(programName, "axTB303", kVstMaxProgNameLen);	// default program name

    _filterFreq = 5000.0;
    
    _waveTable = new axAudioWaveTable();
    _waveTable->SetWaveformType(axAudioWaveTable::axWAVE_TYPE_SQUARE);
    
    _filter = new axAudioFilter();
    _filter->SetFreq(_filterFreq);
    _filter->SetQ(0.707);
    _filter->SetGain(1.0);
    
    _env = new axAudioEnvelope();
    _env->SetAttack(0.001);
    _env->SetDecay(0.8);
    
    double c5 = 220.0 * pow(ST_RATIO, 3);
    c0 = c5 * pow(0.5, 5);
}

//void AGain::getParameterName (VstInt32 index, char* label)
//{
//    if(index == 0)
//    {
//        vst_strncpy(label, "Gain", kVstMaxParamStrLen);
//    }
//    else if(index == 1)
//    {
//        vst_strncpy(label, "Filter Freq", kVstMaxParamStrLen);
//    }
//}
//
//void AGain::getParameterDisplay (VstInt32 index, char* text)
//{
//    if(index == 0)
//    {
//        dB2string(fGain, text, kVstMaxParamStrLen);
//    }
//    else if(index == 1)
//    {
//        float2string(_filterFreq, text, kVstMaxParamStrLen);
//    }
//}
//
//void AGain::getParameterLabel (VstInt32 index, char* label)
//{
//    if(index == 0)
//    {
//        vst_strncpy(label, "dB", kVstMaxParamStrLen);
//    }
//    else if(index == 1)
//    {
//        vst_strncpy(label, "Hz", kVstMaxParamStrLen);
//    }
//}

bool getProductString (char* text)
{
    vst_strncpy(text, "Gain", kVstMaxProductStrLen);
    return true;
}

VstInt32 AGain::processEvents(VstEvents* ev)
{
    int numEvent = ev->numEvents;

    for(int i = 0; i < numEvent; i++)
    {
        if(ev->events[i]->type == kVstMidiType)
        {
            VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
            char* midiData = event->midiData;

            VstInt32 status = midiData[0] & 0xf0;   // ignoring channel

            // Note on.
            if(status == 0x90)
            {
                int midiNote = (int)midiData[1];
                double freq = c0 * pow(ST_RATIO, midiNote);
                _waveTable->SetFreq(freq);
                _env->TriggerNote();
            }
            // Note off.
            else if(status == 0x80)
            {

            }
        }
    }

    return 1;
}

void AGain::processReplacing(float** inputs,
                             float** outputs,
                             VstInt32 sampleFrames)
{
    (void)inputs;
    float* out1 = outputs[0];
    float* out2 = outputs[1];


    while (--sampleFrames >= 0)
    {
        float value = 0.0;
        _waveTable->ProcessSample(&value);

        t_out input_filter(value, value);
        t_out filter_processed = _filter->ProcessStereo(input_filter);

        float env = _env->Process();

        *out1 = filter_processed.l * env * fGain;
        *out2 = filter_processed.r * env * fGain;

        out1++;
        out2++;
    }
}

void AGain::processDoubleReplacing(double** inputs,
                                   double** outputs,
                                   VstInt32 sampleFrames)
{
    (void)inputs;
    
    double* out1 = outputs[0];
    double* out2 = outputs[1];
    double gain = fGain;

    while (--sampleFrames >= 0)
    {
        double value = 0.0;
        _waveTable->ProcessSample(&value);

        t_out input_filter(value, value);
        t_out filter_processed = _filter->ProcessStereo(input_filter);

        double env = _env->Process();

        *out1 = filter_processed.l * env * gain;
        *out2 = filter_processed.r * env * gain;

        out1++;
        out2++;
    }
}

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    axEventManager::GetInstance();
    axApp* app = axApp::CreateApp();

    return new AGain(audioMaster);
}

void axMain::MainEntryPoint(axApp* app)
{
    MyProject* myProject = new MyProject(nullptr, axRect(0, 0, 856, 273));
}
