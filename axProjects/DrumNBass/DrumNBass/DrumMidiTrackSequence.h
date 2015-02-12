////
////  DrumMidiTrackSquence.h
////  DrumNBass
////
////  Created by Alexandre Arsenault on 2015-02-06.
////  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
////
//
//#ifndef __DrumNBass__DrumMidiTrackSquence__
//#define __DrumNBass__DrumMidiTrackSquence__
//
//#include "axLib.h"
//#include "DrumUtils.h"
//
//class DrumMidiTrackSequence: public axPanel
//{
//public:
//    DrumMidiTrackSequence(axWindow* parent,
//                          const axRect& rect,
//                          int num);
//    
//    void SetNumberOfSubTrack(const int& n)
//    {
//        _nSubTrack = n;
//        Update();
//    }
//    
//    void SetColorSelection(const ColorChoice& color);
//    
////    void SetPreset(TrackInfo* info);
//    
//    void SetVelocity(const int& index,
//                     const int& subtrack,
//                     const float& value);
//    
//private:
//    std::vector<MidiNoteParams> _notes;
//    
//    
//    int _nbars, // Number of bars.
//    _highLightIndex, // Mouse over index.
//    _track_number, // To call audio functions.
//    _nSubTrack, // Number of subtracks.
//    _selectedSeparationNumber; // Index of subtrack selected.
//    
//    // Color probability chosen.
//    ColorChoice _choice;
//    
//    // Images to put over rectangle.
//    axImage* _bgImg, *_hightlighImg;
//    
//    // Background colors when no notes on.
//    axColor _bgColors[4];
//    
//    // Notes color.
//    axColor _colorChoice[CHOICE_NUM];
//    
//    // Probability value associated with color.
//    float _probabilityValues[CHOICE_NUM];
//    
//    // Colors of the background order in sequence.
//    std::vector<axColor*> _sequenceColors;
//    
//    // Rectangles of the background order in sequence.
//    std::vector<axRect> _gridRects;
//    
////    AudioMidiSeq* _audio;
//    
//    // Events.
//    virtual void OnPaint();
//    virtual void OnMouseMotion(const axPoint& mousePos);
//    virtual void OnMouseLeftDown(const axPoint& mousePos);
//    virtual void OnMouseLeave();
//};
//
//#endif /* defined(__DrumNBass__DrumMidiTrackSquence__) */
