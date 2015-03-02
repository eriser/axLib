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
#include "axVst.h"

#include "axEventManager.h"
#include <iostream>

#ifdef __APPLE__
#include "axVstGuiMac.h"
#elif _MSC_VER
#pragma message("axVstGuiWin32 not implemented yet.")
#endif


int axVst::pluginIdCounter = 0;
std::mutex axVst::axVstPrivateMutex;

//------------------------------------------------------------------------------
axVst::axVst(audioMasterCallback audioMaster, const int& numberParameters) :
// Heritage.
AudioEffectX(audioMaster, 1, numberParameters)	// 1 program, 1 parameter only
{
    axVst::axVstPrivateMutex.lock();
    setNumInputs (2);		// stereo in
    setNumOutputs (2);		// stereo out
    setUniqueID ('axEq');	// identify
    canProcessReplacing();	// supports replacing output
    canDoubleReplacing();	// supports double precision processing
    
    vst_strncpy (programName, "axTB303", kVstMaxProgNameLen);	// default program name
    
    char* dir = (char*)getDirectory();
    
    editor = new axVstGuiMac(this);
    
    _pluginId = pluginIdCounter++;
    
    isSynth(false);
    
    _canDoList.insert(std::string("receiveVstEvents"));
    _canDoList.insert(std::string("receiveVstMidiEvent"));
    _canDoList.insert(std::string("midiProgramNames"));
    
//    curProgram = _pluginId;
    axVst::axVstPrivateMutex.unlock();
}



//-------------------------------------------------------------------------------------------------------
axVst::~axVst ()
{
    // nothing to do here
}

int axVst::GetPluginId() const
{
    return _pluginId;
}

void axVst::AddParameter(const axParameterInfo& param)
{
    axVst::axVstPrivateMutex.lock();
    _parameters.push_back(param);
    axVst::axVstPrivateMutex.unlock();
}

void axVst::AddCapability(const std::string& capability)
{
    axVst::axVstPrivateMutex.lock();
    _canDoList.insert(capability);
    axVst::axVstPrivateMutex.unlock();
}

void axVst::open()
{
    std::cout << "axVst::open." << std::endl;
    //    std::cout << "axVst::open ----- >  " << _pluginId << std::endl;
    //    std::cout << "axVst : uID" << getCurrentUniqueId() << std::endl;
    //    std::cout << "axVst : program" << getProgram() << std::endl;
}

void axVst::close()
{
    //    std::cout << "axVst::close ----- >  " << _pluginId << std::endl;
}

//-------------------------------------------------------------------------------------------------------
void axVst::setProgramName (char* name)
{
    vst_strncpy(programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void axVst::getProgramName (char* name)
{
    vst_strncpy(name, programName, kVstMaxProgNameLen);
    
}

//VstInt32 axVst::getProgram()
//{
//    std::cout << "axVst::getProgram." << std::endl;
////    axVst::axVstPrivateMutex.lock();
//    VstInt32 progId = _pluginId;
////    axVst::axVstPrivateMutex.unlock();
//    return progId;
//}

void axVst::SetParameterFromGUI(VstInt32 index, float value)
{
    if(index < _parameters.size())
    {
        _parameters[index].value = value;
    }
    updateDisplay();
}

void axVst::setParameter (VstInt32 index, float value)
{
    if(index < _parameters.size())
    {
        _parameters[index].value = value;

//        axEventManager::GetInstance()->PushEvent(10000000 + getProgram(),
//                          0, new axVstParameterMsg(value, index));
        
        axEventManager::GetInstance()->PushEvent(10000000 + GetPluginId(),
                                                 0, new axVstParameterMsg(value, index));
    }
}

float axVst::getParameter (VstInt32 index)
{
    if(index < _parameters.size())
    {
        return _parameters[index].value;
    }

    return 0.0f;
}

void axVst::getParameterName (VstInt32 index, char* label)
{
    if(index < _parameters.size())
    {
        vst_strncpy(label, _parameters[index].name.c_str(),
                    kVstMaxParamStrLen);
    }
//    if(index == 0)
//    {
//        vst_strncpy(label, "Gain", kVstMaxParamStrLen);
//    }
//    else if(index == 1)
//    {
//        vst_strncpy(label, "Filter Freq", kVstMaxParamStrLen);
//    }
    
}

void axVst::getParameterDisplay (VstInt32 index, char* text)
{
    if(index < _parameters.size())
    {
        float2string(_parameters[index].value,
                     text, kVstMaxParamStrLen);
    }
//    if(index == 0)
//    {
//        dB2string(fGain, text, kVstMaxParamStrLen);
//    }
//    else if(index == 1)
//    {
//        float2string(_filterFreq, text, kVstMaxParamStrLen);
//    }
//    
}

void axVst::getParameterLabel (VstInt32 index, char* label)
{
    if(index < _parameters.size())
    {
        vst_strncpy(label, _parameters[index].label.c_str(),
                    kVstMaxParamStrLen);
    }
    
    
//    if(index == 0)
//    {
//        vst_strncpy(label, "dB", kVstMaxParamStrLen);
//    }
//    else if(index == 1)
//    {
//        vst_strncpy(label, "Hz", kVstMaxParamStrLen);
//    }
}

//------------------------------------------------------------------------
bool axVst::getEffectName (char* name)
{
    vst_strncpy(name, "axTB303", kVstMaxEffectNameLen);
    
    return true;
}

//------------------------------------------------------------------------
bool axVst::getProductString (char* text)
{
    vst_strncpy(text, "axTB303", kVstMaxProductStrLen);
    return true;
}

//------------------------------------------------------------------------
bool axVst::getVendorString (char* text)
{
    vst_strncpy(text, "Steinberg Media Technologies", kVstMaxVendorStrLen);
    return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 axVst::getVendorVersion ()
{
    return 1002;
}

VstPlugCategory axVst::getPlugCategory()
{
    return kPlugCategSynth;
}

void axVst::setBlockSize(long blockSize)
{
    std::cout << "BlockSize : " << blockSize << std::endl;
}

VstInt32 axVst::canDo(char *text)
{
    std::string needToDo(text);
    
    if(_canDoList.find(needToDo) != _canDoList.end())
    {
        return 1;
    }
    
    return 0;
    
//    // TODO: Fill in according to your plugin's capabilities
//    if(!strcmp(text, "receiveVstEvents"))
//    {
//        return 1;
//    }
//    else if(!strcmp(text, "receiveVstMidiEvent"))
//    {
//        return 1;
//    }
//    else if(!strcmp(text, "midiProgramNames"))
//    {
//        return 1;
//    }
//    
//    // -1 => explicitly can't do; 0 => don't know
//    return 0;
}

VstInt32 axVst::processEvents(VstEvents* ev)
{
    axVst::axVstPrivateMutex.lock();
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
//                axVstMidiNoteMsg msg((int)midiData[1], (int)midiData[2]);
                int midiNote = (int)midiData[1];
                OnVstMidiNoteOnEvent(axVstMidiNoteMsg(midiNote, 0));
//                int midiNote = (int)midiData[1];
//                
//                //                n += oct + semi;
//                double freq = c0 * pow(ST_RATIO, midiNote);
//                _waveTable->SetFreq(freq);
//                
//                
//                _env->TriggerNote();
                
                
                
                //                std::cout << "On : " << (int)midiData[1] << " " <<  (int)midiData[2] << std::endl;
                
            }
            // Note off.
            else if(status == 0x80)
            {
                int midiNote = (int)midiData[1];
                OnVstMidiNoteOffEvent(axVstMidiNoteMsg(midiNote, 0));
                //                std::cout << "Off : " << (int)midiData[1] << " " <<  (int)midiData[2] << std::endl;
                
            }
        }
    }
    axVst::axVstPrivateMutex.unlock();
    return 1;
}

