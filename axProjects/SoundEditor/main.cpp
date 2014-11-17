#include "main.h"
#include "SoundEditorAudio.h"

#include "axFileDialog.h"
#include "axToolBar.h"
#include "axWaveform.h"
#include "axWaveformNavigator.h"

SoundEditor::SoundEditor(axApp* app, axWindow* parent,
									 const axRect& rect):
								     axPanel(app, parent, rect)
{
    axToolBarInfo tbar_info(axColor(0.6, 0.6, 0.6),
                            axColor(0.6, 0.6, 0.6),
                            axColor(0.0, 0.0, 0.0));
    axToolBarEvents tbar_evts;
    _toolbar = new axToolBar(app, this, axRect(1, 0, rect.size.x-1, 40),
                             tbar_evts, tbar_info);
    
    
    axButtonInfo btn_info;
    btn_info.normal = axColor(0.8, 0.8, 0.8);
    btn_info.hover = axColor(0.9, 0.9, 0.9);
    btn_info.clicking = axColor(0.7, 0.7, 0.7);
    btn_info.contour = axColor(0.0, 0.0, 0.0);
    btn_info.selected = btn_info.normal;
    
    axButtonEvents btn1_evts;
    btn1_evts.button_click = GetOnOpenDialog();
    
    axSize btn_size(60, 30);
    
    axButton* btn1 = new axButton(app, _toolbar,
                                  axRect(axPoint(5, 5), btn_size),
                                  btn1_evts,
                                  btn_info,
                                  "", "Open");
    
    
    axButtonEvents btn2_evts;
    btn2_evts.button_click = GetOnPlayButton();
    
    axButton* btn2 = new axButton(app, _toolbar,
                                  axRect(btn1->GetNextPosRight(5), btn_size),
                                  btn2_evts,
                                  btn_info,
                                  "", "Play");
    
    //---------------------------------------------------------------------------
    
    std::string folder_path("/Users/alexarse/Project/axLib/axExamples/Demo/");
    
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
    
    axSliderEvents sld_evts;
    sld_evts.slider_value_change = GetOnZoomValue();
    
    _zoomSlider = new axSlider(app, this, axRect(axPoint(765, 90),
                                                    axSize(15, 245)),
                                  sld_evts, sld_info,
                                  axSLIDER_FLAG_VERTICAL |
                                  axSLIDER_FLAG_RIGHT_ALIGN |
                                  axSLIDER_FLAG_NO_SLIDER_LINE);
    _zoomSlider->SetBackgroundAlpha(0.1);
    _zoomSlider->SetValue(1.0);
    
    
    SoundEditorAudio* audio = SoundEditorAudio::GetInstance();
    
    _waveform = new axWaveform(app, this, axRect(40, 90, 720, 200));
    _waveform->SetAudioBuffer(audio->GetSoundBuffer());
    
    _waveformNavig = new axWaveformNavigator(app, this, axRect(40, 290, 720, 45));
    _waveformNavig->SetAudioBuffer(audio->GetSoundBuffer());
    _waveformNavig->SetValueChangeEvt(GetOnWaveformNavigator());
    
    audio->SetPlayingPositionEvent(_waveformNavig->GetOnPlayingPositionChange());
}

void SoundEditor::SetPlayAudioEvent(axEvtFunction(int) fct)
{
    _evtPlayAudio = fct;
}

void SoundEditor::SetChangePathAudioEvent(axEvtFunction(std::string) fct)
{
    _evtChangePathAudio = fct;
}

void SoundEditor::OnOpenDialog(const axButtonMsg& msg)
{
    std::string file_path = axOpenFileDialog();
    
    if(!file_path.empty())
    {
        std::string extension = axGetExtension(file_path);
        if(extension == std::string("wav") || extension == std::string("WAV"))
        {
            if(_evtChangePathAudio)
            {
                _evtChangePathAudio(file_path);
                _waveformNavig->SetAudioBuffer(SoundEditorAudio::GetInstance()->GetSoundBuffer());
                _waveform->SetAudioBuffer(SoundEditorAudio::GetInstance()->GetSoundBuffer());
                _waveformNavig->SetBorders(_waveform->GetBorders());
                _zoomSlider->SetValue(1.0);
            }
        }
    }
}

void SoundEditor::OnPlayButton(const axButtonMsg& msg)
{
    if(_evtPlayAudio)
    {
        _evtPlayAudio(0);
    }
}

void SoundEditor::OnZoomValue(const axSliderMsg& msg)
{
    _waveform->SetZoom(msg.GetValue());
    _waveformNavig->SetBorders(_waveform->GetBorders());
    Update();
}

void SoundEditor::OnSamplePosition(const axSliderMsg& msg)
{
    _waveform->SetLeftPosition(msg.GetValue());
    _waveformNavig->SetBorders(_waveform->GetBorders());
}

void SoundEditor::OnWaveformNavigator(const double&  left_border)
{
    _waveform->SetLeftPosition(left_border);
}

void SoundEditor::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.3, 0.3, 0.3), 1.0);
	gc->DrawRectangle(rect0);

//    gc->SetColor(axColor(0.7, 0.3, 0.3), 1.0);
//    gc->DrawRectangleContour(rect0.GetInteriorRect(axPoint(10, 10)));

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);
}

//------------------------------------------------------------------------------
// Main.
//------------------------------------------------------------------------------
axMain::axMain()
{
}

void axMain::MainEntryPoint(axApp* app)
{
    SoundEditorAudio* audio = SoundEditorAudio::GetInstance();
    SoundEditor* sound_editor = new SoundEditor(app,
                                                nullptr,
                                                axRect(0, 0, 800, 400));
    
    sound_editor->SetPlayAudioEvent(audio->GetOnPlay());
    sound_editor->SetChangePathAudioEvent(audio->GetOnChangeFilePath());
    
    audio->InitAudio();
    audio->StartAudio();
    
}


