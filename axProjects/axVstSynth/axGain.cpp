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
#include "axAudioBuffer.h"

#ifndef ST_RATIO
#define ST_RATIO (1.0594630943592952)
#endif

//******************************************************************************
// axTestPanel.
//******************************************************************************
class axTestPanel : public axPanel
{
public:
    axTestPanel(axWindow* parent, const axRect& rect):
    axPanel(parent, rect)
    {
//        axButton* btn = new axButton(this, axRect(100, 30, 50, 50),
//                                     axButtonEvents(GetOnButtonClick()),
//                                     axSTANDARD_BUTTON);
        
        axKnobInfo knob_info(axColor(0.3, 0.3, 0.3, 0.0),
                             axColor(0.5, 0.5, 0.5, 0.0),
                             axColor(0.8, 0.8, 0.8, 0.0),
                             128,
                             axSize(50, 50),
                             std::string("/Users/alexarse/Project/axLib/ressources/plastic_knob_50x50.png"),
                             std::string("/Users/alexarse/Project/axLib/ressources/plastic_knob_50x50.png"));
        
        _gainKnob = new axKnob(this, axRect(40, 40, 50, 50),
                               axKnobEvents(GetOnKnobGain()), knob_info);
        
        _gainKnob->SetValue(1.0);
        
        axVstCore* vstCore = static_cast<axVstCore*>
        (axApp::GetInstance()->GetCore());
        
        axVstCoreData* vstCoreData = vstCore->GetVstCoreData();
        
        if(vstCoreData->effect != nullptr)
        {
            axEventManager* evtManager = axEventManager::GetInstance();
            
            evtManager->AddConnection(10000000 + vstCoreData->effect->getProgram(),
                                      0,
                                      GetOnVstParameterValueChange());
        }
    }
    
    axEVENT_ACCESSOR(axButtonMsg, OnButtonClick);
    axEVENT_ACCESSOR(axKnobMsg, OnKnobGain);
    
    axEVENT_ACCESSOR(axVstParameterMsg, OnVstParameterValueChange);
    
private:
    axKnob* _gainKnob;
    
    void OnButtonClick(const axButtonMsg& msg)
    {
        axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>
        (axApp::GetInstance()->GetCore());
        axVstCoreData* vstCoreData = vstCoreMac->GetVstCoreData();
        
        vstCoreData->effect->setParameter (0, 0.5);
    }
    
    void OnVstParameterValueChange(const axVstParameterMsg& msg)
    {
        if(msg.GetIndex() == 0)
        {
            if(_gainKnob->GetValue() != msg.GetValue())
            {
                _gainKnob->SetValue(msg.GetValue(), false);
            }
        }
        else if(msg.GetIndex() == 1)
        {
//            if(_gainKnob->GetValue() != msg.GetValue())
//            {
//                _gainKnob->SetValue(msg.GetValue(), false);
//            }
        }
    }
    
    void OnKnobGain(const axKnobMsg& msg)
    {
        axVstCore* vstCore = static_cast<axVstCore*>
                             (axApp::GetInstance()->GetCore());
        
        axVstCoreData* vstCoreData = vstCore->GetVstCoreData();
        
        if(vstCoreData->effect != nullptr)
        {
            AGain* aGain = static_cast<AGain*>(vstCoreData->effect);
            aGain->SetParameterFromGUI(0, msg.GetValue());
        }
    }
    
    void OnPaint()
    {
        axGC* gc = GetGC();
        axRect rect = axRect(axPoint(0, 0), GetRect().size);
        
        gc->SetColor(axColor(0.4, 0.4, 0.4));
        gc->DrawRectangle(rect);
        
        gc->SetColor(axColor(0.0, 0.0, 0.0));
        gc->DrawRectangleContour(axRect(1, 1, rect.size.x - 1, rect.size.y - 1));
    }
};


PolyPhonicChannel::PolyPhonicChannel(axAudioBuffer* waveTableAudioBuffer)
{
    std::cout << "Init poly voice." << std::endl;
    
    /// @todo Change this.
    _processBuffer = new double*[2];
    _processBuffer[0] = new double[8192];
    _processBuffer[1] = new double[8192];
    for(int i = 0; i < 8192; i++)
    {
        _processBuffer[0][i] = 0.0;
        _processBuffer[1][i] = 0.0;
    }
    
//    std::string sndFile("/Users/alexarse/Project/axLib/axProjects/axVstSynth/build/UninstalledProducts/Piano.wav");
//    axAudioBuffer* audioBuffer = new axAudioBuffer(sndFile);
    
//    axApp* app = axApp::GetInstance();
////    app->GetResourceManager()->Lock();
//    std::cout << "Init poly voice 2." << std::endl;
//    axAudioBuffer* buf = app->GetResourceManager()->GetResource("wf1");
//    std::cout << "Init poly voice 3." << std::endl;
//    _waveTable = new axAudioWaveTable(buf);
//    std::cout << "Init poly voice 4." << std::endl;
//    app->GetResourceManager()->Unlock();

    _waveTable = new axAudioWaveTable();
    _waveTable->SetWaveformType(axAudioWaveTable::axWAVE_TYPE_SQUARE);
    
//    _waveTable = new axAudioWaveTable(waveTableAudioBuffer);
    
    
    _filter = new axAudioFilter();
    _filter->SetFreq(5000.0);
    _filter->SetQ(0.707);
    _filter->SetGain(1.0);
    
    _env = new axAudioEnvelope();
    _env->SetAttack(0.001);
    _env->SetDecay(0.8);
}

