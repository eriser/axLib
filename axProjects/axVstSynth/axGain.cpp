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
//#include "TestWindow.h"
#include "axCocoaInterfaceMac.h"

//AGain* GlobalAGain = nullptr;

#ifndef ST_RATIO
#define ST_RATIO (1.0594630943592952)
#endif



class axVstParameterMsg : public axMsg
{
public:
    axVstParameterMsg(const double& value):
    _value(value)
    {
    }
    
    double GetValue() const
    {
        return _value;
    }
    
    axMsg* GetCopy()
    {
        return new axVstParameterMsg(*this);
    }
    
private:
    double _value;
};

class axTestPanel : public axPanel
{
public:
    axTestPanel(axWindow* parent, const axRect& rect):
    axPanel(parent, rect)
    {
        
//        std::cout << "axTestPanel ID : " << GetId() << std::endl;
        
        axButton* btn = new axButton(this, axRect(100, 30, 50, 50),
                                     axButtonEvents(GetOnButtonClick()),
                                     axSTANDARD_BUTTON);
        
        axKnobInfo knob_info(axColor(0.3, 0.3, 0.3, 0.0),
                             axColor(0.5, 0.5, 0.5, 0.0),
                             axColor(0.8, 0.8, 0.8, 0.0),
                             128,
                             axSize(46, 46),
                             std::string("/Users/alexarse/Project/axLib/axExamples/axDemo/knob_dark.png"),
                             std::string("/Users/alexarse/Project/axLib/axExamples/axDemo/knob_dark.png"));
        
        _gainKnob = new axKnob(this, axRect(40, 40, 46, 46),
                               axKnobEvents(GetOnKnobGain()), knob_info);
        
        axEventManager* evtManager = axEventManager::GetInstance();
        
        evtManager->AddConnection(10000000,
                                  14,
                                  GetOnVstParameterValueChange());
        
//        std::cout << axApp::GetInstance()->GetAppDirectory() << std::endl;
    }
    
    axEVENT_ACCESSOR(axButtonMsg, OnButtonClick);
    axEVENT_ACCESSOR(axKnobMsg, OnKnobGain);
    
    axEVENT_ACCESSOR(axVstParameterMsg, OnVstParameterValueChange);
    
private:
    axKnob* _gainKnob;
    
    void OnButtonClick(const axButtonMsg& msg)
    {
//        std::cout << "Button click." << std::endl;
        axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>
                                   (axApp::GetInstance()->GetCore());
        axVstCoreData* vstCoreData = vstCoreMac->GetVstCoreData();
        
        vstCoreData->effect->setParameter (0, 0.5);
//        GlobalAGain->setParameter (0, 0.5);
    }
    
    void OnVstParameterValueChange(const axVstParameterMsg& msg)
    {
//        std::cout << "OnVstParameterValueChange." << std::endl;
        
        if(_gainKnob->GetValue() != msg.GetValue())
        {
            _gainKnob->SetValue(msg.GetValue(), false);
        }
        
        //std::cout << "Button click." << std::endl;
        //GlobalAGain->setParameter (0, 0.5);
    }
    
    void OnKnobGain(const axKnobMsg& msg)
    {
        axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>
        (axApp::GetInstance()->GetCore());
        axVstCoreData* vstCoreData = vstCoreMac->GetVstCoreData();
        
        
        
        if(vstCoreData->effect != nullptr)
        {
            AGain* aGain = static_cast<AGain*>(vstCoreData->effect);
            aGain->SetParameterFromGUI(0, msg.GetValue());
        }
        
        
////        std::cout << "knob click." << std::endl;
//        if(GlobalAGain != nullptr)
//        {
////            std::cout << "GlobalAGain exist." << std::endl;
////            GlobalAGain->setParameter(0, msg.GetValue());
//            GlobalAGain->SetParameterFromGUI(0, msg.GetValue());
////            GlobalAGain->updateDisplay();
//        }
    }
    
    void OnPaint()
    {
//        std::cout << "OnPaint" << std::endl;
        axGC* gc = GetGC();
        axRect rect = axRect(axPoint(0, 0), GetRect().size);
        
        gc->SetColor(axColor(0.0, 1.0, 0.0));
        gc->DrawRectangle(rect);
    }
};

bool axGainGUI::_pluginHasBeenOpenOnHostInit = false;
bool axGainGUI::_isFirstTimeVstGUIOpen = true;

