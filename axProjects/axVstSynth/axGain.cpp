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

AGain* GlobalAGain = nullptr;

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
        
        std::cout << "axTestPanel ID : " << GetId() << std::endl;
        
//        axButton* btn = new axButton(this, axRect(30, 30, 50, 50),
//                                     axButtonEvents(GetOnButtonClick()),
//                                     axSTANDARD_BUTTON);
        
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
        std::cout << "Button click." << std::endl;
        GlobalAGain->setParameter (0, 0.5);
    }
    
    void OnVstParameterValueChange(const axVstParameterMsg& msg)
    {
        std::cout << "OnVstParameterValueChange." << std::endl;
        
        if(_gainKnob->GetValue() != msg.GetValue())
        {
            _gainKnob->SetValue(msg.GetValue(), false);
        }
        
        //std::cout << "Button click." << std::endl;
        //GlobalAGain->setParameter (0, 0.5);
    }
    
    void OnKnobGain(const axKnobMsg& msg)
    {
        std::cout << "knob click." << std::endl;
        if(GlobalAGain != nullptr)
        {
            std::cout << "GlobalAGain exist." << std::endl;
//            GlobalAGain->setParameter(0, msg.GetValue());
            GlobalAGain->SetParameterFromGUI(0, msg.GetValue());
//            GlobalAGain->updateDisplay();
        }
    }
    
    void OnPaint()
    {
        std::cout << "OnPaint" << std::endl;
        axGC* gc = GetGC();
        axRect rect = axRect(axPoint(0, 0), GetRect().size);
        
        gc->SetColor(axColor(0.0, 1.0, 0.0));
        gc->DrawRectangle(rect);
    }
};

axGainGUI::axGainGUI(AudioEffect* effect):
AEffEditor(effect)
{
    std::cout << "axGainGUI constructor." << std::endl;
    
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
    std::cout << "axGainGUI getRect." << std::endl;
     *rect = &_rect;
    return true;
}

bool axGainGUI::open(void* ptr)
{
    std::cout << "axGainGUI::open." << std::endl;
    CreateNSWindow(ptr, systemWindow);
    
    systemWindow = ptr;
    
    return AEffEditor::open (ptr);
}

void axGainGUI::draw(ERect* rect)
{
    std::cout << "axGainGUI::draw(ERect* rect)" << std::endl;
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
AudioEffectX(audioMaster, 1, 1)	// 1 program, 1 parameter only
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
    
    std::cout << "AGain::AGain (Constructor)." << std::endl;
    char* dir = (char*)getDirectory();
    
    std::cout << "Dir : " << dir << std::endl;
    
    editor = new axGainGUI(this);
    
    GlobalAGain = this;
    
    _pluginId = pluginIdCounter++;
    
}

//-------------------------------------------------------------------------------------------------------
AGain::~AGain ()
{
    // nothing to do here
}

void AGain::open()
{
    std::cout << "AGain::open ----- >  " << _pluginId << std::endl;
//    std::cout << "AGAIN : uID" << getCurrentUniqueId() << std::endl;
//    std::cout << "AGAIN : program" << getProgram() << std::endl;
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

//VstInt32 AGain::getProgram()
//{
//     std::cout << "AGain::getProgram()" << std::endl;
//    return 0;
//}

//-----------------------------------------------------------------------------------------
void AGain::SetParameterFromGUI(VstInt32 index, float value)
{
    std::cout << "AGain::SetParameterFromGUI" << std::endl;
    fGain = value;
    updateDisplay();
}

void AGain::setParameter (VstInt32 index, float value)
{
    std::cout << "----------------------------AGain::setParameter" << std::endl;
    
    fGain = value;
    
    
    axEventManager* evtManager = axEventManager::GetInstance();
    
    if(evtManager != nullptr)
    {
        std::cout << "evtManager exist." << std::endl;
        evtManager->PushEvent(10000000, 14,
                              new axVstParameterMsg(value));
    }
}

//-----------------------------------------------------------------------------------------
float AGain::getParameter (VstInt32 index)
{
    return fGain;
}

//-----------------------------------------------------------------------------------------
void AGain::getParameterName (VstInt32 index, char* label)
{
    vst_strncpy(label, "Gain", kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void AGain::getParameterDisplay (VstInt32 index, char* text)
{
    dB2string(fGain, text, kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void AGain::getParameterLabel (VstInt32 index, char* label)
{
    vst_strncpy(label, "dB", kVstMaxParamStrLen);
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

//-----------------------------------------------------------------------------------------
void AGain::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
    
    while (--sampleFrames >= 0)
    {
        (*out1++) = (*in1++) * fGain;
        (*out2++) = (*in2++) * fGain;
    }
}

//-----------------------------------------------------------------------------------------
void AGain::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
    double dGain = fGain;
    
    while (--sampleFrames >= 0)
    {
        (*out1++) = (*in1++) * dGain;
        (*out2++) = (*in2++) * dGain;
    }
}

VstIntPtr AGain::dispatcher(VstInt32 opCode,
                            VstInt32 index,
                            VstIntPtr value,
                            void* ptr, float opt)
{
    if(opCode == effEditOpen)
    {
        std::cout << "AGain::dispatcher : effEditOpen." << std::endl;
        editor->open(ptr);
    }
    else if(opCode == effEditClose)
    {
        std::cout << "AGain::dispatcher : effEditClose." << std::endl;
    }
    else if(opCode == effBeginLoadProgram)
    {
        std::cout << "AGain::dispatcher : effBeginLoadProgram." << std::endl;
    }
    else if(opCode == effClose)
    {
        std::cout << "AGain::dispatcher : effClose." << std::endl;
    }
    
    return AudioEffectX::dispatcher(opCode, index, value, ptr, opt);
}

//------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    //    InitMacApp();
    std::cout << "AudioEffect* createEffectInstance" << std::endl;
    return new AGain(audioMaster);
}

void axMain::MainEntryPoint(axApp* app)
{
    std::cout << "axMain::MainEntryPoint(axApp* app)" << std::endl;
    axTestPanel* panel = new axTestPanel(nullptr, axRect(0, 0, 200, 200));
}
