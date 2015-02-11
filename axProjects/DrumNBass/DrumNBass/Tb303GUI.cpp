//
//  Tb303GUI.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-06.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "Tb303GUI.h"

/*******************************************************************************
 * MyLED.
 ******************************************************************************/
MyLED::MyLED(axWindow* parent,
             const axRect& rect) :
axPanel(parent, rect)
{
    
    _ledImg = new axImage(axApp::GetAppPath() +
                          "resources/images/axLED_9x9.png");
    _imgIndex = 0;
}

void MyLED::SetActive(const bool& on)
{
    if(on)
    {
        _imgIndex = 1;
    }
    else
    {
        _imgIndex = 0;
    }
    
    Update();
}

void MyLED::SetOff()
{
    _imgIndex = 0;
    Update();
}

void MyLED::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect = axRect(axPoint(0, 0), GetRect().size);
    
    
    gc->DrawPartOfImage(_ledImg,
                        axPoint(0, _imgIndex * 9),
                        axSize(9, 9),
                        axPoint(0, 0));
}

/*******************************************************************************
 * MyNumberPanel.
 ******************************************************************************/
MyNumberPanel::MyNumberPanel(axWindow* parent,
                             const axPoint& pos) :
axPanel(parent, axRect(pos, axSize(28, 15))),
_number(1)
{
    
}

void MyNumberPanel::SetNumber(const int& num)
{
    _number = axClamp<int>(num, 0, 99);
    Update();
}

int MyNumberPanel::GetNumber() const
{
    return _number;
}

void MyNumberPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect = axRect(axPoint(0, 0), GetRect().size);
    
    gc->SetColor(axColor(0.3, 0.0, 0.0, 1.0));
    gc->DrawRectangle(rect);
    
    gc->SetColor(axColor(0.4, 0.0, 0.0, 1.0));
    axFont font(1);
    font.SetFontSize(16);
    
//    gc->SetFontType(std::string("digital-7 (mono).ttf"));
//    gc->SetFontSize(16);
    gc->DrawChar(font, '0', axPoint(5, -4));
    gc->DrawChar(font, '0', axPoint(13, -4));
    
    
    gc->SetColor(axColor(0.95, 0.0, 0.0, 1.0));
    
    if(_number > 9)
    {
        gc->DrawChar(font, std::to_string(_number)[0], axPoint(5, -4));
        
        gc->DrawChar(font, std::to_string(_number)[1], axPoint(13, -4));
    }
    else
    {
        gc->DrawChar(font, std::to_string(_number)[0], axPoint(13, -4));
    }
    
    gc->SetColor(axColor(0.4, 0.0, 0.0, 1.0));
    gc->DrawRectangleContour(rect.GetInteriorRect(axPoint(1, 1)));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0, 1.0));
    gc->DrawRectangleContour(rect);
    
}

/*******************************************************************************
 * MyButton.
 ******************************************************************************/
MyButton::MyButton(axWindow* parent,
                   const axRect& rect,
                   const axButton::Events& events,
                   const axButton::Info& info,
                   const axPoint& led_delta_pos):
axButton(parent, rect, axButton::Events(GetOnClickButton()), info),
_led(nullptr),
_active(false)
{
    if(events.button_click)
    {
        AddConnection(2, events.button_click);
    }
    
    
//    _led = new MyLED(parent, rect.position);
    _led = new MyLED(parent, axRect(rect.position  + led_delta_pos,
                                    axSize(9, 9)));
}

void MyButton::SetLed(MyLED* led)
{
    _led = led;
}

void MyButton::SetActive(const bool& on)
{
    _led->SetActive(on);
}

void MyButton::SetOff()
{
    _led->SetActive(false);
}

void MyButton::OnClickButton(const axButton::Msg& msg)
{
    if(_active)
    {
        _led->SetActive(false);
    }
    else
    {
        _led->SetActive(true);
    }
    
    _active = !_active;
    
    PushEvent(2, new axButton::Msg(msg));
}

MyButton::MyButtonBuilder::MyButtonBuilder(axWindow* parent,
                                           const axSize& size,
                                           const axButton::Info& info,
                                           const axPoint& led_delta_pos,
                                           const int& delta):
_parent(parent),
_size(size),
_info(info),
_delta(led_delta_pos),
_xDelta(delta),
_pastBtn(nullptr)
{
    
}