axGainGUI::axGainGUI(AudioEffect* effect):
AEffEditor(effect)
{
//    std::cout << "axGainGUI constructor." << std::endl;
    
    // Notify effect that "this is the editor".
    effect->setEditor(this);
    systemWindow = nullptr;
    
    _rect.left = 0;
    _rect.right = 200;
    _rect.top = 0;
    _rect.bottom = 200;
    
    
}

// Vst will crash if this is not implemented.
bool axGainGUI::getRect(ERect** rect)
{
//    std::cout << "axGainGUI getRect." << std::endl;
     *rect = &_rect;
    return true;
}

bool axGainGUI::open(void* ptr)
{
//    std::cout << "axGainGUI::open(void* ptr)" << std::endl;
    
    if(_isFirstTimeVstGUIOpen == true)
    {
        _isFirstTimeVstGUIOpen = false;
        
        if(getEffect()->getProgram() == 1)
        {
            _pluginHasBeenOpenOnHostInit = true;
        }
    }

//    int pluginIndex = getEffect()->getProgram() - 1;
    int pluginIndex = getEffect()->getProgram();
    
    if(_pluginHasBeenOpenOnHostInit == true)
    {
        pluginIndex = pluginIndex - 1;
    }
    
    
    axApp* app = axApp::GetInstance();
    axVstCoreMac* vstCoreMac = static_cast<axVstCoreMac*>(app->GetCore());
    
    // First instance of vst plugin.
    if(vstCoreMac->GetCurrentManagerIndex() == 0 &&
       vstCoreMac->GetNumberOfManager() == 1 &&
       vstCoreMac->GetVstCoreData()->appDelegate == nullptr)
    {
//        std::cout << "First vst instance" << std::endl;

        vstCoreMac->SetCurrentManagerIndex(0);
        axVstCoreData* coreData = vstCoreMac->GetVstCoreData();
        CreateNSWindowFromApp(ptr, systemWindow, coreData->appDelegate);
        coreData->effect = effect;
        vstCoreMac->InitGL();
        vstCoreMac->UpdateAll();
    }
    
    // Create new instance of vst plugin.
    else if(pluginIndex != vstCoreMac->GetCurrentManagerIndex() &&
       pluginIndex >= vstCoreMac->GetNumberOfManager())
    {
//        std::cout << "Second vst instance" << std::endl;
        
        vstCoreMac->InitManagers();
        vstCoreMac->SetCurrentManagerIndex(pluginIndex);
        axVstCoreData* coreData = vstCoreMac->GetVstCoreData();
        CreateNSWindowFromApp(ptr, systemWindow, coreData->appDelegate);
        coreData->effect = effect;
        vstCoreMac->InitGL();
        vstCoreMac->UpdateAll();
//        vstCoreMac->Init(axSize(0, 0));
//        vstCoreMac->SetCurrentManagerIndex(pluginIndex);
//        vstCoreMac->ReInitApp();
    }
    
    // Plugin instance already exist.
    else
    {
//        std::cout << "Reattach vst instance" << std::endl;
        vstCoreMac->SetCurrentManagerIndex(pluginIndex);
        CreateNSWindowFromApp(ptr, systemWindow,
                              vstCoreMac->GetVstCoreData()->appDelegate);
        vstCoreMac->ReInitApp();
        vstCoreMac->InitGL();
        vstCoreMac->UpdateAll();
//        axVstCoreData* coreData = vstCoreMac->GetVstCoreData();
    }
    
    
//    
//    axVstCoreData* coreData = vstCoreMac->GetVstCoreData();
//    CreateNSWindowFromApp(ptr, systemWindow, coreData->appDelegate);
//    vstCoreMac->InitGL();
//    vstCoreMac->ReInitApp();
//    
//    vstCoreMac->UpdateAll();
    systemWindow = ptr;
    
    return AEffEditor::open(ptr);
}

void axGainGUI::draw(ERect* rect)
{
//    std::cout << "axGainGUI::draw(ERect* rect)" << std::endl;
}

//void axGainGUI::MyOpen(void*& win)
//{
//    std::cout << "Myopen" << std::endl;
//    CreateNSWindow(systemWindow, win);
//}

int AGain::pluginIdCounter = 0;

