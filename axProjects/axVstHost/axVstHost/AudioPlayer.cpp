#include "AudioPlayer.h"
#include "axVstInterface.h"
AudioPlayer::AudioPlayer(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    axButton::Info btnInfo;
    btnInfo.normal = axColor(0.9, 0.4, 0.0);
    btnInfo.hover = axColor(1.0, 0.4, 0.0);
    btnInfo.clicking = axColor(0.95, 0.4, 0.0);
    btnInfo.selected = axColor(0.9, 0.4, 0.0);
    btnInfo.font_color = axColor(1.0);
    btnInfo.contour = axColor(0.0);
    btnInfo.round_corner_radius = 5;

    _openBtn = new axButton(this, axRect(10, 5, 50, 20),
                            axButton::Events(GetOnOpenAudioFile()),
                            btnInfo, "", "Open");

    
    axSliderInfo sld_info;
    sld_info.img_path = std::string("sliderPlain.png");
    sld_info.btn_size = axSize(12, 12);
    sld_info.slider_width = 4;
    
    // Doesn't seem to work for now.
    sld_info.bgColorNormal = axColor::axTransparentColor;
    sld_info.bgColorHover = axColor::axTransparentColor;
    sld_info.bgColorClicked = axColor::axTransparentColor;
    sld_info.contourColor = axColor(0.0, 0.0, 0.0);
    
    sld_info.sliderColorNormal = axColor(0.9, 0.4, 0.0);
    sld_info.sliderColorHover = axColor(1.0, 0.4, 0.0);
    sld_info.sliderColorClicked = axColor(0.95, 0.4, 0.0);
    sld_info.sliderContourColor = axColor(0.2, 0.2, 0.2);
    
    sld_info.backSliderColor = axColor(0.7, 0.7, 0.7);
    sld_info.backSliderContourColor = axColor(0.2, 0.2, 0.2);
    
    axWaveform::Info waveformInfo(
                                  axColor::axTransparentColor, // Background.
                                  axColor(0.3), // Waveform.
                                  axColor(0.0), // Contour.
                                  axColor(0.7), // Grid main lines.
                                  axColor(0.8), // Grid second lines.
                                  axColor(0.5), // Middle line.
                                  axColor(1.0, 0.0, 0.0) // Env color.
                                  );
    
    _waveform = new axWaveform(this, axRect(10, 50, 470, 100), waveformInfo);
    _waveformNavigator = new axWaveformNavigator(this,
                                                 axRect(_waveform->GetNextPosDown(0),
                                                        axSize(470, 20)),
                                                 axSliderEvents(GetOnNavigatorEvent()));
    
    axSlider* sld = new axSlider(this, axRect(_waveform->GetNextPosRight(0) - axPoint(1, 0), axSize(12, 120)),
                                 axSliderEvents(GetOnZoomValueChange()), sld_info, axSLIDER_FLAG_VERTICAL | axSLIDER_FLAG_RIGHT_ALIGN);
    
    sld->SetValue(1.0);

    // Backward button.
    axButton* back = new axButton(this,
                                   axRect(axPoint(90, 3),
                                          axSize(25, 25)),
                                   axButton::Events(),
                                   axBUTTON_TRANSPARENT,
                                   "backwardBtn.png");
    
    // Play pause button.
    axToggle::Info togInfo;
    togInfo.normal = axColor::axTransparentColor;
    togInfo.hover = axColor::axTransparentColor;
    togInfo.clicking = axColor::axTransparentColor;
    togInfo.selected = axColor::axTransparentColor;
    togInfo.selected_hover = axColor::axTransparentColor;
    togInfo.selected_clicking = axColor::axTransparentColor;
    togInfo.contour = axColor::axTransparentColor;
    togInfo.img = "playPauseBtn.png";
    togInfo.single_img = false;
    
    axToggle* _play = new axToggle(this,
                                    axRect(back->GetNextPosRight(5), axSize(25, 25)),
                                    axToggle::Events(GetOnPlayAudioFile()),
                                    togInfo);
    
    // Stop button.
    axButton* stop = new axButton(this,
                                   axRect(_play->GetNextPosRight(5), axSize(25, 25)),
                                   axButton::Events(),
                                   axBUTTON_TRANSPARENT,
                                   "stopBtn.png");
    
    // Forward button.
    axButton* forw = new axButton(this,
                                   axRect(stop->GetNextPosRight(5), axSize(25, 25)),
                                   axButton::Events(),
                                   axBUTTON_TRANSPARENT,
                                   "forwardBtn.png");
}

void AudioPlayer::OnOpenAudioFile(const axButton::Msg& msg)
{
    std::string file_path = axApp::GetInstance()->GetCore()->OpenFileDialog();
    
    axPrint(file_path);
    
    if(!file_path.empty())
    {
//        Audio* audio = GetApp()->GetResourceManager()->GetResource("Audio");
//        audio->SetAudioFile(file_path);
//        axAudioBuffer* buffer = audio->GetAudioFile();
        
//        _waveform->SetAudioBuffer(buffer);
//        _waveformNavigator->SetAudioBuffer(buffer);
    }
}

void AudioPlayer::OnPlayAudioFile(const axToggle::Msg& msg)
{
//    Audio* audio = GetApp()->GetResourceManager()->GetResource("Audio");
    
    if(msg.GetSelected())
    {
//        audio->PlayAudioFile();
    }
    else
    {
        // Pause.
    }
    
}

void AudioPlayer::OnZoomValueChange(const axSliderMsg& msg)
{
//    _waveform->SetZoom(msg.GetValue());
//    _waveformNavigator->SetBorders(_waveform->GetBorders());
//    Update();
}

void AudioPlayer::OnNavigatorEvent(const axSliderMsg& msg)
{
//    _waveform->SetLeftPosition(msg.GetValue());
}

void AudioPlayer::OnPaint()
{
    axGC gc(this);
    axRect rect(GetRect());
    
    gc.DrawRectangleColorFade(rect, axColor(0.8), axColor(0.9));
    
    gc.SetColor(axColor(0.45));
    gc.DrawRectangle(axRect(0, 0, rect.size.x, 30));
}
