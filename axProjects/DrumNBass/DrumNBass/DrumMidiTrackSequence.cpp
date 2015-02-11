//
//  DrumMidiTrackSequence.cpp
//  DrumNBass
//
//  Created by Alexandre Arsenault on 2015-02-06.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "DrumMidiTrackSequence.h"


DrumMidiTrackSequence::DrumMidiTrackSequence(axWindow* parent,
                                           const axRect& rect,
                                           int num):
axPanel(parent, rect),
_track_number(num)
{
//    _audio = static_cast<AudioMidiSeq*>(audio);
    
    _nbars = 16;
    _highLightIndex = -1;
    _notes.resize(_nbars);
    _nSubTrack = 1;
    _selectedSeparationNumber = 0;
    
    _bgImg = new axImage("/Users/alexarse/Project/axLib/axProjects/MidiSequencer/btn.png");
    _hightlighImg = new axImage("/Users/alexarse/Project/axLib/axProjects/MidiSequencer/btnhigh.png");
    
    _colorChoice[CHOICE_RED] = axColor(0.8, 0.0, 0.0);
    _colorChoice[CHOICE_GREEN] = axColor(0.0, 0.8, 0.0);
    _colorChoice[CHOICE_BLUE] = axColor(0.0, 0.0, 0.8);
    
    _probabilityValues[CHOICE_RED] = 1.0;
    _probabilityValues[CHOICE_GREEN] = 0.5;
    _probabilityValues[CHOICE_BLUE] = 0.2;
    
    _bgColors[0] = axColor(0.12, 0.12, 0.12);
    _bgColors[1] = axColor(0.07, 0.07, 0.07);
    
    _bgColors[2] = axColor(0.15, 0.15, 0.15);
    _bgColors[3] = axColor(0.1, 0.1, 0.1);
    
    bool four_bar = false;
    _sequenceColors.resize(_nbars);
    for (int i = 0; i < _nbars; i++)
    {
        if (i % 4 == 0)
        {
            four_bar = !four_bar;
        }
        
        if (i % 2 == 0)
        {
            if (four_bar)
            {
                _sequenceColors[i] = &_bgColors[3];
            }
            else
            {
                _sequenceColors[i] = &_bgColors[0];
            }
        }
        else
        {
            if (four_bar)
            {
                _sequenceColors[i] = &_bgColors[3];
            }
            else
            {
                _sequenceColors[i] = &_bgColors[0];
            }
        }
    }
    
    _gridRects.resize(_nbars);
    double sizex_over_nbars = rect.size.x / double(_nbars);
    for (int i = 0; i < _nbars; i++)
    {
        int x_left = i * sizex_over_nbars;
        int x_right = (i + 1) * sizex_over_nbars;
        _gridRects[i] = axRect(x_left, 0, x_right - x_left, rect.size.y);
    }
}

void DrumMidiTrackSequence::SetColorSelection(const ColorChoice& color)
{
    _choice = color;
}

void DrumMidiTrackSequence::SetVelocity(const int& index,
                                       const int& subtrack,
                                       const float& value)
{
    if (_notes[index].velocities[subtrack] != value)
    {
        _notes[index].velocities[subtrack] = value;
        Update();
    }
    
}

//void DrumMidiTrackSequence::SetPreset(TrackInfo* info)
//{
//    _nSubTrack = info->nSubTrack;
//    
//    for(int j = 0; j < 3; j++)
//    {
//        for(int i = 0; i < 16; i++)
//        {
//            if(info->notes[0][i] || info->notes[1][i] || info->notes[2][i])
//            {
//                _notes[i].active = true;
//            }
//            
//            _notes[i].actives[j] = info->notes[j][i];
//            
//            if (info->probability[j][i] == _probabilityValues[CHOICE_RED])
//            {
//                _notes[i].colors[j] = _colorChoice[CHOICE_RED];
//            }
//            else if (info->probability[j][i] == _probabilityValues[CHOICE_GREEN])
//            {
//                _notes[i].colors[j] = _colorChoice[CHOICE_GREEN];
//            }
//            else if (info->probability[j][i] == _probabilityValues[CHOICE_BLUE])
//            {
//                _notes[i].colors[j] = _colorChoice[CHOICE_BLUE];
//            }
//        }
//    }
//    
//    Update();
//}