//------------------------------------------------------------------------------
AGain::AGain (audioMasterCallback audioMaster) :
// Heritage.
AudioEffectX(audioMaster, 1, 2)	// 1 program, 1 parameter only
{
    setNumInputs (2);		// stereo in
    setNumOutputs (2);		// stereo out
    setUniqueID ('Gain');	// identify
    canProcessReplacing ();	// supports replacing output
    canDoubleReplacing ();	// supports double precision processing
    
    //--------------------------------------
//    cEffect.flags |= effFlagsHasEditor;
//    cEffect.version = 1;
    
    fGain = 1.f;			// default to 0 dB
    vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
    
//    std::cout << "AGain::AGain (Constructor)." << std::endl;
    char* dir = (char*)getDirectory();
    
//    std::cout << "Dir : " << dir << std::endl;
    
    editor = new axGainGUI(this);
    
//    GlobalAGain = this;
    
    _pluginId = pluginIdCounter++;
    
    isSynth(true);
    
    
    _filterFreq = 5000.0;
    
    
    //-----------------------------
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

//-------------------------------------------------------------------------------------------------------
AGain::~AGain ()
{
    // nothing to do here
}

void AGain::open()
{
//    std::cout << "AGain::open ----- >  " << _pluginId << std::endl;
//    std::cout << "AGAIN : uID" << getCurrentUniqueId() << std::endl;
//    std::cout << "AGAIN : program" << getProgram() << std::endl;
}

void AGain::close()
{
//    std::cout << "AGain::close ----- >  " << _pluginId << std::endl;
}

//-------------------------------------------------------------------------------------------------------
void AGain::setProgramName (char* name)
{
    vst_strncpy(programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void AGain::getProgramName (char* name)
{
    vst_strncpy(name, programName, kVstMaxProgNameLen);
    
}

VstInt32 AGain::getProgram()
{
//     std::cout << "AGain::getProgram()" << std::endl;
    return _pluginId;
}

//-----------------------------------------------------------------------------------------
void AGain::SetParameterFromGUI(VstInt32 index, float value)
{
//    std::cout << "AGain::SetParameterFromGUI" << std::endl;
    fGain = value;
    updateDisplay();
}

void AGain::setParameter (VstInt32 index, float value)
{
//    std::cout << "----------------------------AGain::setParameter" << std::endl;
    
    if(index == 0)
    {
        fGain = value;
    }
    else if(index == 1)
    {
//        std::cout << "filter param in : " << value << std::endl;
        axRange<float> filterRange(30.0, 5000.0);
        _filterFreq = filterRange.GetValueFromZeroToOne(value);
//        std::cout << "filter freq : " << _filterFreq << std::endl;
        _filter->SetFreq(_filterFreq);
    }
    
//    std::cout << index << std::endl;
    
    
    
    
//    axEventManager* evtManager = axEventManager::GetInstance();
//    
//    if(evtManager != nullptr)
//    {
////        std::cout << "evtManager exist." << std::endl;
//        evtManager->PushEvent(10000000, 14,
//                              new axVstParameterMsg(value));
//    }
}

//-----------------------------------------------------------------------------------------
float AGain::getParameter (VstInt32 index)
{
    float value = 0.0;
    if(index == 0)
    {
        value = fGain;
    }
    else if(index == 1)
    {
        value = _filterFreq;
    }
    
    return value;
}

//-----------------------------------------------------------------------------------------
void AGain::getParameterName (VstInt32 index, char* label)
{
    if(index == 0)
    {
        vst_strncpy(label, "Gain", kVstMaxParamStrLen);
    }
    else if(index == 1)
    {
        vst_strncpy(label, "Filter Freq", kVstMaxParamStrLen);
    }
    
}

//-----------------------------------------------------------------------------------------
void AGain::getParameterDisplay (VstInt32 index, char* text)
{
    if(index == 0)
    {
        dB2string(fGain, text, kVstMaxParamStrLen);
    }
    else if(index == 1)
    {
        float2string(_filterFreq, text, kVstMaxParamStrLen);
    }
    
}

//-----------------------------------------------------------------------------------------
void AGain::getParameterLabel (VstInt32 index, char* label)
{
    if(index == 0)
    {
        vst_strncpy(label, "dB", kVstMaxParamStrLen);
    }
    else if(index == 1)
    {
        vst_strncpy(label, "Hz", kVstMaxParamStrLen);
    }
}

//------------------------------------------------------------------------
bool AGain::getEffectName (char* name)
{
    vst_strncpy (name, "Gain", kVstMaxEffectNameLen);
    
    return true;
}

//------------------------------------------------------------------------
bool AGain::getProductString (char* text)
{
    vst_strncpy(text, "Gain", kVstMaxProductStrLen);
    return true;
}

//------------------------------------------------------------------------
bool AGain::getVendorString (char* text)
{
    vst_strncpy(text, "Steinberg Media Technologies", kVstMaxVendorStrLen);
    return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 AGain::getVendorVersion ()
{
    return 1000;
}

VstPlugCategory AGain::getPlugCategory()
{
    return kPlugCategSynth;
}

VstInt32 AGain::canDo(char *text)
{
    // TODO: Fill in according to your plugin's capabilities
    if(!strcmp(text, "receiveVstEvents"))
    {
        return 1;
    }
    else if(!strcmp(text, "receiveVstMidiEvent"))
    {
        return 1;
    }
    else if(!strcmp(text, "midiProgramNames"))
    {
        return 1;
    }
    
    // -1 => explicitly can't do; 0 => don't know
    return 0;
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
                
//                n += oct + semi;
                double freq = c0 * pow(ST_RATIO, midiNote);
                _waveTable->SetFreq(freq);

                
                _env->TriggerNote();
                
                
                
//                std::cout << "On : " << (int)midiData[1] << " " <<  (int)midiData[2] << std::endl;

            }
            // Note off.
            else if(status == 0x80)
            {
//                std::cout << "Off : " << (int)midiData[1] << " " <<  (int)midiData[2] << std::endl;

            }
        }
    }
    
    return 1;
}

//-----------------------------------------------------------------------------------------
void AGain::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
    
//    float* in1  =  inputs[0];
//    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
    
    
    while (--sampleFrames >= 0)
    {
        float value = 0.0;
        _waveTable->ProcessSample(&value);
        
        t_out input_filter(value, value);
        t_out filter_processed = _filter->ProcessStereo(input_filter);
        
        float env = _env->Process();
        
//        t_out input_filter(*(output), *(output+1));
//        t_out filter_processed = _filter->ProcessStereo(input_filter);
        
        *out1 = filter_processed.l * env * fGain;
        *out2 = filter_processed.r * env * fGain;
        
        out1++;
        out2++;
        
//        (*out1++) = (*in1++) * fGain;
//        (*out2++) = (*in2++) * fGain;
    }
}

