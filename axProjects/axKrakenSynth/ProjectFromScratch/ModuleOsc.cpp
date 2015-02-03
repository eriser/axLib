#include "ModuleOsc.h"
#include "axObjectLoader.h"

/*******************************************************************************
 * Oscillators.
 ******************************************************************************/
ModuleOsc::ModuleOsc(axWindow* parent, const axRect& rect, const int& index):
axPanel(parent, rect),
_index(index)
{
    axObject::AddEventFunction("OnKnobGainChange", GetOnKnobGainChange());
    axObject::AddEventFunction("OnOctaveChange", GetOnOctaveChange());
    axObject::AddEventFunction("OnWaveformChange", GetOnWaveformChange());
    axObject::AddEventFunction("OnSemiToneChange", GetOnSemiToneChange());
    axObject::AddEventFunction("OnKnobPanChange", GetOnKnobPanChange());
    
    std::string app_path = axApp::GetAppPath();
    axObjectLoader loader(this, app_path + "ressources/objects/Osc.xml");
    
    _font = new axFont(0);
    _font->SetFontSize(10);
    
    _waveformLed[0] = axWindow::GetResourceManager()->GetResource("sineLed");
    _waveformLed[1] = axWindow::GetResourceManager()->GetResource("sqrtLed");
    _waveformLed[2] = axWindow::GetResourceManager()->GetResource("triLed");
    _waveformLed[3] = axWindow::GetResourceManager()->GetResource("sawLed");
    
    _knobGain = axWindow::GetResourceManager()->GetResource("gain");
    _knobPan = axWindow::GetResourceManager()->GetResource("pan");
    _knobPan->SetValue(0.5);
    _waveformLed[0]->SetSelected(true);
}

void ModuleOsc::OnKnobGainChange(const axKnob::Msg& msg)
{
    KrakenAudio::GetInstance()->SetOscillatorGain(_index, msg.GetValue());
}

void ModuleOsc::OnKnobPanChange(const axKnob::Msg& msg)
{
    KrakenAudio::GetInstance()->SetOscillatorPan(_index, msg.GetValue());
}

void ModuleOsc::OnOctaveChange(const axNumberBox::Msg& msg)
{
    KrakenAudio::GetInstance()->SetOscillatorOctave(_index,
                                                    int(msg.GetValue()));
}

void ModuleOsc::OnSemiToneChange(const axNumberBox::Msg& msg)
{
    KrakenAudio::GetInstance()->SetOscillatorSemiTone(_index,
                                                      int(msg.GetValue()));
}

void ModuleOsc::OnWaveformChange(const axToggle::Msg& msg)
{
    for(auto& n : _waveformLed)
    {
        n->SetSelected(false);
    }
    
    msg.GetSender()->SetSelected(true);
    std::cout << msg.GetMsg() << std::endl;
    
    axAudioWaveTable::axWaveformType type =
    axAudioWaveTable::axWaveformType::axWAVE_TYPE_SINE;
    
    if(msg.GetMsg() == "sine")
    {
        type = axAudioWaveTable::axWaveformType::axWAVE_TYPE_SINE;
    }
    else if(msg.GetMsg() == "tri")
    {
        type = axAudioWaveTable::axWaveformType::axWAVE_TYPE_TRIANGLE;
    }
    else if(msg.GetMsg() == "sqrt")
    {
        type = axAudioWaveTable::axWaveformType::axWAVE_TYPE_SQUARE;
    }
    else if(msg.GetMsg() == "saw")
    {
        type = axAudioWaveTable::axWaveformType::axWAVE_TYPE_SAW;
    }
    
    KrakenAudio::GetInstance()->SetOscillatorWaveform(_index, type);
}

void ModuleOsc::OnPaint()
{
    axGC* gc = GetGC();

    gc->SetColor(axColor(0.5, 0.5, 0.5, 1.0));
    gc->DrawString(*_font, "Filter", axPoint(12, 59));
    
    gc->DrawString(*_font, "1", axPoint(43, 59));
    gc->DrawString(*_font, "2", axPoint(73, 59));
    gc->DrawString(*_font, "3", axPoint(103, 59));
}

/*******************************************************************************
 * Oscillators.
 ******************************************************************************/
Oscillators::Oscillators(axWindow* parent, const axPoint& pos):
ModuleFrame<ModuleOsc>(parent, axRect(pos, axSize(158, 120 + 17)))
{
    std::string app_path = axApp::GetInstance()->GetAppDirectory();
    app_path += std::string("ressources/");
    
    
//    _font = new axFont(0);

    
    axDropMenuInfo drop_info;
    drop_info.normal = axColor(0.6, 0.6, 0.6, 0.0);
    drop_info.hover = axColor(0.65, 0.65, 0.65, 0.1);
    drop_info.clicking = axColor(0.5, 0.5, 0.5, 0.0);
    drop_info.contour = axColor(0.0, 0.0, 0.0, 0.0);
    drop_info.selected = drop_info.normal;
    drop_info.font_color = axColor(0.5, 0.5, 0.5);
    
    std::vector<std::string> menu_str = {"OSC 1", "OSC 2", "OSC 3", "OSC 4", "OSC 5"};
    _moduleMenu = new axDropMenu(this, axRect(4, 1, 84, 15),
                                 axDropMenuEvents(GetOnModuleChange()),
                                 drop_info,
                                 app_path + std::string("images/DropMenuArrow.png"),
                                 "",
                                 "",
                                 menu_str,
                                 axDROP_MENU_SINGLE_IMG);
    
    AddModule(new ModuleOsc(this, axRect(0, 17, 158, 120), 0), "OSC 1");
    AddModule(new ModuleOsc(this, axRect(0, 17, 158, 120), 1), "OSC 2");
    AddModule(new ModuleOsc(this, axRect(0, 17, 158, 120), 2), "OSC 3");
    AddModule(new ModuleOsc(this, axRect(0, 17, 158, 120), 3), "OSC 4");
    AddModule(new ModuleOsc(this, axRect(0, 17, 158, 120), 4), "OSC 5");
    
    
    
    KrakenAudio::GetInstance()->SetOscillatorOnOff(0, true);
}