void MyButton::MyButtonBuilder::SetDelta(const axPoint& delta)
{
    _delta = delta;
}

MyButton* MyButton::MyButtonBuilder::Create(const axPoint& pos,
                                            const axEventFunction& evt)
{
    
    return _pastBtn = new MyButton(_parent, axRect(pos, _size), axButton::Events(evt),
                                   _info, _delta);
}

MyButton* MyButton::MyButtonBuilder::Create(const axEventFunction& evt)
{
    return _pastBtn = new MyButton(_parent,
                                   axRect(_pastBtn->GetNextPosRight(_xDelta),
                                          _size), axButton::Events(evt),
                                   _info, _delta);
}

/*******************************************************************************
 * MyPreference.
 ******************************************************************************/
MyPreference::MyPreference(const axRect& rect) :
axPanel(3, nullptr, rect)
{
    
}

void MyPreference::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect = axRect(axPoint(0, 0), GetRect().size);
    
    
    gc->SetColor(axColor("#9B9A9A"));
    gc->DrawRectangle(rect);
    
    gc->SetColor(axColor(0.0, 0.0, 0.0));
    axFont font(0);
    gc->DrawString(font, std::string("Audio"), axPoint(20, 20));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0));
    gc->DrawRectangleContour(rect);
}

/*******************************************************************************
 * MyProject.
 ******************************************************************************/
