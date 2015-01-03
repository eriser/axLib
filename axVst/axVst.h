/*******************************************************************************
 * Copyright (c) 2013 Alexandre Arsenault.
 *
 * This file is part of axLibrary.
 *
 * axLibrary is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axLibrary License.
 *
 * axLibrary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axLibrary. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axLibrary, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 ******************************************************************************/

#ifndef __AX_VST__
#define __AX_VST__

/*******************************************************************************
 * @file    axVst
 * @author  Alexandre Arsenault <alx.arsenault@gmail.com>
 * @brief   Wrapper for vst AudioEffectX class.
 * @date    02/01/2014
 ******************************************************************************/

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "public.sdk/source/vst2.x/aeffeditor.h"
#include "axMsg.h"
#include <string>
#include <vector>
#include <set>

class axVstParameterMsg : public axMsg
{
public:
    axVstParameterMsg(const double& value, const int& index):
    _value(value), _index(index)
    {
    }
    
    double GetValue() const
    {
        return _value;
    }
    
    int GetIndex() const
    {
        return _index;
    }
    
    axMsg* GetCopy()
    {
        return new axVstParameterMsg(*this);
    }
    
private:
    double _value;
    int _index;
};

class axVstMidiNoteMsg //: public axMsg
{
public:
    axVstMidiNoteMsg(const int& note, const int& velocity):
    _note(note), _velocity(velocity)
    {
    }
    
    int GetNote() const
    {
        return _note;
    }
    
    int GetVelocity() const
    {
        return _velocity;
    }
    
//    axMsg* GetCopy()
//    {
//        return new axVstMidiNoteMsg(*this);
//    }
    
private:
    int _note, _velocity;
};

class axVst : public AudioEffectX
{
public:
    axVst(audioMasterCallback audioMaster, const int& numberParameters);
    ~axVst();
    
    virtual void open();
    virtual void close();
    
    virtual VstInt32 getProgram();
    
    
    // Processing
    virtual void processReplacing(float** inputs,
                                  float** outputs,
                                  VstInt32 sampleFrames);
    
    virtual void processDoubleReplacing(double** inputs,
                                        double** outputs,
                                        VstInt32 sampleFrames);
    
    // Program
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    
    // Parameters
    void SetParameterFromGUI(VstInt32 index, float value);
    
    virtual void setParameter(VstInt32 index, float value);
    virtual float getParameter(VstInt32 index);
    virtual void getParameterLabel(VstInt32 index, char* label);
    virtual void getParameterDisplay(VstInt32 index, char* text);
    virtual void getParameterName(VstInt32 index, char* text);
    
    virtual bool getEffectName(char* name);
    virtual bool getVendorString(char* text);
    virtual bool getProductString(char* text);
    virtual VstInt32 getVendorVersion();
    
    virtual void setBlockSize(long blockSize);
    
    virtual VstInt32 canDo(char *text);
    
    VstInt32 processEvents(VstEvents* ev);
    
    virtual VstPlugCategory getPlugCategory();
    
    virtual VstIntPtr dispatcher(VstInt32 opCode,
                                 VstInt32 index,
                                 VstIntPtr value,
                                 void* ptr, float opt);
    
    enum axVstPlugCategory
    {
        axVstPlugUnknown = 0,		// Unknown, category not implemented.
        axVstPlugEffect,			// Simple Effect.
        axVstPlugSynth,             // VST Instrument (Synths, samplers).
        axVstPlugAnalysis,			// Scope, Tuner.
        axVstPlugMastering,         // Dynamics.
        axVstPlugSpacializer,		// Panners.
        axVstPlugRoomFx,			// Delays and Reverbs.
        axVstPlugSurroundFx,		// Dedicated surround processor.
        axVstPlugRestoration,		// Denoiser.
        axVstPlugOfflineProcess,	// Offline Process
        axVstPlugShell,             // Plug-in is container of other plug-ins.
        axVstPlugGenerator,         // ToneGenerator.
        
        axVstPlugMaxCount
    };
    
    virtual void OnVstMidiNoteOnEvent(const axVstMidiNoteMsg& msg){}
    virtual void OnVstMidiNoteOffEvent(const axVstMidiNoteMsg& msg){}
    
protected:
    int _pluginId;
    static int pluginIdCounter;
    char programName[kVstMaxProgNameLen + 1];
    
    struct axParameterInfo
    {
        axParameterInfo(const std::string& _name,
                        const std::string& _label, const float& _value):
        name(_name),
        label(_label),
        value(_value)
        
        {
            
        }
        
        axParameterInfo()
        {
            
        }
        
        std::string name, label;
        float value;
    };
    
    std::vector<axParameterInfo> _parameters;
    std::set<std::string> _canDoList;
    


    
    void AddParameter(const axParameterInfo& param);
    void AddCapability(const std::string& capability);
};


#endif // __AX_VST__