double** PolyPhonicChannel::GetProcessedBuffers()
{
    return _processBuffer;
}

void PolyPhonicChannel::TriggerNote(const double& freq)
{
    _waveTable->SetFreq(freq);
    _env->TriggerNote();
}

void PolyPhonicChannel::SetFilterFreq(const double& freq)
{
    _filter->SetFreq(freq);
}

void PolyPhonicChannel::ProcessChannel(VstInt32 sampleFrames)
{
    double* out1 = _processBuffer[0];
    double* out2 = _processBuffer[1];
    
    while (--sampleFrames >= 0)
    {
        double value = 0.0;
        _waveTable->ProcessSample(&value);
        
        t_out input_filter(value, value);
        t_out filter_processed = _filter->ProcessStereo(input_filter);
        
        double env = _env->Process();
        
        *out1++ = filter_processed.l * env;
        *out2++ = filter_processed.r * env;
    }
}


//******************************************************************************
// AGain.
//******************************************************************************
AGain::AGain(audioMasterCallback audioMaster):
axVst(audioMaster, 2)
{
//    std::cout << "AGain constructor." << std::endl;
    
    AddParameter(axParameterInfo("Gain", "dB", 1.0));
    AddParameter(axParameterInfo("Filter", "Hz", 5000.0));
    
    // Default program name
    vst_strncpy(programName, "axTB303", kVstMaxProgNameLen);
    
//    std::string sndFile("/Users/alexarse/Project/axLib/axProjects/axVstSynth/build/UninstalledProducts/Piano.wav");
//    _waveTableAudioBuffer = new axAudioBuffer(sndFile);
    
//    axApp* app = axApp::GetInstance();
//    axAudioBuffer* buf = app->GetResourceManager()->GetResource("wf1");
//
//    std::cout << "Buf size : " << buf->GetBufferInfo().frames << std::endl;
    
    
    
//    _polyChannels.resize(10);
//    for(int i = 0; i < 10; i++)
//    {
////        _polyChannels[i] = new PolyPhonicChannel(_waveTableAudioBuffer);
//        _polyChannels[i] = new PolyPhonicChannel(nullptr);
//    }
//    
//    _polyChannelIndex = 0;
    

    
    
//    axAudioBuffer* audioBuffer = new axAudioBuffer(sndFile);
    
    
    
//    _waveTable = new axAudioWaveTable(audioBuffer);
//    
//    _filter = new axAudioFilter();
//    _filter->SetFreq(_parameters[1].value);
//    _filter->SetQ(0.707);
//    _filter->SetGain(1.0);
//    
//    _env = new axAudioEnvelope();
//    _env->SetAttack(0.001);
//    _env->SetDecay(0.8);
    
    double c5 = 220.0 * pow(ST_RATIO, 3);
    c0 = c5 * pow(0.5, 5);
    

    axEventManager* evtManager = axEventManager::GetInstance();
    
    evtManager->AddConnection(10000000 + getProgram(),
                              0,
                              GetOnVstParameterValueChange());
}

void AGain::open()
{
    std::string sndFile("/Users/alexarse/Project/axLib/axProjects/axVstSynth/build/UninstalledProducts/Piano.wav");
    _waveTableAudioBuffer = new axAudioBuffer(sndFile);
    
    _polyChannels.resize(10);
    for(int i = 0; i < 10; i++)
    {
        //        _polyChannels[i] = new PolyPhonicChannel(_waveTableAudioBuffer);
        _polyChannels[i] = new PolyPhonicChannel(_waveTableAudioBuffer);
    }
    
    _polyChannelIndex = 0;
}

bool AGain::getProductString (char* text)
{
    vst_strncpy(text, "Gain", kVstMaxProductStrLen);
    return true;
}