void DrumMidiTrackSequence::OnMouseMotion(const axPoint& mousePos)
{
    axPoint pos = mousePos - GetAbsoluteRect().position;
    
    int index = pos.x / double(GetRect().size.x) * _nbars;
    int selected = (pos.y / double(GetRect().size.y)) * _nSubTrack;
    
    if(index != _highLightIndex ||
       selected != _selectedSeparationNumber)
    {
        _highLightIndex = index;
        _selectedSeparationNumber = selected;
        Update();
    }
}

void DrumMidiTrackSequence::OnMouseLeftDown(const axPoint& mousePos)
{
    if(_highLightIndex != -1)
    {
        _notes[_highLightIndex].active = true;
        
        // Note is already active, needs to be turn off.
        if(_notes[_highLightIndex].actives[_selectedSeparationNumber] == true)
        {
            _notes[_highLightIndex].actives[_selectedSeparationNumber] = false;
            
            // AUDIO CALL.
//            _audio->SetMidiNote(_track_number,
//                                _selectedSeparationNumber,
//                                _highLightIndex, false);
        }
        // Note needs to be turn on.
        else
        {
            _notes[_highLightIndex].actives[_selectedSeparationNumber] = true;
            _notes[_highLightIndex].colors[_selectedSeparationNumber] = _colorChoice[_choice];
            
            // AUDIO CALL.
//            _audio->SetMidiNote(_track_number, _selectedSeparationNumber, _highLightIndex, true);
//            _audio->SetProbability(_track_number,
//                                   _selectedSeparationNumber,
//                                   _highLightIndex,
//                                   _probabilityValues[_choice]);
        }
        Update();
    }
}

void DrumMidiTrackSequence::OnMouseLeave()
{
    if(_highLightIndex != -1)
    {
        _highLightIndex = -1;
        Update();
    }
}

void DrumMidiTrackSequence::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    int height_y = rect0.size.y / double(_nSubTrack);
    
    // Draw grid.
    for(int i = 0; i < _nbars; i++)
    {
        gc->SetColor(*_sequenceColors[i]);
        gc->DrawRectangle(_gridRects[i]);
        
        // Draw notes.
        for (int k = 0; k < _nSubTrack; k++)
        {
            if (_notes[i].actives[k] == true)
            {
                int pos_y = k * rect0.size.y / double(_nSubTrack);
                int size_y = rect0.size.y / double(_nSubTrack);
                //cout << "vel : " << _notes[i].velocities[k] << endl;
                float alpha = _notes[i].velocities[k] * 0.8 + 0.2;
                gc->SetColor(_notes[i].colors[k], alpha);
                gc->DrawRectangle(axRect(_gridRects[i].position.x,
                                         pos_y, _gridRects[i].size.x, size_y));
            }
        }
        
        // Draw background image.
        gc->DrawImageResize(_bgImg,
                            _gridRects[i].position,
                            _gridRects[i].size, 1.0);
        
        // Highligted note.
        if (i == _highLightIndex)
        {
            int y_top = height_y * _selectedSeparationNumber;
            int y_bot = height_y * (1.0 + _selectedSeparationNumber);
            gc->DrawImageResize(_hightlighImg,
                                axPoint(_gridRects[i].position.x, y_top),
                                axSize(_gridRects[i].size.x, y_bot - y_top), 1.0);
        }
        
        // Draw contour.
        gc->SetColor(axColor(0.3, 0.3, 0.3), 1.0);
        gc->DrawLine(axPoint(_gridRects[i].position.x, 0), 
                     axPoint(_gridRects[i].position.x, rect0.size.y));
    }
    
    // Draw separation;
    gc->SetColor(axColor(0.3, 0.3, 0.3), 1.0);
    int y_pos = height_y;
    
    for(int i = 0; i < _nSubTrack; i++)
    {
        gc->DrawLine(axPoint(0.0, y_pos), axPoint(rect0.size.x, y_pos));
        y_pos += height_y;
    }
    
    //gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
}
