////
////  DrumMidiSequencer.h
////  DrumNBass
////
////  Created by Alexandre Arsenault on 2015-02-05.
////  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
////
//
//#ifndef __DrumNBass__DrumMidiSequencer__
//#define __DrumNBass__DrumMidiSequencer__
//
//#include "axLib.h"
//#include "DrumUtils.h"
//
//class DrumInstrumentPreset;
//class LineSelection;
//
//#include "DrumMidiTrack.h"
//
//class DrumMidiSequencer: public axPanel
//{
//public:
//    DrumMidiSequencer(axWindow* parent, const axRect& rect);
//    
//    void AddNewTrack(const std::string& trackName, const int& num);
//    
////    void SetTrackResizeFct(axEvtFunction(int) fct)
////    {
////        _track_resize_parent_fct = fct;
////    }
//    
////    void SetPreset(DrumMachinePreset* preset);
//    
//    
////    axEVENT(int, OnChangeTrackHeight);
//    
//
//    
//private:
//    std::vector<DrumMidiTrack*> _midiTracks;
//    ColorChoice _choice;
//    LineSelection* _lineSelection;
//    
//    axPoint GetNextTrackPosition() const;
//    
//    axSize _trackSize;
//    
//    std::vector<axButton*> _btns;
//    
////    axImage* _side_img;
//    
////    axEvtFunction(int) _track_resize_parent_fct;
//    
//    axEVENT_ACCESSOR(DrumMidiTrack::Msg, OnChangeTrackHeight);
//    void OnChangeTrackHeight(const DrumMidiTrack::Msg& msg);
//    
//    axEVENT_ACCESSOR(axButton::Msg, OnColorChange);
//    void OnColorChange(const axButton::Msg& msg);
//
//    // axImage* _bgImg;
//    
//    virtual void OnPaint();
//};
//
//#endif /* defined(__DrumNBass__DrumMidiSequencer__) */
