#ifndef __again__
#define __again__

#include "axLib.h"

#include "axAudioWaveTable.h"
#include "axAudioFilter.h"
#include "axAudioEnvelope.h"
#include "axVst.h"

class PolyPhonicChannel
{
public:
    PolyPhonicChannel(axAudioBuffer* waveTableAudioBuffer);
    
    void ProcessChannel(VstInt32 sampleFrames);
    
    double** GetProcessedBuffers();
    
    
    void TriggerNote(const double& freq);

    void SetFilterFreq(const double& freq);
    
private:
    axAudioWaveTable* _waveTable;
    axAudioFilter* _filter;
    axAudioEnvelope* _env;
    
    double** _processBuffer;
};

class AGain : public axVst
{
public:
    AGain(audioMasterCallback audioMaster);

//    VstInt32 processEvents(VstEvents* ev);
    
    virtual void processReplacing(float** inputs,
                                  float** outputs,
                                  VstInt32 sampleFrames);
    
    virtual void processDoubleReplacing(double** inputs,
                                        double** outputs,
                                        VstInt32 sampleFrames);
    
    virtual void OnVstMidiNoteOnEvent(const axVstMidiNoteMsg& msg);
    virtual void OnVstMidiNoteOffEvent(const axVstMidiNoteMsg& msg);

private:
    axEVENT_ACCESSOR(axVstParameterMsg, OnVstParameterValueChange);
//    axEVENT_ACCESSOR(axVstMidiNoteMsg, OnVstMidiNote);
    
    axAudioWaveTable* _waveTable;
    axAudioFilter* _filter;
    axAudioEnvelope* _env;
    
    axAudioBuffer* _waveTableAudioBuffer;
    
//    double** _polyBuffers;
    std::vector<PolyPhonicChannel*> _polyChannels;
    int _polyChannelIndex;
//    PolyPhonicChannel* _polyChannels;
    
    double c0;
    
//    virtual void OnVstMidiNoteOnEvent(const axVstMidiNoteMsg& msg);
//    virtual void OnVstMidiNoteOffEvent(const axVstMidiNoteMsg& msg);
    
//    void OnVstMidiNote(const axVstMidiNoteMsg& msg)
//    {
//        
//    }
    
    void OnVstParameterValueChange(const axVstParameterMsg& msg)
    {
        if(msg.GetIndex() == 0)
        {

        }
        else if(msg.GetIndex() == 1)
        {
//            std::cout << "Filter value " << msg.GetValue() << std::endl;
//            axRange<float> fRange(50.0, 5000.0);
//            float filterValue = fRange.GetValueFromZeroToOne(msg.GetValue());
////
////            for(int i = 0; i < 10; i++)
////            {
////                _polyChannels[i]->SetFilterFreq(filterValue);
////            }
//            
//            _filter->SetFreq(filterValue);
        }
    }
};

#endif