#include "ModuleAudioFile.h"
#include "axObjectLoader.h"
#include "axWaveform.h"
#include "axWaveformNavigator.h"
#include "axAudioBuffer.h"

/*******************************************************************************
 * ModuleAudioFile.
 ******************************************************************************/
ModuleAudioFile::ModuleAudioFile(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    std::string app_path = axApp::GetInstance()->GetAppDirectory();
    axObjectLoader loader(this, app_path + "ressources/objects/Audiofile.xml");
    
    
//    _wave = axWindow::GetResourceManager()->GetResource("waveform");
    
//    axAudioBuffer* audioBuffer = new axAudioBuffer("ressources/sounds/snare.wav");
//    
//    axRect navRect(_wave->GetNextPosDown(0), axSize(rect.size.x - 30, 15));
//    _navig = new axWaveformNavigator(this, navRect);
//    
//    _wave->SetAudioBuffer(audioBuffer);
//    _navig->SetAudioBuffer(audioBuffer);
    
//    axSliderInfo sld_info;
//    sld_info.img_path = app_path + std::string("ressources/images/sliderPlain.png");
//    sld_info.btn_size = axSize(12, 12);
//    sld_info.slider_width = 4;
//    
//    // Doesn't seem to work for now.
//    sld_info.bgColorNormal = axColor(0.7, 0.7, 0.7, 0.0);
//    sld_info.bgColorHover = axColor(0.9, 0.9, 0.9, 0.0);
//    sld_info.bgColorClicked = axColor(0.6, 0.6, 0.6, 0.0);
//    sld_info.contourColor = axColor(0.5, 0.5, 0.5, 1.0);
//    
//    sld_info.sliderColorNormal = axColor(0.2, 0.2, 0.9, 0.1);
//    sld_info.sliderColorHover = axColor(0.2, 0.2, 1.0, 0.1);
//    sld_info.sliderColorClicked = axColor(0.2, 0.2, 0.6, 0.1);
//    sld_info.sliderContourColor = axColor(0.2, 0.2, 0.2, 0.1);
//    
//    sld_info.backSliderColor = axColor(0.7, 0.7, 0.7, 0.1);
//    sld_info.backSliderContourColor = axColor(0.2, 0.2, 0.2, 0.1);
//    
//    axSliderEvents sld_evts;
//    sld_evts.slider_value_change = GetOnZoomValue();
    
//    axSlider* zoom = new axSlider(this, axRect(_wave->GetNextPosRight(0),
//                                               axSize(15, rect.size.y - 100)),
//                                  sld_evts, sld_info,
//                                  axSLIDER_FLAG_VERTICAL |
//                                  axSLIDER_FLAG_RIGHT_ALIGN |
//                                  axSLIDER_FLAG_NO_SLIDER_LINE);
//    zoom->SetValue(1.0);

}

void ModuleAudioFile::OnZoomValue(const axSliderMsg& msg)
{
//    _wave->SetZoom(msg.GetValue());
//    _navig->SetBorders(_wave->GetBorders());
    Update();
}

/*******************************************************************************
 * AudioFiles.
 ******************************************************************************/
AudioFiles::AudioFiles(axWindow* parent, const axPoint& pos):
ModuleFrame<ModuleAudioFile>(parent, axRect(pos, axSize(383, 200 + 17)))
{
    std::string app_path = axApp::GetInstance()->GetAppDirectory();
    app_path += std::string("ressources/");

    axDropMenuInfo drop_info;
    drop_info.normal = axColor(0.6, 0.6, 0.6, 0.0);
    drop_info.hover = axColor(0.65, 0.65, 0.65, 0.1);
    drop_info.clicking = axColor(0.5, 0.5, 0.5, 0.0);
    drop_info.contour = axColor(0.0, 0.0, 0.0, 0.0);
    drop_info.selected = drop_info.normal;
    drop_info.font_color = axColor(0.5, 0.5, 0.5);
    
    std::vector<std::string> menu_str = {"WAVE 1", "WAVE 2", "WAVE 3"};
    _moduleMenu = new axDropMenu(this, axRect(4, 1, 84, 15),
                                 axDropMenuEvents(GetOnModuleChange()),
                                 drop_info,
                                 app_path + std::string("images/DropMenuArrow.png"),
                                 "",
                                 "",
                                 menu_str,
                                 axDROP_MENU_SINGLE_IMG);
    
    AddModule(new ModuleAudioFile(this, axRect(0, 17, 380, 200)), "WAVE 1");
    AddModule(new ModuleAudioFile(this, axRect(0, 17, 380, 200)), "WAVE 2");
    AddModule(new ModuleAudioFile(this, axRect(0, 17, 380, 200)), "WAVE 3");
}