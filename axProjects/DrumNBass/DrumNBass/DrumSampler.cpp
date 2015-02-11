//
//  DrumSampler.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-06.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "DrumSampler.h"

SynthControl::SynthControl(axWindow* parent,
                           const axRect& rect,
                           const string& snd_path,
//                           axAudio* audio,
                           const int& track_num) :
axPanel(parent, rect),
_trackNum(track_num)
{
    
    std::string app_path(axApp::GetAppPath());
    std::string images_path(app_path + "resources/images/");
//    _audio = static_cast<AudioMidiSeq*>(audio);
    _bgImg = new axImage("/Users/alexarse/Project/axLib/axProjects/MidiSequencer/synth_bg.png");
    
    _font = new axFont(0);
    
    axButton::Info btn_info(axColor(0.2, 0.2, 0.2),
                            axColor(0.3, 0.3, 0.3),
                            axColor(0.2, 0.2, 0.2),
                            axColor(0.2, 0.2, 0.2),
                            axColor(0.0, 0.0, 0.0),
                            axColor(0.8, 0.8, 0.8));
    
    axButton::Events btnEvents;
    btnEvents.button_click = GetOnOpenFile();
    axButton* btn = new axButton(this, axRect(10, 5, 60, 20),
                                 btnEvents, btn_info, "", "Open");
    
    
    _wave = new axWaveform(this,
                           axRect(10, 30, rect.size.x - 20, 80),
                           axWaveform::Info(axColor(0.2, 0.2, 0.2, 1.0),
                                            axColor(0.6, 0.6, 0.6),
                                            axColor(0.0, 0.0, 0.0),
                                            axColor(0.0, 0.0, 0.0),
                                            axColor(0.5, 0.5, 0.5),
                                            axColor(0.5, 0.5, 0.5),
                                            axColor(0.5, 0.5, 0.5)));
//    _wave->SetAudioBuffer(_audio->GetAudioBufferFromTrack(_trackNum));
    
    axKnob::Info knob_info(axColor(0.3, 0.3, 0.3, 0.0),
                           axColor(0.3, 0.3, 0.3, 0.0),
                           axColor(0.3, 0.3, 0.3, 0.0),
                           128,
                           axSize(46, 46),
                           images_path + "axLittlePhatty.png",
                           images_path + "axLittlePhatty.png");
    
    axSize knobSize(46, 46);
    int space = 5, dspace = 25;
    axKnob::Events freq_evt;

    axKnob* knob = new axKnob(this,
                              axRect(axPoint(15, 156), knobSize),
                              axKnob::Events(GetOnFilterFrequency()),
                              knob_info,
                              0,
                              1.0);

    axKnob* res = new axKnob(this,
                             axRect(knob->GetNextPosRight(space), knobSize),
                             axKnob::Events(GetOnFilterRes()),
                             knob_info,
                             0,
                             0.00707);
    
    axKnob* env = new axKnob(this,
                             axRect(res->GetNextPosRight(space), knobSize),
                             axKnob::Events(GetOnFilterEnvelope()),
                             knob_info,
                             0,
                             0.0);
    
    axKnob* att = new axKnob(this,
                             axRect(env->GetNextPosRight(dspace), knobSize),
                             axKnob::Events(GetOnAttack()),
                             knob_info,
                             0,
                             0.0);
    
    axKnob* dec = new axKnob(this,
                             axRect(att->GetNextPosRight(space), knobSize),
                             axKnob::Events(GetOnDecay()),
                             knob_info,
                             0,
                             1.0);
    
    axKnob* pitch = new axKnob(this,
                               axRect(dec->GetNextPosRight(dspace), knobSize),
                               axKnob::Events(GetOnTuning()),
                               knob_info,
                               0,
                               0.5);
    
    axKnob* env_pitch = new axKnob(this,
                                   axRect(pitch->GetNextPosRight(space),
                                          knobSize),
                                   axKnob::Events(GetOnTuningEnv()),
                                   knob_info,
                                   0,
                                   0.0);
    axKnob* pan = new axKnob(this,
                             axRect(env_pitch->GetNextPosRight(dspace),
                                    knobSize),
                             axKnob::Events(),
                             knob_info,
                             0,
                             0.5);
    
    axKnob* gain = new axKnob(this,
                              axRect(pan->GetNextPosRight(space),
                                     knobSize),
                              axKnob::Events(GetOnGain()),
                              knob_info,
                              0,
                              0.5);
}

void SynthControl::OnOpenFile(const axButton::Msg& msg)
{
    string f;
    if ((f = GetApp()->OpenFileDialog()) != "")
    {
//        _audio->GetAudioBufferFromTrack(_trackNum)->OpenSoundFile(f);
//        _wave->SetAudioBuffer(_audio->GetAudioBufferFromTrack(_trackNum));
        //_wave->SetBufferPtr(_audio->GetAudioBufferFromTrack(_trackNum));
    }
}