//-----------------------------------------------------------------------------------------
void axVst::processReplacing(float** inputs, float** outputs, VstInt32 sampleFrames)
{
    std::cout << "No replacement float." << std::endl;
    //    float* in1  =  inputs[0];
    //    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];
    
    
    while (--sampleFrames >= 0)
    {
//        float value = 0.0;
//        _waveTable->ProcessSample(&value);
//        
//        t_out input_filter(value, value);
//        t_out filter_processed = _filter->ProcessStereo(input_filter);
//        
//        float env = _env->Process();
        
        //        t_out input_filter(*(output), *(output+1));
        //        t_out filter_processed = _filter->ProcessStereo(input_filter);
//        
//        *out1 = filter_processed.l * env * fGain;
//        *out2 = filter_processed.r * env * fGain;
        
        out1++;
        out2++;
        
        //        (*out1++) = (*in1++) * fGain;
        //        (*out2++) = (*in2++) * fGain;
    }
}

//-----------------------------------------------------------------------------------------
void axVst::processDoubleReplacing(double** inputs, double** outputs, VstInt32 sampleFrames)
{
    std::cout << "No replacement double." << std::endl;
    //    double* in1  =  inputs[0];
    //    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
//    double dGain = fGain;
    
    while (--sampleFrames >= 0)
    {
//        double value = 0.0;
//        _waveTable->ProcessSample(&value);
//        
//        t_out input_filter(value, value);
//        t_out filter_processed = _filter->ProcessStereo(input_filter);
//        
//        double env = _env->Process();
//        
//        *out1 = filter_processed.l * env * fGain;
//        *out2 = filter_processed.r * env * fGain;
        
        out1++;
        out2++;
        
        //        (*out1++) = (*in1++) * dGain;
        //        (*out2++) = (*in2++) * dGain;
    }
}

VstIntPtr axVst::dispatcher(VstInt32 opCode,
                            VstInt32 index,
                            VstIntPtr value,
                            void* ptr, float opt)
{
    if(opCode == effEditOpen)
    {
        //        std::cout << "axVst::dispatcher : effEditOpen." << std::endl;
        //        editor->open(ptr);
    }
    else if(opCode == effEditClose)
    {
        //        std::cout << "axVst::dispatcher : effEditClose." << std::endl;
    }
    else if(opCode == effBeginLoadProgram)
    {
        //        std::cout << "axVst::dispatcher : effBeginLoadProgram." << std::endl;
    }
    else if(opCode == effClose)
    {
        //        std::cout << "axVst::dispatcher : effClose." << std::endl;
    }
//case effSetProgram:			if (value < numPrograms) setProgram ((VstInt32)value); break;
//case effGetProgram:			v = getProgram ();									break;
    
    return AudioEffectX::dispatcher(opCode, index, value, ptr, opt);
}