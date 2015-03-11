//
//  AGain.h
//  AGain
//
//  Created by Alexandre Arsenault on 2015-03-03.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __again__
#define __again__

//#include <axLib/axLib.h>

//class EqVst : public axVst
//{
//public:
//    EqVst(audioMasterCallback audioMaster);
//    
//    virtual void open();
//    
//private:
//};
//#include "audioeffectx.h"

#include "axVst.h"

//-------------------------------------------------------------------------------------------------------
class AGain : public AudioEffectX
{
public:
    AGain (audioMasterCallback audioMaster);
    ~AGain ();
    
    // Processing
    virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
    virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);
    
    // Program
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    
    // Parameters
    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterLabel (VstInt32 index, char* label);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);
    
    virtual bool getEffectName (char* name);
    virtual bool getVendorString (char* text);
    virtual bool getProductString (char* text);
    virtual VstInt32 getVendorVersion ();
    
protected:
    float fGain;
    char programName[kVstMaxProgNameLen + 1];
};

#endif

