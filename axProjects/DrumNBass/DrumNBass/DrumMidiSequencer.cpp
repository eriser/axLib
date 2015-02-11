//
//  DrumDrumMidiSequencer.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-05.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "DrumMidiSequencer.h"
#include "LineSelection.h"
//#include "DrumMidiTrack.h"

DrumMidiSequencer::DrumMidiSequencer(axWindow* parent,
                                     const axRect& rect):
// Heritage.
axPanel(parent, rect),
// Members.
_choice(CHOICE_RED)
{

    std::string app_path(axApp::GetAppPath());
    std::string btnImgPath(app_path + "resources/images/btn.png");
    
    axFlag btnFlags = axButton::Flags::IMG_RESIZE | axButton::Flags::SINGLE_IMG;
    axSize btnSize(20, 20);
    
    // Red button.
    _btns.push_back(new axButton(this,
                                 axRect(axPoint(0, 0), btnSize),
                                 axButton::Events(GetOnColorChange()),
                                 axButton::Info(axColor(0.8, 0.0, 0.0, 0.3),
                                                axColor(0.9, 0.0, 0.0),
                                                axColor(0.8, 0.0, 0.0),
                                                axColor(0.8, 0.0, 0.0),
                                                axColor(0.0, 0.0, 0.0),
                                                axColor(0.0, 0.0, 0.0)),
                                 btnImgPath,
                                 "",
                                 btnFlags,
                                 std::to_string(CHOICE_RED)));
    
    // Green button.
    _btns.push_back(new axButton(this,
                                 axRect(_btns[_btns.size()-1]->GetNextPosRight(0),
                                        btnSize),
                                 axButton::Events(GetOnColorChange()),
                                 axButton::Info(axColor(0.0, 0.8, 0.0, 0.3),
                                                axColor(0.0, 0.9, 0.0),
                                                axColor(0.0, 0.8, 0.0),
                                                axColor(0.0, 0.8, 0.0),
                                                axColor(0.0, 0.0, 0.0),
                                                axColor(0.0, 0.0, 0.0)),
                                 btnImgPath,
                                 "",
                                 btnFlags,
                                 std::to_string(CHOICE_GREEN)));
    
    // Blue button.
    _btns.push_back(new axButton(this,
                                 axRect(_btns[_btns.size()-1]->GetNextPosRight(0),
                                        btnSize),
                                 axButton::Events(GetOnColorChange()),
                                 axButton::Info(axColor(0.0, 0.0, 0.8, 0.3),
                                                axColor(0.0, 0.0, 0.9),
                                                axColor(0.0, 0.0, 0.8),
                                                axColor(0.0, 0.0, 0.8),
                                                axColor(0.0, 0.0, 0.0),
                                                axColor(0.0, 0.0, 0.0)),
                                 btnImgPath,
                                 "",
                                 btnFlags,
                                 std::to_string(CHOICE_BLUE)));
    
    _trackSize = axSize(rect.size.x, 90);
    
    _lineSelection = new LineSelection(this,
                                       axRect(60, 0, rect.size.x - 60, 20));
    
    
//    AudioMidiSeq* au = static_cast<AudioMidiSeq*>(audio);
//    for (int i = 0; i < 8; i++)
//    {
//        AddNewTrack(au->GetAudioTrack(i)->GetTrackName(), audio, i);
//    }
    
    for (int i = 0; i < 8; i++)
    {
        AddNewTrack("Name", i);
    }

}

void DrumMidiSequencer::OnColorChange(const axButton::Msg& msg)
{
    _choice = (ColorChoice)stoi(msg.GetMsg());
    
    for(DrumMidiTrack* t : _midiTracks)
    {
        t->SetColorSelection(_choice);
    }
}

void DrumMidiSequencer::AddNewTrack(const std::string& trackName, const int& num)
{
    if(_midiTracks.size() == 0)
    {
        _midiTracks.push_back(new DrumMidiTrack(this,
                                                axRect(axPoint(0, 20), _trackSize),
                                                DrumMidiTrack::Events(GetOnChangeTrackHeight()),
                                                trackName, num));
        
        SetSize(axSize(GetRect().size.x,
                       _midiTracks[_midiTracks.size() - 1]->GetBottomLeftPosition().y));
        
    }
    else
    {
        _midiTracks.push_back(new DrumMidiTrack(this,
                                            axRect(GetNextTrackPosition(),
                                                   _trackSize),
                                                DrumMidiTrack::Events(GetOnChangeTrackHeight()),
                                            trackName, num));
        
        SetSize(axSize(GetRect().size.x,
                       _midiTracks[_midiTracks.size() - 1]->GetBottomLeftPosition().y));
    }
    
//    function<void(int)> min_fct(GetOnChangeTrackHeight());
//    MidiTrackEvents evt(min_fct);
//    _midiTracks[_midiTracks.size() - 1]->SetEvent(evt);
}

//void DrumMidiSequencer::SetPreset(DrumMachinePreset* preset)
//{
//    if(preset->info != nullptr)
//    {
//        //cout << __PRETTY_FUNCTION__ << endl;
//        cout << "NB TRACKS = " << preset->nTracks << endl;
//        
//        if(preset->nTracks <= _midiTracks.size())
//        {
//            for(int i = 0; i < preset->nTracks; i++)
//            {
//                // TrackInfo& t_info(preset.info[i]);
//                _midiTracks[i]->SetPreset(&preset->info[i]);
//            }
//        }
//        else
//        {
//            cerr << "Error : Too many tracks in presets." << endl;
//        }
//    }
//    else
//    {
//        cerr << "Error : Preset is not loaded." << endl;
//    }
//}

axPoint DrumMidiSequencer::GetNextTrackPosition() const
{
//    return  axPoint(0, 0);
    return _midiTracks[_midiTracks.size() - 1]->GetBottomLeftPosition();
}

void DrumMidiSequencer::OnChangeTrackHeight(const DrumMidiTrack::Msg& msg)
{
    for(int i = 1; i < _midiTracks.size(); i++)
    {
        _midiTracks[i]->SetPosition(_midiTracks[i-1]->GetBottomLeftPosition());
    }
    
    SetSize(axSize(GetRect().size.x, 
                   _midiTracks[_midiTracks.size() - 1]->GetBottomLeftPosition().y));
    
    GetParent()->OnResize();
    
//    if(_track_resize_parent_fct)
//    {
//        _track_resize_parent_fct(msg.GetMsg());
//    }
}

void DrumMidiSequencer::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.1));
    gc->DrawRectangle(rect0);
    
    gc->SetColor(axColor::axBlackColor);
    gc->DrawRectangleContour(rect0);
}