//-----------------------------------------------------------------------------------------
void AGain::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
//    double* in1  =  inputs[0];
//    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
    double dGain = fGain;
    
    while (--sampleFrames >= 0)
    {
        double value = 0.0;
        _waveTable->ProcessSample(&value);
        
        t_out input_filter(value, value);
        t_out filter_processed = _filter->ProcessStereo(input_filter);
        
        double env = _env->Process();
        
        *out1 = filter_processed.l * env * fGain;
        *out2 = filter_processed.r * env * fGain;
        
        out1++;
        out2++;
        
//        (*out1++) = (*in1++) * dGain;
//        (*out2++) = (*in2++) * dGain;
    }
}

VstIntPtr AGain::dispatcher(VstInt32 opCode,
                            VstInt32 index,
                            VstIntPtr value,
                            void* ptr, float opt)
{
    if(opCode == effEditOpen)
    {
//        std::cout << "AGain::dispatcher : effEditOpen." << std::endl;
//        editor->open(ptr);
    }
    else if(opCode == effEditClose)
    {
//        std::cout << "AGain::dispatcher : effEditClose." << std::endl;
    }
    else if(opCode == effBeginLoadProgram)
    {
//        std::cout << "AGain::dispatcher : effBeginLoadProgram." << std::endl;
    }
    else if(opCode == effClose)
    {
//        std::cout << "AGain::dispatcher : effClose." << std::endl;
    }
    
    return AudioEffectX::dispatcher(opCode, index, value, ptr, opt);
}

//------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
//    if(axApp::MainInstance == nullptr)
//    {
//        std::cout << "----------------axApp::MainInstance is null" << std::endl;
//    }
//    else
//    {
//        std::cout << "----------------axApp::MainInstance exist" << std::endl;
//    }
//    
//    std::cout << "***********************createEffectInstance" << std::endl;
    axEventManager::GetInstance();
    axApp* app = axApp::CreateApp();

    return new AGain(audioMaster);
}

void axMain::MainEntryPoint(axApp* app)
{
    axTestPanel* panel = new axTestPanel(nullptr, axRect(0, 0, 200, 200));
}