MyProject::MyProject(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    std::string app_path = axApp::GetAppPath();
    std::string images_path(app_path + "resources/images/");
    
    _bgImg = new axImage(images_path + std::string("tb303Small.png"));
    
    axButton::Info btn_info(axColor(0.4, 0.4, 0.4, 1.0),
                          axColor(0.5, 0.5, 0.5, 1.0),
                          axColor(0.3, 0.3, 0.3, 1.0),
                          axColor(0.4, 0.4, 0.4, 1.0),
                          axColor(0.0, 0.0, 0.0, 1.0),
                          axColor(0.0, 0.0, 0.0, 1.0));
    
    axButton::Info btn_info_dark(axColor(0.2, 0.2, 0.2, 1.0),
                               axColor(0.3, 0.3, 0.3, 1.0),
                               axColor(0.1, 0.1, 0.1, 1.0),
                               axColor(0.2, 0.2, 0.2, 1.0),
                               axColor(0.0, 0.0, 0.0, 1.0),
                               axColor(0.0, 0.0, 0.0, 1.0));
    
    MyButton::MyButtonBuilder btnBuilder(this, axSize(18, 36),
                                         btn_info, axPoint(4, -15), 22);
    
    _btns.resize(NUM_OF_BUTTONS);
    
    _btns[NOTE_C0] = btnBuilder.Create(axPoint(49, 167), GetOnNoteClick());
    _btns[NOTE_C0]->SetActive(true);
    _btns[NOTE_D] = btnBuilder.Create(GetOnNoteClick());
    _btns[NOTE_E] = btnBuilder.Create(GetOnNoteClick());
    
//    btnBuilder.SetDelta(24);
    _btns[NOTE_F] = btnBuilder.Create(GetOnNoteClick());
    _btns[NOTE_G] = btnBuilder.Create(GetOnNoteClick());
    _btns[NOTE_A] = btnBuilder.Create(GetOnNoteClick());
    _btns[NOTE_B] = btnBuilder.Create(GetOnNoteClick());
    _btns[NOTE_C1] = btnBuilder.Create(GetOnNoteClick());
    
    _btns[DOWN] = btnBuilder.Create(GetOnDownClick());
    
    
    _btns[UP] = btnBuilder.Create(GetOnUpClick());
    _btns[ACCENT] = btnBuilder.Create(GetOnButtonClick());
    _btns[SLIDE] = btnBuilder.Create(GetOnButtonClick());
    
    // Dark buttons.
    btnBuilder.SetInfo(btn_info_dark);
    btnBuilder.SetXDelta(25);
    _btns[NOTE_C0_S] = btnBuilder.Create(axPoint(68, 113), GetOnNoteClick());
    _btns[NOTE_D_S] = btnBuilder.Create(GetOnNoteClick());
    
    btnBuilder.SetXDelta(21);
    _btns[NOTE_F_S] = btnBuilder.Create(axPoint(193, 113), GetOnNoteClick());
    _btns[NOTE_G_S] = btnBuilder.Create(GetOnNoteClick());
    
//    btnBuilder.SetXDelta(21);
    _btns[NOTE_A_S] = btnBuilder.Create(GetOnNoteClick());
    
    axSize horiBtnSize(32, 18);
    
//    axButton* wave = new axButton(this,
//                                  axRect(axPoint(154, 25), axSize(32, 15)),
//                                  axEventFunction(),
//                                  btn_info);
//    
//    MyButton* pitch = new MyButton(this,
//                                   axRect(axPoint(132, 120), horiBtnSize),
//                                   axEventFunction(),
//                                   btn_info_dark,
//                                   axPoint(15, -17));
//
//    axButton* clear = new axButton(this,
//                                   axRect(axPoint(50, 120), horiBtnSize),
//                                   axEventFunction(),
//                                   btn_info);
//    
//    MyButton* run = new MyButton(this,
//                                 axRect(axPoint(46, 169), axSize(42, 35)),
//                                 axEventFunction(GetOnRunClick()),
//                                 btn_info,
//                                 axPoint(15, -17));
//    
    axButton* back = new axButton(this,
                                  axRect(axPoint(527, 120), horiBtnSize),
                                  axButton::Events(GetOnBackEditPattern()),
                                  btn_info);

    axButton* slide = new axButton(this,
                                   axRect(axPoint(484, 111), axSize(31, 15)),
                                   axEventFunction(),
                                   btn_info);
//
    axButton* next = new axButton(this,
                                  axRect(axPoint(527, 187), horiBtnSize),
                                  axButton::Events(GetOnNextEditPattern()),
                                  btn_info);

    axKnob::Info knob_info(axColor(0.3, 0.3, 0.3, 0.0),
                         axColor(0.3, 0.3, 0.3, 0.0),
                         axColor(0.3, 0.3, 0.3, 0.0),
                         128,
                         axSize(46, 46),
                         images_path + std::string("knob_dark.png"),
                         images_path + std::string("knob_dark.png"));
    
    
    knob_info.knob_size = axSize(50, 50);
    knob_info.img_path = images_path + std::string("axKnobTB303_50x50.png");
    knob_info.img_path = images_path + std::string("axKnobTB303_50x50.png");
    
    axSize knob_size(50, 50);
    
//    axKnob* speed = new axKnob(this, axRect(axPoint(15, 25), knob_size),
//                               axKnob::Events(),
//                               knob_info,
//                               0, 0.5);
//    
//    axKnob* volume = new axKnob(this, axRect(speed->GetNextPosRight(10), knob_size),
//                                axKnob::Events(GetOnVolumeChange()),
//                                knob_info, 0, 0.0);

    axKnob* f1 = new axKnob(this, axRect(axPoint(60, 25), knob_size),
                            axKnob::Events(GetOnTuningChange()),
                            knob_info, axNO_FLAG, 0.5);
    
    axKnob* freq = new axKnob(this, axRect(f1->GetNextPosRight(10), knob_size),
                              axKnob::Events(GetOnFreqChange()),
                              knob_info, axNO_FLAG, 1.0);
    
    axKnob* res = new axKnob(this, axRect(freq->GetNextPosRight(10), knob_size),
                             axKnob::Events(GetOnResChange()),
                             knob_info, axNO_FLAG, 0.0);
    
    axKnob* f4 = new axKnob(this, axRect(res->GetNextPosRight(10), knob_size),
                            axKnob::Events(),
                            knob_info, axNO_FLAG, 0.5);
    
    axKnob* f5 = new axKnob(this, axRect(f4->GetNextPosRight(10), knob_size),
                            axKnob::Events(GetOnDecayChange()),
                            knob_info, axNO_FLAG, 0.5);
    
    axKnob* f6 = new axKnob(this, axRect(f5->GetNextPosRight(10), knob_size),
                            axKnob::Events(),
                            knob_info, axNO_FLAG, 0.5);
    
    _numberPanel = new MyNumberPanel(this, axPoint(530, 165));
//
//    
//    axButton* prefBtn = new axButton(this,
//                                     axRect(axPoint(820, 10),
//                                            axSize(20, 20)),
//                                     axButton::Events(GetOnPreference()),
//                                     axBUTTON_TRANSPARENT,
//                                     "settings.png",
//                                     "",
//                                     axButton::Flags::SINGLE_IMG);
//    
//    
//    _pref = new MyPreference(axRect(640, 10, 170, 58));
//    _pref->Hide();
}