void SynthControl::OnFilterFrequency(const axKnob::Msg& msg)
{
//    axFloat freq = msg.GetValue() * (20000.0 - 20.0) + 20.0;
//    _audio->GetAudioTrack(_trackNum)->SetFilterFrequency(freq);
}

void SynthControl::OnFilterRes(const axKnob::Msg& msg)
{
//    axFloat q = msg.GetValue() * 5.0;
//    _audio->GetAudioTrack(_trackNum)->SetFilterRes(q);
}

void SynthControl::OnFilterEnvelope(const axKnob::Msg& msg)
{
//    _audio->GetAudioTrack(_trackNum)->SetFilterFrequencyEnvelopeAmount(msg.GetValue());
}

void SynthControl::OnAttack(const axKnob::Msg& msg)
{
//    _audio->GetAudioTrack(_trackNum)->SetAttack(msg.GetValue());
}

void SynthControl::OnDecay(const axKnob::Msg& msg)
{
//    _audio->GetAudioTrack(_trackNum)->SetDecay(msg.GetValue());
}

void SynthControl::OnTuning(const axKnob::Msg& msg)
{
//    axFloat value = (msg.GetValue() * 2.0);
//    _audio->GetAudioTrack(_trackNum)->SetSpeed(value);
}

void SynthControl::OnTuningEnv(const axKnob::Msg& msg)
{
//    _audio->GetAudioTrack(_trackNum)->SetPitchEnvelopeAmount(msg.GetValue());
}


void SynthControl::OnGain(const axKnob::Msg& msg)
{
//    _audio->GetAudioTrack(_trackNum)->SetGain(msg.GetValue());
}

void SynthControl::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.1, 0.1, 0.1), 1.0);
    gc->DrawRectangle(rect0);
    
    
    // Filter.
    gc->SetColor(axColor(0.2));
    gc->DrawRectangle(axRect(10, 120, 157, 90));
    
    // Env.
    gc->DrawRectangle(axRect(183, 120, 105, 90));
    
    // Pitch.
    gc->DrawRectangle(axRect(305, 120, 105, 90));
    
    // Master.
    gc->DrawRectangle(axRect(427, 120, 105, 90));
    
    gc->SetColor(0.15);
    gc->DrawRectangle(axRect(10, 120, 157, 15));
    
    // Env.
    gc->DrawRectangle(axRect(183, 120, 105, 15));
    
    // Pitch.
    gc->DrawRectangle(axRect(305, 120, 105, 15));
    
    // Master.
    gc->DrawRectangle(axRect(427, 120, 105, 15));
    
    gc->SetColor(axColor(0.5));
    gc->DrawStringAlignedCenter(*_font, "FILTER", axRect(10, 120, 157, 15));
    
    // Env.
    gc->DrawStringAlignedCenter(*_font, "ENVELOPE", axRect(183, 120, 105, 15));
    
    // Pitch.
    gc->DrawStringAlignedCenter(*_font, "PITCH", axRect(305, 120, 105, 15));
    
    // Master.
    gc->DrawStringAlignedCenter(*_font, "MASTER", axRect(427, 120, 105, 15));
    
    
    
    axSize labelSize(46, 14);
    axRect labelRect(axPoint(15, 138), labelSize);
    int space = 5, dspace = 25;
    
    gc->SetColor(axColor(0.7));
    gc->DrawStringAlignedCenter(*_font, "Freq", labelRect);
    
    labelRect.position.x = labelRect.GetTopRight().x + space;
    gc->DrawStringAlignedCenter(*_font, "Res", labelRect);
    
    labelRect.position.x = labelRect.GetTopRight().x + space;
    gc->DrawStringAlignedCenter(*_font, "Env", labelRect);
    
    
    
    labelRect.position.x = labelRect.GetTopRight().x + dspace;
    gc->DrawStringAlignedCenter(*_font, "Pitch", labelRect);
    
    labelRect.position.x = labelRect.GetTopRight().x + space;
    gc->DrawStringAlignedCenter(*_font, "Env", labelRect);
    
    
    
    labelRect.position.x = labelRect.GetTopRight().x + dspace;
    gc->DrawStringAlignedCenter(*_font, "Attack", labelRect);
    
    labelRect.position.x = labelRect.GetTopRight().x + space;
    gc->DrawStringAlignedCenter(*_font, "Decay", labelRect);
    
    
    
    labelRect.position.x = labelRect.GetTopRight().x + dspace;
    gc->DrawStringAlignedCenter(*_font, "Pan", labelRect);
    
    labelRect.position.x = labelRect.GetTopRight().x + space;
    gc->DrawStringAlignedCenter(*_font, "Volume", labelRect);
}


//-----------------------------------------------------------------------------
DrumPad::DrumPad(axWindow* parent,
                 const axRect& rect, const int& track_id) :
