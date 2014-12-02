//
//  EnvelopeEditor.cpp
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-18.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "EnvelopeEditor.h"
#include "SoundEditorAudio.h"
#include "axAudioBuffer.h"
#include "axAudioFilter.h"
//#include "axAudioBufferPlayer.h"

EnvelopeEditor::EnvelopeEditor(axWindow* parent,
                               const axRect& rect):
axPanel(parent, rect)
{
	//std::string folder_path("/Users/alexarse/Project/axLib/axExamples/Demo/");
	std::string folder_path("C:/Users/Alexandre Arsenault/Desktop/axLib/axExamples/Demo/");
    
    axSliderInfo sld_info;
    sld_info.img_path = folder_path + std::string("sliderPlain.png");
    sld_info.btn_size = axSize(12, 12);
    sld_info.slider_width = 4;
    
    // Doesn't seem to work for now.
    sld_info.bgColorNormal = axColor(0.7, 0.7, 0.7);
    sld_info.bgColorHover = axColor(0.9, 0.9, 0.9);
    sld_info.bgColorClicked = axColor(0.6, 0.6, 0.6);
    sld_info.contourColor = axColor(0.0, 0.0, 0.0);
    
    sld_info.sliderColorNormal = axColor(0.2, 0.2, 0.9);
    sld_info.sliderColorHover = axColor(0.2, 0.2, 1.0);
    sld_info.sliderColorClicked = axColor(0.2, 0.2, 0.6);
    sld_info.sliderContourColor = axColor(0.2, 0.2, 0.2);
    
    sld_info.backSliderColor = axColor(0.7, 0.7, 0.7);
    sld_info.backSliderContourColor = axColor(0.2, 0.2, 0.2);
    
    
    
    SoundEditorAudio* audio = SoundEditorAudio::GetInstance();
    axAudioBuffer* audio_buffer_obj = audio->GetSoundBuffer();
    axBufferInfo buff_info = audio_buffer_obj->GetBufferInfo();
    float* audio_buffer = audio_buffer_obj->GetBuffer();
    _envBuffer = std::vector<float>(audio_buffer, audio_buffer + buff_info.frames);
    
    axAudioFilter* audio_filter = audio->GetFilter();
    audio_filter->SetFreq(100.0);
    audio_filter->SetQ(0.7071067811865475);
    audio_filter->ProcessMonoBlock(_envBuffer.data(), buff_info.frames);
    
    axSliderEvents sld_evts;
    sld_evts.slider_value_change = GetOnFilterValue();
    axSlider* sld = new axSlider(this, axRect(axPoint(10, 5),
                                                   axSize(200, 15)),
                                 sld_evts, sld_info,
                                 axSLIDER_FLAG_NO_SLIDER_LINE);
    sld->SetBackgroundAlpha(0.1);
    sld->SetValue(1.0);
    
    sld_evts.slider_value_change = GetOnFilterQValue();
    axSlider* sld2 = new axSlider(this, axRect(sld->GetNextPosRight(5),
                                                    axSize(200, 15)),
                                  sld_evts, sld_info,
                                  axSLIDER_FLAG_NO_SLIDER_LINE);
    sld2->SetBackgroundAlpha(0.1);
    sld2->SetValue(1.0);
    
    sld_evts.slider_value_change = GetOnFilterGainValue();
    axSlider* sld3 = new axSlider(this, axRect(sld2->GetNextPosRight(5),
                                                    axSize(200, 15)),
                                  sld_evts, sld_info,
                                  axSLIDER_FLAG_NO_SLIDER_LINE);
    sld3->SetBackgroundAlpha(0.1);
    sld3->SetValue(1.0);
}

void EnvelopeEditor::SetEnvChangeEvent(axEvtFunction(EnvelopeEditorMsg) fct)
{
    _envChangeEvt = fct;
}

void EnvelopeEditor::OnFilterValue(const axSliderMsg& msg)
{
    double value = msg.GetValue();
    value = 100.0 + value * (1000.0 - 100.0);
    std::cout << value << std::endl;
    
    SoundEditorAudio* audio = SoundEditorAudio::GetInstance();
    axAudioFilter* audio_filter = audio->GetFilter();
    
    ProcessEnvelope(value,
                    audio_filter->GetQ(),
                    audio_filter->GetGain());
}

void EnvelopeEditor::OnFilterGainValue(const axSliderMsg& msg)
{
    double value = msg.GetValue();
    value = 1.0 + value * (5.0 - 0.1);
    std::cout << value << std::endl;
    
    SoundEditorAudio* audio = SoundEditorAudio::GetInstance();
    axAudioFilter* audio_filter = audio->GetFilter();
    
    ProcessEnvelope(audio_filter->GetFreq(),
                    audio_filter->GetQ(),
                    value);
}

void EnvelopeEditor::OnFilterQValue(const axSliderMsg& msg)
{
    
    double value = msg.GetValue();
    value = 0.1 + value * (10.0 - 0.1);
    std::cout << value << std::endl;
    
    SoundEditorAudio* audio = SoundEditorAudio::GetInstance();
    axAudioFilter* audio_filter = audio->GetFilter();
    
    ProcessEnvelope(audio_filter->GetFreq(),
                    value,
                    audio_filter->GetGain());
}

void EnvelopeEditor::ProcessEnvelope(const double& freq,
                                     const double& q,
                                     const double& gain)
{
    SoundEditorAudio* audio = SoundEditorAudio::GetInstance();
    axAudioBuffer* audio_buffer_obj = audio->GetSoundBuffer();
    axBufferInfo buff_info = audio_buffer_obj->GetBufferInfo();
    float* audio_buffer = audio_buffer_obj->GetBuffer();
    _envBuffer = std::vector<float>(audio_buffer, audio_buffer + buff_info.frames);
    
    axAudioFilter* audio_filter = audio->GetFilter();
    audio_filter->SetFreq(freq);
    audio_filter->SetQ(q);
    audio_filter->SetGain(gain);
    audio_filter->ProcessMonoBlock(_envBuffer.data(), buff_info.frames);
    
    
    _envPoints.clear();
    // Point of times(percent) and value.
    _envPoints.push_back(axFloatPoint(0.0, 0.0));
    
    float* data = _envBuffer.data();
    double max = 0.0;
    const int N_POINTS = 40;
    const int N_SAMPLES = _envBuffer.size() / double(N_POINTS - 1);
    int index_max = 0;
    for(int i = 0; i < _envBuffer.size(); i++)
    {
        double value = abs(data[i]);
        
        if(value > max)
        {
            max = value;
            index_max = i;
        }
        
        if(i != 0 && i % N_SAMPLES == 0)
        {
            _envPoints.push_back(axFloatPoint(index_max / double(_envBuffer.size()) ,max));
            max = 0.0;
        }
    }
    
    _envPoints.push_back(axFloatPoint(1.0, 0.0));
    
    if(_envChangeEvt)
    {
        _envChangeEvt(EnvelopeEditorMsg(&_envBuffer, &_envPoints));
    }
}

void EnvelopeEditor::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.8, 0.3, 0.3), 1.0);
    gc->DrawRectangle(rect0);
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
}