void MyProject::OnVolumeChange(const axKnob::Msg& msg)
{
    //MyAudioSynth::GetInstance()->SetVolume(msg.GetValue());
}

void MyProject::OnTuningChange(const axKnob::Msg& msg)
{
//    axRange<double> range(0.5, 2.0);
//    MyAudioSynth::GetInstance()->SetTuning(range.GetValueFromZeroToOne(msg.GetValue()));
}

void MyProject::OnWaveChoice(const axDropMenuMsg& msg)
{
//    MyAudioSynth* audio = MyAudioSynth::GetInstance();
//    std::string m = msg.GetMsg();
//    if(m == "Sine")
//    {
//        audio->SetWaveformType(axAudioWaveTable::
//                               axWaveformType::axWAVE_TYPE_SINE);
//    }
//    else if(m == "Triangle")
//    {
//        audio->SetWaveformType(axAudioWaveTable::
//                               axWaveformType::axWAVE_TYPE_TRIANGLE);
//    }
//    else if(m == "Square")
//    {
//        audio->SetWaveformType(axAudioWaveTable::
//                               axWaveformType::axWAVE_TYPE_SQUARE);
//    }
//    else if(m == "Saw")
//    {
//        audio->SetWaveformType(axAudioWaveTable::
//                               axWaveformType::axWAVE_TYPE_SAW);
//    }
}

void MyProject::OnUpClick(const axButton::Msg& msg)
{
//    MyButton* sender = static_cast<MyButton*>(msg.GetSender());
//    //    sender->SetActive(true);
//    
//    MyAudioSynth::GetInstance()->SetNoteInfoUp(_numberPanel->GetNumber() - 1,
//                                               sender->IsActive());
}

void MyProject::OnDownClick(const axButton::Msg& msg)
{
//    MyButton* sender = static_cast<MyButton*>(msg.GetSender());
//    //    sender->SetActive(true);
//    
//    MyAudioSynth::GetInstance()->SetNoteInfoDown(_numberPanel->GetNumber() - 1,
//                                                 sender->IsActive());
}

void MyProject::OnNoteClick(const axButton::Msg& msg)
{
    //    std::cout << "Note click : " << msg.GetSender()->GetId() << std::endl;
    
//    std::vector<MyButtonId> ids = { NOTE_C0, NOTE_D, NOTE_E, NOTE_F, NOTE_G,
//        NOTE_A, NOTE_B, NOTE_C1, NOTE_C0_S,
//        NOTE_D_S, NOTE_F_S, NOTE_G_S, NOTE_A_S };
//    
//    MyButtonId noteId;
//    
//    for(auto& n : ids)
//    {
//        MyButton* sender = static_cast<MyButton*>(msg.GetSender());
//        if(_btns[n] != sender)
//        {
//            _btns[n]->SetActive(false);
//        }
//        else
//        {
//            noteId = n;
//            sender->SetActive(true);
//        }
//    }
//    
//    int index = 0;
//    
//    switch(noteId)
//    {
//        case NOTE_C0: index = 0; break;
//        case NOTE_C0_S: index = 1; break;
//        case NOTE_D: index = 2; break;
//        case NOTE_D_S: index = 3; break;
//        case NOTE_E: index = 4; break;
//        case NOTE_F: index = 5; break;
//        case NOTE_F_S: index = 6; break;
//        case NOTE_G: index = 7; break;
//        case NOTE_G_S: index = 8; break;
//        case NOTE_A: index = 9; break;
//        case NOTE_A_S: index = 10; break;
//        case NOTE_B: index = 11; break;
//        case NOTE_C1: index = 12; break;
//    }
//    
//    
//    std::cout << "NOTE CLICK : " << _numberPanel->GetNumber() - 1 << " " << index << std::endl;
//    MyAudioSynth::GetInstance()->SetNoteInfoNote(_numberPanel->GetNumber() - 1,
//                                                 index);
}

