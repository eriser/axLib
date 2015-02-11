#include "ModuleLfo.h"
//#include "KrakenAudio.h"
#include "axAudioWaveTable.h"

/*******************************************************************************
 * ModuleLfo.
 ******************************************************************************/
ModuleLfo::ModuleLfo(axWindow* parent, const axRect& rect, const int& index):
axPanel(parent, rect),
_index(index)
{
    axObject::AddEventFunction("OnFreqChange", GetOnFreqChange());
    axObject::AddEventFunction("OnGainChange", GetOnGainChange());
    axObject::AddEventFunction("OnWaveformChange", GetOnWaveformChange());

    
    std::string app_path = axApp::GetAppPath();    
    axObjectLoader loader(this, app_path + "resources/objects/Lfo.xml");
    
    _waveformLed[0] = axWindow::GetResourceManager()->GetResource("sineLed");
    _waveformLed[1] = axWindow::GetResourceManager()->GetResource("sqrtLed");
    _waveformLed[2] = axWindow::GetResourceManager()->GetResource("triLed");
    _waveformLed[3] = axWindow::GetResourceManager()->GetResource("sawLed");
    
    _waveformLed[0]->SetSelected(true);
}

void ModuleLfo::OnGainChange(const axKnob::Msg& msg)
{
//    KrakenAudio::GetInstance()->SetLfoGain(_index, msg.GetValue());
}

void ModuleLfo::OnFreqChange(const axKnob::Msg& msg)
{
//    double freq = axLineairInterpole(0.1, 20.0, msg.GetValue());
//    KrakenAudio::GetInstance()->SetLfoGain(_index, freq);
}

void ModuleLfo::OnWaveformChange(const axToggle::Msg& msg)
{
    for(auto& n : _waveformLed)
    {
        n->SetSelected(false);
    }
    
    msg.GetSender()->SetSelected(true);
    std::cout << msg.GetMsg() << std::endl;
    
    axAudioWaveTable::axWaveformType wtype =
    axAudioWaveTable::axWaveformType::axWAVE_TYPE_SINE;
    
    if(msg.GetMsg() == "sine")
    {
        wtype = axAudioWaveTable::axWaveformType::axWAVE_TYPE_SINE;
    }
    else if(msg.GetMsg() == "tri")
    {
        wtype = axAudioWaveTable::axWaveformType::axWAVE_TYPE_TRIANGLE;
    }
    else if(msg.GetMsg() == "sqrt")
    {
        wtype = axAudioWaveTable::axWaveformType::axWAVE_TYPE_SQUARE;
    }
    else if(msg.GetMsg() == "saw")
    {
        wtype = axAudioWaveTable::axWaveformType::axWAVE_TYPE_SAW;
    }
    
//    KrakenAudio::GetInstance()->SetLfoWaveform(_index, type);
}
/*******************************************************************************
 * Lfos.
 ******************************************************************************/
Lfos::Lfos(axWindow* parent, const axPoint& pos):
ModuleFrame<ModuleLfo>(parent, axRect(pos, axSize(158, 55 + 17)))
{
    std::string app_path = axApp::GetInstance()->GetAppDirectory();
    app_path += std::string("resources/");
    
    axDropMenuInfo drop_info;
    drop_info.normal = axColor(0.6, 0.6, 0.6, 0.0);
    drop_info.hover = axColor(0.65, 0.65, 0.65, 0.1);
    drop_info.clicking = axColor(0.5, 0.5, 0.5, 0.0);
    drop_info.contour = axColor(0.0, 0.0, 0.0, 0.0);
    drop_info.selected = drop_info.normal;
    drop_info.font_color = axColor(0.5, 0.5, 0.5);
    
    std::vector<std::string> menu_str = {"LFO 1", "LFO 2", "LFO 3", "LFO 4", "LFO 5"};
    _moduleMenu = new axDropMenu(this, axRect(4, 1, 84, 15),
                                 axDropMenuEvents(GetOnModuleChange()),
                                 drop_info,
                                 app_path + std::string("images/DropMenuArrow.png"),
                                 "",
                                 "",
                                 menu_str,
                                 axDROP_MENU_SINGLE_IMG);
    
    AddModule(new ModuleLfo(this, axRect(0, 17, 158, 55), 0), "LFO 1");
    AddModule(new ModuleLfo(this, axRect(0, 17, 158, 55), 1), "LFO 2");
    AddModule(new ModuleLfo(this, axRect(0, 17, 158, 55), 2), "LFO 3");
    AddModule(new ModuleLfo(this, axRect(0, 17, 158, 55), 3), "LFO 4");
    AddModule(new ModuleLfo(this, axRect(0, 17, 158, 55), 4), "LFO 5");
}