axPanel(parent, rect)
{
    _trackName = "";
//    _clickEvent = click_evt;
    _trackId = track_id;
    _bgImg = new axImage("/Users/alexarse/Project/axLib/axProjects/MidiSequencer/pad.png");
    _selected = false;
    
    _font = new axFont(0);
    
}

void DrumPad::SetTrackName(const string& name)
{
    _trackName = name;
}

void DrumPad::SetSelected()
{
    if (_selected == false)
    {
        _selected = true;
        Update();
    }
    
}
void DrumPad::SetUnselected()
{
    if (_selected == true)
    {
        _selected = false;
        Update();
    }
}

void DrumPad::OnMouseLeftDown(const axPoint& pos)
{
//    if (_clickEvent)
//    {
//        DrumPadMsg msg(this, _trackId);
//        _clickEvent(msg);
//    }
}

void DrumPad::OnMouseEnter()
{
    _highlight = true;
    Update();
}

void DrumPad::OnMouseLeave()
{
    _highlight = false;
    Update();
}
//
//void SynthControl::OnOpenFile(const axButtonMsg& msg)
//{
//	// cout << "FILE" << endl;
//	// string f = OpenFileDialog("/home/alexarse/Desktop/axLib/axProjects/FileDialog/main");
//}

void DrumPad::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    
    //    gc->DrawImage(_bgImg, axPoint(0, 0));
    
    if (_selected)
    {
        gc->SetColor(axColor(0.95, 0.4, 0.08, 1.0));
    }
    else
    {
        if (_highlight)
        {
            gc->SetColor(axColor(0.6, 0.6, 0.6, 1.0));
            
        }
        else
        {
            gc->SetColor(axColor(0.4, 0.4, 0.4, 1.0));
        }
    }
    
    
    gc->DrawRectangle(rect);
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    //	gc->SetFontSize(12);
    gc->DrawStringAlignedCenter(*_font, _trackName, axRect(0, 0, rect.size.x, 20));
    
    gc->SetColor(axColor(0.0));
    
    gc->DrawRectangleContour(axRect(1, 0, rect.size.x - 1, rect.size.y - 1), 2.0);
    gc->DrawRectangleContour(rect);
}



DrumSampler::DrumSampler(axWindow* parent, const axRect& rect) :
axPanel(parent, rect)
{
    //_topBar = new axImage("/Users/alexarse/Project/axLib/axProjects/MidiSequencer/tool.png");
    
//    _audio = static_cast<AudioMidiSeq*>(audio);
//    cout << "NUMBER OF AUDIO TRACK " << _audio->GetNumberOfTrack() << endl;
    
    
//    _synths.resize(_audio->GetNumberOfTrack());
    _synths.resize(8);
    _pads.resize(8);
    
    int x = 10;
    int pad_size = ((rect.size.x - 20) - 5.0 * (6.0)) / double(_pads.size());
    
//    axEvtFunction(DrumPadMsg) padClickFct(GetOnDrumPadClick());
    
    
    for (int i = 0; i < 8; i++)
    {
        _pads[i] = new DrumPad(this, axRect(x, 25, pad_size, 60),
                               i);
        x += pad_size + 5;
//        _pads[i]->SetTrackName(_audio->GetAudioTrack(i)->GetTrackName());
        _pads[i]->SetTrackName("Name");
        //
    }
    
    
    for (int i = 0; i < _synths.size(); i++)
    {
        _synths[i] = new SynthControl(this,
                                      axRect(7, 93, rect.size.x - 14, 220/* rect.size.y - 80*/),
                                      "", i);
//        _synths[i] = new SynthControl(this,
//                                      axRect(7, 93, rect.size.x - 14, 220/* rect.size.y - 80*/),
//                                      _audio->GetSoundPath(i), i);
        
        _synths[i]->Hide();
    }
    
    _synths[0]->Show();
    _pads[0]->SetSelected();
}

void DrumSampler::OnDrumPadClick(const DrumPadMsg& msg)
{
    cout << "Drum pad click " << msg.GetMsg() << endl;
    for (int i = 0; i < _synths.size(); i++)
    {
        //if (_synths[i]->IsShown())
        //{
        _synths[i]->Hide();
        _pads[i]->SetUnselected();
        //}
    }
    
    _synths[msg.GetMsg()]->Show();
    _pads[msg.GetMsg()]->SetSelected();
    
}

void DrumSampler::OnPaint()
{    
    axGC* gc = GetGC();
    //BlockDrawing();
    
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.2, 0.2, 0.2), 1.0);
    gc->DrawRectangle(rect0);
    
    gc->DrawRectangleColorFade(axRect(0, 0, rect0.size.x, 20),
                               axColor(0.15, 0.15, 0.15),
                               axColor(0.1, 0.1, 0.1));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(axRect(0, 0, rect0.size.x, 20));
    
    gc->SetColor(axColor(0.65, 0.65, 0.65));
    axFont font(0);
    font.SetFontSize(10);
    gc->DrawString(font, "SAMPLES", axPoint(10, 2));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
}