void MyProject::OnRunClick(const axButton::Msg& msg)
{
//    if(static_cast<MyButton*>(msg.GetSender())->IsActive())
//    {
//        MyAudioSynth::GetInstance()->StartAudio();
//    }
//    else
//    {
//        MyAudioSynth::GetInstance()->StopAudio();
//    }
}

void MyProject::OnButtonClick(const axButton::Msg& msg)
{
//    std::cout << "Button click." << std::endl;
}

void MyProject::OnDecayChange(const axKnob::Msg& msg)
{
//    MyAudioSynth::GetInstance()->SetDecay(msg.GetValue());
}

void MyProject::OnFreqChange(const axKnob::Msg& msg)
{
//    axRange<double> freqRange(100.0, 20000.0);
//    double f = freqRange.GetValueFromZeroToOne(msg.GetValue());
//    
//    MyAudioSynth::GetInstance()->SetFilterFreq(f);
}

void MyProject::OnResChange(const axKnob::Msg& msg)
{
//    axRange<double> qRange(0.707, 10.0);
//    double r = qRange.GetValueFromZeroToOne(msg.GetValue());
//    
//    MyAudioSynth::GetInstance()->SetFilterRes(r);
}

void MyProject::UpdateParameters(const int& index)
{
    std::vector<MyButtonId> ids = { NOTE_C0, NOTE_D, NOTE_E, NOTE_F, NOTE_G,
        NOTE_A, NOTE_B, NOTE_C1, NOTE_C0_S,
        NOTE_D_S, NOTE_F_S, NOTE_G_S, NOTE_A_S };
    
    for(auto& n : ids)
    {
        _btns[n]->SetActive(false);
    }
    
    //const MyAudioSynth::Note* notes = MyAudioSynth::GetInstance()->GetNotes();
    
//    switch(notes[index].note)
//    {
//        case 0: _btns[NOTE_C0]->SetActive(true); break;
//        case 1: _btns[NOTE_C0_S]->SetActive(true); break;
//        case 2: _btns[NOTE_D]->SetActive(true); break;
//        case 3: _btns[NOTE_D_S]->SetActive(true); break;
//        case 4: _btns[NOTE_E]->SetActive(true); break;
//        case 5: _btns[NOTE_F]->SetActive(true); break;
//        case 6: _btns[NOTE_F_S]->SetActive(true); break;
//        case 7: _btns[NOTE_G]->SetActive(true); break;
//        case 8: _btns[NOTE_G_S]->SetActive(true); break;
//        case 9: _btns[NOTE_A]->SetActive(true); break;
//        case 10: _btns[NOTE_A_S]->SetActive(true); break;
//        case 11: _btns[NOTE_B]->SetActive(true); break;
//        case 12: _btns[NOTE_C1]->SetActive(true); break;
//    }
    
//    _btns[DOWN]->SetActive(notes[index].down);
//    _btns[UP]->SetActive(notes[index].up);
//    _btns[ACCENT]->SetActive(notes[index].accent);
//    _btns[SLIDE]->SetActive(notes[index].slide);
    
    Update();
}

void MyProject::OnNextEditPattern(const axButton::Msg& msg)
{
    int num = _numberPanel->GetNumber();
    ++num;
    if(num > 16)
    {
        num = 1;
    }
    
    _numberPanel->SetNumber(num);
    
    UpdateParameters(num - 1);
}

void MyProject::OnBackEditPattern(const axButton::Msg& msg)
{
    int num = _numberPanel->GetNumber();
    --num;
    if(num < 1)
    {
        num = 16;
    }
    
    _numberPanel->SetNumber(num);
    
    UpdateParameters(num - 1);
}

void MyProject::OnPreference(const axButton::Msg& msg)
{
    if(_pref->IsShown())
    {
        _pref->Hide();
    }
    else
    {
        _pref->Show();
    }
}

void MyProject::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect0(axPoint(0, 0), GetRect().size);
    
    gc->SetColor(axColor(0.4, 0.4, 0.4), 1.0);
    gc->DrawRectangle(rect0);
    
    gc->DrawImage(_bgImg, axPoint(0, 0));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
    
}

//void axMain::MainEntryPoint(axApp* app)
//{
//    MyAudioSynth* audio = MyAudioSynth::GetInstance();
//    
//    MyProject* myProject = new MyProject(nullptr, axRect(0, 0, 856, 273));
//    
//    audio->InitAudio();
//    //    audio->StartAudio();
//}