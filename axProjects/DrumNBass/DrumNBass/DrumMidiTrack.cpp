////
////  DrumDrumMidiTrack.cpp
////  DrumNBass
////
////  Created by Alexandre Arsenault on 2015-02-05.
////  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
////
//
//#include "DrumMidiTrack.h"
//
///*******************************************************************************
// * DrumMidiTrack::Msg.
// ******************************************************************************/
//DrumMidiTrack::Msg::Msg()
//{
//    _sender = nullptr;
//}
//
//DrumMidiTrack::Msg::Msg(DrumMidiTrack* sender, const int& msg)
//{
//    _sender = sender;
//    _msg = msg;
//}
//
//DrumMidiTrack* DrumMidiTrack::Msg::GetSender() const
//{
//    return _sender;
//}
//
//int DrumMidiTrack::Msg::GetMsg() const
//{
//    return _msg;
//}
//
//axMsg* DrumMidiTrack::Msg::GetCopy()
//{
//    return new DrumMidiTrack::Msg(*this);
//}
//
///*******************************************************************************
// * DrumMidiTrack.
// ******************************************************************************/
//DrumMidiTrack::DrumMidiTrack(axWindow* parent,
//                             const axRect& rect,
//                             const DrumMidiTrack::Events& events,
//                             const std::string& trackName,
//                             int track_number):
//axPanel(parent, rect),
//_events(events),
//_track_number(track_number),
//_nSubTrack(1)
//{
//    
//    std::string app_path(axApp::GetAppPath());
//    std::string images_path(app_path + "resources/images/");
//    
//    if(_events.size_change)
//    {
//        AddConnection(Events::SIZE_CHANGE, _events.size_change);
//    }
//    
//    axButton::Info btn_info(axColor(0.17, 0.17, 0.17),
//                            axColor(0.3, 0.3, 0.3),
//                            axColor(0.2, 0.2, 0.2),
//                            axColor(0.2, 0.2, 0.2),
//                            axColor(0.0, 0.0, 0.0),
//                            axColor(0.8, 0.8, 0.8));
//    
//    int x = 10, y = 10, xDelta = 25;
//    
//    int height = 15;
//    
//    _addBtn = new axButton(this,
//                           axRect(rect.size.x - 14, 0, 14, height),
//                           axButton::Events(GetOnAddSeparation()),
//                           btn_info, images_path + "btnhigh.png", "+",
//                           axButton::Flags::SINGLE_IMG |
//                           axButton::Flags::IMG_RESIZE);
//    
//    _removeBtn = new axButton(this,
//                              axRect(rect.size.x - 14, height, 14, height),
//                              axButton::Events(GetOnRemoveSeparation()),
//                              btn_info,
//                              images_path + "btnhigh.png", "-",
//                              axButton::Flags::SINGLE_IMG |
//                              axButton::Flags::IMG_RESIZE);
//    
//    
//    // _trackName = new DrumMidiTrackName(app, this,
//    //                 axRect(0, 0, 60, 30),
//    //                 trackName);
//    
//    axButton* name = new axButton(this,
//                                  axRect(0, 0, 60, 30),
//                                  axButton::Events(GetOnMinimize()),
//                                  btn_info,
//                                  "",//images_path + "miditrackname.png",
//                                  trackName,
//                                  axButton::Flags::SINGLE_IMG);
//    
//    _trackSeq = new DrumMidiTrackSequence(this,
//                                      axRect(60, 0,
//                                             rect.size.x - 60 - 14,
//                                             30),
//                                      _track_number);
////    //
////    axEvtFunction(MultipleSliderMsg) fct(GetOnVelocity());
////    axEventFunction sfct(GetOnStandardDeviation());
//    _velocity = new DrumMidiVelocity(this, axRect(0, 30, rect.size.x, 60),
//                                     DrumMidiVelocity::Events());
//    _velocity->SetNumberOfSlider(_nSubTrack);
//    
////
////    
////    _nSubTrack = 1;
//    _trackSeq->SetNumberOfSubTrack(_nSubTrack);
//    
////    _audio->SetTrackNumberOfSection(_track_number, _nSubTrack);
////    
////    OnMinimize(axButton::Msg(name, ""));
//    
//    OnMinimize(axButton::Msg(name, ""));
//    
//}
//
////void DrumMidiTrack::SetPreset(TrackInfo* info)
////{
////    _trackSeq->SetPreset(info);
////    _velocity->SetPreset(info);
////}
////
////void DrumMidiTrack::OnVelocity(const MultipleSliderMsg& msg)
////{
////    //cout << msg.bar_index << " " << msg.index << " " << msg.value << endl;
////    _audio->SetVelocity(_track_number,
////                        msg.index,
////                        msg.bar_index,
////                        msg.value);
////    
////    //cout << "index : " << msg.index << " bar index : " << msg.bar_index << endl;
////    _trackSeq->SetVelocity(msg.bar_index, msg.index, msg.value);
////}
////
////void DrumMidiTrack::OnStandardDeviation(const axNumberBox::Msg& msg)
////{
////    // cout << "TEST : " << msg.GetValue() << endl;
////    
////    _audio->SetStandardDeviation(_track_number, msg.GetValue() * 0.5);
////    // cout << msg.bar_index << " " << msg.index << " " << msg.value << endl;
////    // _audio->SetVelocity(_track_number,
////    //                     msg.index,
////    //                     msg.bar_index,
////    //                     msg.value);
////}
////
//void DrumMidiTrack::OnAddSeparation(const axButton::Msg& msg)
//{
//    _trackSeq->SetNumberOfSubTrack(++_nSubTrack);
//    _velocity->SetNumberOfSlider(_nSubTrack);
//    //_audio->SetTrackNumberOfSection(_track_number, _nSubTrack);
//}
//
//void DrumMidiTrack::OnRemoveSeparation(const axButton::Msg& msg)
//{
//    _nSubTrack--;
//    if(_nSubTrack <= 0)
//    {
//        _nSubTrack = 1;
//    }
//    _trackSeq->SetNumberOfSubTrack(_nSubTrack);
//    _velocity->SetNumberOfSlider(_nSubTrack);
////    _audio->SetTrackNumberOfSection(_track_number, _nSubTrack);
//}
//
//void DrumMidiTrack::OnMinimize(const axButton::Msg& msg)
//{
//    if(_velocity->IsShown())
//    {
//        _velocity->Hide();
//        SetSize(GetRect().size - axSize(0, 60));
//        
//        PushEvent(Events::SIZE_CHANGE, new DrumMidiTrack::Msg(this, 0));
//    }
//    else
//    {
//        _velocity->Show();
//        SetSize(GetRect().size + axSize(0, 60));
//        
//        PushEvent(Events::SIZE_CHANGE, new DrumMidiTrack::Msg(this, 0));
//    }
//}
//
//
////
//////------------------------------------------------------------------------
////
//void DrumMidiTrack::SetColorSelection(const ColorChoice& choice)
//{
//    _trackSeq->SetColorSelection(choice);
//}
//
//void DrumMidiTrack::OnPaint()
//{
//    axGC* gc = GetGC();
//    axRect rect(GetRect());
//    axRect rect0(axPoint(0, 0), rect.size);
//    
//    gc->SetColor(axColor::axGreenColor);
//    gc->DrawRectangle(rect0);
//    
//    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
//    gc->DrawRectangleContour(rect0);
//}