void AGain::OnVstMidiNoteOnEvent(const axVstMidiNoteMsg& msg)
{
    
    int midiNote = (int)msg.GetNote();
    double freq = c0 * pow(ST_RATIO, midiNote);
    
    
    std::cout << "AGain( id : " << _pluginId << " ) " << "::OnVstMidiNoteOnEvent " << msg.GetNote() << std::endl;
    _polyChannels[_polyChannelIndex]->TriggerNote(freq);
    
//    _waveTable->SetFreq(freq);
//    _env->TriggerNote();
    
    if(_polyChannelIndex + 1 == 10)
    {
        _polyChannelIndex = 0;
    }
    else
    {
        _polyChannelIndex++;
    }
}

void AGain::OnVstMidiNoteOffEvent(const axVstMidiNoteMsg& msg)
{
}

//VstInt32 AGain::processEvents(VstEvents* ev)
//{
//    int numEvent = ev->numEvents;
//    
//    for(int i = 0; i < numEvent; i++)
//    {
//        if(ev->events[i]->type == kVstMidiType)
//        {
//            VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
//            char* midiData = event->midiData;
//            
//            VstInt32 status = midiData[0] & 0xf0;   // ignoring channel
//            
//            // Note on.
//            if(status == 0x90)
//            {
//                int midiNote = (int)midiData[1];
//                double freq = c0 * pow(ST_RATIO, midiNote);
//                
//                _polyChannels[_polyChannelIndex]->TriggerNote(freq);
//            
//                _waveTable->SetFreq(freq);
//                _env->TriggerNote();
//                
//                if(_polyChannelIndex + 1 == 10)
//                {
//                    _polyChannelIndex = 0;
//                }
//                else
//                {
//                    _polyChannelIndex++;
//                }
//            }
//            // Note off.
//            else if(status == 0x80)
//            {
//                
//            }
//        }
//    }
//    
//    return 1;
//}

void AGain::processReplacing(float** inputs,
                             float** outputs,
                             VstInt32 sampleFrames)
{
    (void)inputs;
    
    float* out1 = outputs[0];
    float* out2 = outputs[1];
    double gain = 1.0f; //_parameters[0].value;
    
    // Process all voices.
    for(int i = 0; i < 10; i++)
    {
        _polyChannels[i]->ProcessChannel(sampleFrames);
    }
    
    // Get poly channel buffers.
    double* voices[10];
    for(int i = 0; i < 10; i++)
    {
        voices[i] = _polyChannels[i]->GetProcessedBuffers()[0];
    }
    
    // Output process loop..
    for(int i = 0; i < sampleFrames; i++)
    {
        float v = 0.0;
        for(int n = 0; n < 10; n++)
        {
            v = v + voices[n][i];
        }
        
//        if(v != 0.0)
//        {
//            std::cout << "V : " << v << std::endl;
//
//        }
        
        *out1++ = v * gain;
        *out2++ = v * gain;
    }
}

void AGain::processDoubleReplacing(double** inputs,
                                   double** outputs,
                                   VstInt32 sampleFrames)
{
    (void)inputs;
    
    double* out1 = outputs[0];
    double* out2 = outputs[1];
    double gain = 1.0; //_parameters[0].value;
    
    // Process all voices.
    for(int i = 0; i < 10; i++)
    {
        _polyChannels[i]->ProcessChannel(sampleFrames);
    }
    
    // Get poly channel buffers.
    double* voices[10];
    for(int i = 0; i < 10; i++)
    {
        voices[i] = _polyChannels[i]->GetProcessedBuffers()[0];
    }
    
    // Output process loop..
    for(int i = 0; i < sampleFrames; i++)
    {
        double v = 0.0;
        for(int n = 0; n < 10; n++)
        {
            v = v + voices[n][i];
        }
        
        *out1++ = v * gain;
        *out2++ = v * gain;
    }
}

//******************************************************************************
// Vst entry point.
//******************************************************************************
AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    std::cout << "AudioEffect* createEffectInstance." << std::endl;
    
    axEventManager::GetInstance();
    axApp* app = axApp::CreateApp(axSize(300, 130));

//    app->GetResourceManager()->Lock();
//    if(app->GetResourceManager()->GetResource("wf1").is_null())
//    {
//        std::string sndFile("/Users/alexarse/Project/axLib/axProjects/axVstSynth/build/UninstalledProducts/Piano.wav");
//        axAudioBuffer* audioFile = new axAudioBuffer(sndFile, 0);
//        app->GetResourceManager()->Add(std::string("wf1"), audioFile);
//    }
//    app->GetResourceManager()->Unlock();

    return new AGain(audioMaster);
}

//******************************************************************************
// axMain entry point.
//******************************************************************************
void axMain::MainEntryPoint(axApp* app)
{
    axTestPanel* myProject = new axTestPanel(nullptr, axRect(0, 0, 300, 130));
}



