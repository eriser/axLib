#include "MidiTrack.h"

#include <cstdlib> 

string GetCurrentAppDirectory()
{
  //char buf[1024];
  //readlink("/proc/self/exe", buf, sizeof(buf)-1);
  //string path(buf);
  //path = path.substr(0, path.find_last_of("/"));
  //path.push_back('/');

  return "";
}

void ExecApplication(const string& app_name)
{
  //pid_t p_id = fork();

  //// Child.
  //if (p_id == 0)                
  //{
  //  execl(app_name.c_str(), 0);
  //  exit(0);
  //}

  //// Failed to fork.
  //else if (p_id < 0)            
  //{
  //    cerr << "Failed to open " << app_name << endl;
  //    exit(1);
  //    // Throw exception
  //}
}

MultipleSlider::MultipleSlider(axWindow* parent,
                               const axRect& rect, 
                               const axColor& bgColor, 
                               const int& barIndex,
                               axEvtFunction(MultipleSliderMsg)& slider_fct):
      axPanel(parent, rect),
      // Members.
      _barIndex(barIndex),
      _slider_fct(slider_fct)

{
    _bgColor = bgColor;

    axSliderInfo sld_info("",//"sliderPlain.png",   
                          axSize(8, 8),
                          axColor(0.9, 0.9, 0.9), // Bg
                          axColor(1.0, 1.0, 1.0), // bg hover.
                          axColor(0.9, 0.9, 0.9), // Bg clicking.
                          axColor(0.3, 0.3, 0.3), // Slider normal
                          axColor(0.3, 0.3, 0.3), // Slider hover
                          axColor(0.4, 0.4, 0.4), // Slider clicking.
                          axColor(0.3, 0.3, 0.3), // Slider contour.
                          axColor(0.0, 0.0, 0.0), // COntour
                          axColor(0.9, 0.9, 0.9), // Back Slider
                          axColor(0.9, 0.9, 0.9), // Back Slider contour.
                          3);

    _nSlider = 3;
	double position_ratio = rect.size.x / double(_nSlider);
    int size_x = rect.size.x / double(_nSlider);

	/*double t = position_ratio * 1.0;*/

    axFlag slider_flags = axSLIDER_FLAG_VERTICAL | 
              axSLIDER_FLAG_CLICK_ANYWHERE | 
                axSLIDER_FLAG_RIGHT_ALIGN;


//    axEvtFunction(axSliderMsg) sld_fct(GetOnSlider1Move());
    
    axSliderEvents sldEvents;
    sldEvents.slider_value_change = GetOnSlider1Move();
    // function<void(const axSliderMsg&)> sld_fct(GetOnSliderMove());

	double left = position_ratio * 0.0;
	double right = position_ratio * 1.0;
	sliders[0] = new axSlider(this,
		axRect(left, 0, right - left, rect.size.y),
	              sldEvents, sld_info, slider_flags);
    //sliders[0] = new axSlider(app, this, 
    //              axRect(0, 0, size_x, rect.size.y), 
    //              axSliderEvents(sld_fct), sld_info, slider_flags);

	left = position_ratio * 1.0;
	right = position_ratio * 2.0;
    sldEvents.slider_value_change = GetOnSlider2Move();
	sliders[1] = new axSlider(this,
		axRect(left, 0, right - left, rect.size.y),
	              sldEvents, sld_info, slider_flags);
    //sliders[1] = new axSlider(app, this, 
    //              axRect(size_x, 0, size_x, rect.size.y), 
    //              axSliderEvents(sld_fct), sld_info, slider_flags);

	left = position_ratio * 2.0;
	right = position_ratio * 3.0;
    sldEvents.slider_value_change = GetOnSlider3Move();
	sliders[2] = new axSlider(this,
		axRect(left, 0, right - left, rect.size.y),
	            sldEvents, sld_info, slider_flags);
    //sliders[2] = new axSlider(app, this, 
    //            axRect(size_x * 2, 0, size_x, rect.size.y), 
    //            axSliderEvents(sld_fct), sld_info, slider_flags);

    sliders[0]->SetBackgroundAlpha(0.0);
    sliders[1]->SetBackgroundAlpha(0.0);
    sliders[2]->SetBackgroundAlpha(0.0);

    sliders[0]->SetValue(0.7);
    sliders[1]->SetValue(0.7);
    sliders[2]->SetValue(0.7);
    // sliders[2]->Hide();

    SetNumberOfSlider(2);

}

void MultipleSlider::SetValue(const int& index, 
                              const double& value)
{
    if(index >= 0 && index < 3)
    {
        sliders[index]->SetValue(value);
    }
}


void MultipleSlider::OnSlider1Move(const axSliderMsg& msg)
{
    if (_slider_fct)
    {
        MultipleSliderMsg m;
        m.index = 0;
        m.value = 1.0 - msg.GetValue();
        m.bar_index = _barIndex;

      _slider_fct(m);
    }
}

void MultipleSlider::OnSlider2Move(const axSliderMsg& msg)
{
    if (_slider_fct)
    {
        MultipleSliderMsg m;
        m.index = 1;
        m.value = 1.0 - msg.GetValue();
        m.bar_index = _barIndex;

      _slider_fct(m);
    }
}

void MultipleSlider::OnSlider3Move(const axSliderMsg& msg)
{
    if (_slider_fct)
    {
        MultipleSliderMsg m;
        m.index = 2;
        m.value = 1.0 - msg.GetValue();
        m.bar_index = _barIndex;

      _slider_fct(m);
    }
}

void MultipleSlider::SetNumberOfSlider(const int& nb)
{
  _nSlider = nb;

  if(_nSlider == 1)
  {
    axSize rSize(GetRect().size);
    int size_x = rSize.x / double(_nSlider);
    sliders[0]->ResizeSlider(axSize(size_x, rSize.y));

    sliders[1]->Hide();
    sliders[2]->Hide();
  }
  else if(_nSlider == 2)
  {
    axSize rSize(GetRect().size);
    int size_x = rSize.x / double(_nSlider);
    sliders[0]->ResizeSlider(axSize(size_x, rSize.y));
    sliders[1]->ResizeSlider(axSize(size_x, rSize.y));
    sliders[1]->SetPosition(axPoint(size_x, 0));
    sliders[1]->Show();
    sliders[2]->Hide();
  }
  else if(_nSlider == 3)
  {
    axSize rSize(GetRect().size);
    int size_x = rSize.x / double(_nSlider);
    sliders[0]->ResizeSlider(axSize(size_x, rSize.y));
    sliders[1]->ResizeSlider(axSize(size_x, rSize.y));
    sliders[1]->SetPosition(axPoint(size_x, 0));
    
    sliders[1]->Show();
    sliders[2]->Show();
  }
  Update();
}

void MultipleSlider::OnPaint()
{
  axGC* gc = GetGC();
  axRect rect(GetRect());
  axRect rect0(axPoint(0, 0), rect.size);

  // Draw background.
  gc->SetColor(_bgColor, 1.0);
  gc->DrawRectangle(rect0);

  gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
  gc->DrawRectangleContour(rect0);
}


//------------------------------------------------------------------------------
// MidiTrackSequence.
//------------------------------------------------------------------------------
MidiTrackSequence::MidiTrackSequence(axWindow* parent,
                  const axRect& rect, Audio* audio, int num):
                  axPanel(parent, rect),
                  _track_number(num)
{
	_audio = static_cast<AudioMidiSeq*>(audio);
	_nbars = 16;
	_highLightIndex = -1;
	_notes.resize(_nbars);
	_nSubTrack = 2;
	_selectedSeparationNumber = 0;

	_bgImg = new axImage("/Users/alexarse/Project/axLib/axProjects/MidiSequencer/btn.png");
	_hightlighImg = new axImage("/Users/alexarse/Project/axLib/axProjects/MidiSequencer/btnhigh.png");

	_colorChoice[CHOICE_RED] = axColor(0.8, 0.0, 0.0);
	_colorChoice[CHOICE_GREEN] = axColor(0.0, 0.8, 0.0);
	_colorChoice[CHOICE_BLUE] = axColor(0.0, 0.0, 0.8);

	_probabilityValues[CHOICE_RED] = 1.0;
	_probabilityValues[CHOICE_GREEN] = 0.5;
	_probabilityValues[CHOICE_BLUE] = 0.2;

	_bgColors[0] = axColor(0.3, 0.3, 0.3);
	_bgColors[1] = axColor(0.32, 0.32, 0.32);
	_bgColors[2] = axColor(0.4, 0.4, 0.4);
	_bgColors[3] = axColor(0.42, 0.42, 0.42);

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
				_sequenceColors[i] = &_bgColors[0];
			}
			else
			{
				_sequenceColors[i] = &_bgColors[2];
			}
		}
		else
		{
			if (four_bar)
			{
				_sequenceColors[i] = &_bgColors[1];
			}
			else
			{
				_sequenceColors[i] = &_bgColors[2];
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

void MidiTrackSequence::SetColorSelection(const ColorChoice& color)
{
	_choice = color;
}

void MidiTrackSequence::SetVelocity(const int& index,
								    const int& subtrack,
								    const float& value)
{
	if (_notes[index].velocities[subtrack] != value)
	{
		_notes[index].velocities[subtrack] = value;
		Update();
	}
	
}

void MidiTrackSequence::SetPreset(TrackInfo* info)
{
    _nSubTrack = info->nSubTrack;

    for(int j = 0; j < 3; j++)
    {
        for(int i = 0; i < 16; i++)
        {
            if(info->notes[0][i] || info->notes[1][i] || info->notes[2][i])
            {
                _notes[i].active = true;
            }
            
            _notes[i].actives[j] = info->notes[j][i];

			if (info->probability[j][i] == _probabilityValues[CHOICE_RED])
            {
				_notes[i].colors[j] = _colorChoice[CHOICE_RED];
            }
			else if (info->probability[j][i] == _probabilityValues[CHOICE_GREEN])
            {
				_notes[i].colors[j] = _colorChoice[CHOICE_GREEN];
            } 
			else if (info->probability[j][i] == _probabilityValues[CHOICE_BLUE])
            {
				_notes[i].colors[j] = _colorChoice[CHOICE_BLUE];
            }
        }
    }

    Update();
}

void MidiTrackSequence::OnMouseMotion(const axPoint& mousePos)
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

void MidiTrackSequence::OnMouseLeftDown(const axPoint& mousePos)
{
	if(_highLightIndex != -1)
	{
		_notes[_highLightIndex].active = true;

		// Note is already active, needs to be turn off. 
		if(_notes[_highLightIndex].actives[_selectedSeparationNumber] == true)
		{
			_notes[_highLightIndex].actives[_selectedSeparationNumber] = false;

			// AUDIO CALL.
			_audio->SetMidiNote(_track_number, 
							    _selectedSeparationNumber, 
								_highLightIndex, false);      
		}
		// Note needs to be turn on.
		else
		{
			_notes[_highLightIndex].actives[_selectedSeparationNumber] = true;
			_notes[_highLightIndex].colors[_selectedSeparationNumber] = _colorChoice[_choice];

			// AUDIO CALL.
			_audio->SetMidiNote(_track_number, _selectedSeparationNumber, _highLightIndex, true);
			_audio->SetProbability(_track_number,
								   _selectedSeparationNumber,
								   _highLightIndex, 
								   _probabilityValues[_choice]);
		}
		Update();
	}
}

void MidiTrackSequence::OnMouseLeave()
{
	if(_highLightIndex != -1)
	{
		_highLightIndex = -1;
		Update();
	}
}

void MidiTrackSequence::OnPaint()
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
		gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
		gc->DrawLine(axPoint(_gridRects[i].position.x, 0), 
					 axPoint(_gridRects[i].position.x, rect0.size.y));
	}

	// Draw separation;
	gc->SetColor(axColor(0.1, 0.1, 0.1), 1.0);
	int y_pos = height_y;

	for(int i = 0; i < _nSubTrack; i++)
	{
		gc->DrawLine(axPoint(0.0, y_pos), axPoint(rect0.size.x, y_pos));
		y_pos += height_y;
	}

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);
}

//-----------------------------------------------------------------------------
// MidiTrackName.
//-----------------------------------------------------------------------------
MidiTrackName::MidiTrackName(axWindow* parent,
               const axRect& rect, 
               const string& name):
      axPanel(parent, rect),
      _trackName(name)
{

}


void MidiTrackName::OnPaint()
{
  //string path = GetCurrentAppDirectory();

  axGC* gc = GetGC();
  axRect rect(GetRect());
  axRect rect0(axPoint(0, 0), rect.size);

  gc->SetColor(axColor(0.2, 0.2, 0.2), 1.0);
  gc->DrawRectangle(rect0);

  gc->SetColor(axColor(0.8, 0.8, 0.8), 1.0);
  gc->SetFontSize(13);
  gc->DrawStringAlignedCenter(_trackName, rect0);
  gc->DrawRectangleContour(rect0);
}


//-----------------------------------------------------------------------------
// MidiTrack.
//-----------------------------------------------------------------------------
MidiTrack::MidiTrack(axWindow* parent, const axRect& rect,
           const string& trackName, Audio* audio, int track_number):
      axPanel(parent, rect),
      _track_number(track_number),
      _nSubTrack(2)
      
{
  _audio = static_cast<AudioMidiSeq*>(audio);

  axButtonInfo btn_info(axColor(0.2, 0.2, 0.2),
                        axColor(0.3, 0.3, 0.3),
                        axColor(0.2, 0.2, 0.2),
                        axColor(0.2, 0.2, 0.2),
                        axColor(0.0, 0.0, 0.0),
                        axColor(0.8, 0.8, 0.8));

//  function<void (axButtonMsg)> btnFct(GetOnAddSeparation());

  int x = 10, y = 10, xDelta = 25;

  int height = 30.0 / double(_nSubTrack);

    axButtonEvents btnEvents;
    btnEvents.button_click = GetOnAddSeparation();
    
  _addBtn = new axButton(this,
               axRect(rect.size.x - 14, 0, 14, height), 
               btnEvents, 
			   btn_info, "/Users/alexarse/Project/axLib/axProjects/MidiSequencer/btnhigh.png", "+",
			   axBUTTON_SINGLE_IMG | axBUTTON_IMG_RESIZE);

  btnEvents.button_click = GetOnRemoveSeparation();
  _removeBtn = new axButton(this,
               axRect(rect.size.x - 14, height, 14, height), 
               btnEvents,
               btn_info, "/Users/alexarse/Project/axLib/axProjects/MidiSequencer/btnhigh.png", "-",
			   axBUTTON_SINGLE_IMG | axBUTTON_IMG_RESIZE);


  // _trackName = new MidiTrackName(app, this, 
  //                 axRect(0, 0, 60, 30), 
  //                 trackName);

  btnEvents.button_click = GetOnMinimize();
  axButton* name = new axButton(this,
               axRect(0, 0, 60, 30), 
               btnEvents,
			   btn_info, "/Users/alexarse/Project/axLib/axProjects/MidiSequencer/miditrackname.png", trackName, axBUTTON_SINGLE_IMG);

  _trackSeq = new MidiTrackSequence(this,
                    axRect(60, 0, 
                         rect.size.x - 60 - 14, 
                         30), 
                        audio, _track_number);
//
  axEvtFunction(MultipleSliderMsg) fct(GetOnVelocity());
  axEventFunction sfct(GetOnStandardDeviation());
  _velocity = new MidiVelocity(this, axRect(0, 30, rect.size.x, 60), fct, sfct);


  _nSubTrack = 1;
  _trackSeq->SetNumberOfSubTrack(_nSubTrack);
  _velocity->SetNumberOfSlider(_nSubTrack);
  _audio->SetTrackNumberOfSection(_track_number, _nSubTrack);

  OnMinimize(axButtonMsg(name, ""));

}

void MidiTrack::SetPreset(TrackInfo* info)
{
    _trackSeq->SetPreset(info);
    _velocity->SetPreset(info);
}

void MidiTrack::OnVelocity(const MultipleSliderMsg& msg)
{
    //cout << msg.bar_index << " " << msg.index << " " << msg.value << endl;
    _audio->SetVelocity(_track_number, 
                        msg.index, 
                        msg.bar_index, 
                        msg.value);

	//cout << "index : " << msg.index << " bar index : " << msg.bar_index << endl;
	_trackSeq->SetVelocity(msg.bar_index, msg.index, msg.value);
}

void MidiTrack::OnStandardDeviation(const axNumberBoxMsg& msg)
{
  // cout << "TEST : " << msg.GetValue() << endl;

  _audio->SetStandardDeviation(_track_number, msg.GetValue() * 0.5);
    // cout << msg.bar_index << " " << msg.index << " " << msg.value << endl;
    // _audio->SetVelocity(_track_number, 
    //                     msg.index, 
    //                     msg.bar_index, 
    //                     msg.value);
}

void MidiTrack::OnAddSeparation(const axButtonMsg& msg)
{
  _trackSeq->SetNumberOfSubTrack(++_nSubTrack);
  _velocity->SetNumberOfSlider(_nSubTrack);
  _audio->SetTrackNumberOfSection(_track_number, _nSubTrack);
}

void MidiTrack::OnMinimize(const axButtonMsg& msg)
{
  if(_velocity->IsShown())
  {
    _velocity->Hide();
    SetSize(GetRect().size - axSize(0, 60));

    if (_evt.minimize)
    {
      _evt.minimize(0);
    }

  }
  else
  {
    _velocity->Show();
    SetSize(GetRect().size + axSize(0, 60));

    if (_evt.minimize)
    {
      _evt.minimize(0);
    }
  }

}

void MidiTrack::OnRemoveSeparation(const axButtonMsg& msg)
{
  _nSubTrack--;
  if(_nSubTrack <= 0)
  {
    _nSubTrack = 1;
  }
  _trackSeq->SetNumberOfSubTrack(_nSubTrack);
  _velocity->SetNumberOfSlider(_nSubTrack);
  _audio->SetTrackNumberOfSection(_track_number, _nSubTrack);
}

//------------------------------------------------------------------------

void MidiTrack::SetColorSelection(const ColorChoice& choice)
{
  _trackSeq->SetColorSelection(choice);
}

void MidiTrack::OnPaint()
{
  axGC* gc = GetGC();
  axRect rect(GetRect());
  axRect rect0(axPoint(0, 0), rect.size);

  gc->SetColor(axColor("#888800"), 1.0);
  gc->DrawRectangle(rect0);

  gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
  gc->DrawRectangleContour(rect0);
}


//-----------------------------------------------------------------------------
// LineSelection.
//-----------------------------------------------------------------------------
LineSelection::LineSelection(axWindow* parent, const axRect& rect):
      axPanel(parent, rect)
{
    
}


void LineSelection::OnPaint()
{
  axGC* gc = GetGC();
  axRect rect(GetRect());
  axRect rect0(axPoint(0, 0), rect.size);

  gc->SetColor(axColor(0.1, 0.1, 0.1), 1.0);
  gc->DrawRectangle(rect0);

   gc->SetColor(axColor(0.6, 0.6, 0.6), 1.0);
   gc->SetFontSize(10);
   // gc->DrawLine(axPoint(1, rect0.size.y), axPoint(1, rect0.size.y - 10));

  for(int i = 0; i < 16; i++)
  {
    int x_pos = ((i) / double(16)) * (rect0.size.x - 14.0);

    // gc->SetColor(axColor(0.6, 0.6, 0.6), 1.0);
    if(i % 4 == 0)
    {
        gc->DrawLine(axPoint(x_pos, rect0.size.y), axPoint(x_pos, rect0.size.y - 8));
    }
    else
    {
       gc->DrawLine(axPoint(x_pos, rect0.size.y), axPoint(x_pos, rect0.size.y - 6)); 
    }

    int x_pos2 = ((i+1) / double(16)) * (rect0.size.x - 14.0);
    
  // gc->DrawString(_trackName, axPoint(0, 0));
    gc->DrawStringAlignedCenter(to_string(i+1), axRect(x_pos, 5, x_pos2 - x_pos, rect0.size.y - 5));
    
  }
 

  gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
  gc->DrawRectangleContour(rect0);
}



//-----------------------------------------------------------------------------
// MidiVelocity.
//-----------------------------------------------------------------------------
MidiVelocity::MidiVelocity(axWindow* parent,
                           const axRect& rect, 
                           axEvtFunction(MultipleSliderMsg) fct,
                           axEventFunction sfct):
                           axPanel(parent, rect),
                           // Members.
                           _velocity_fct(fct),
                           _standard_deviation_fct(sfct)
{
    for(int i = 0; i < 16; i++)
    {
        double ratio = (rect.size.x - 60 - 14) / double(16);
        int left_x = i * ratio;
        int right_x = (i+1.0) * ratio;

        axRect r(60 + left_x, 0, right_x - left_x, rect.size.y);

        axColor color(0.75, 0.75, 0.75);
        if(i % 2)
        {
          color = axColor(0.8, 0.8, 0.8);
        }

        axEvtFunction(MultipleSliderMsg) sldfct(GetOnChangeVelocity());
        _sliders.push_back(new MultipleSlider(this, r, color, i, sldfct));
    }


    // axKnobInfo knob_info(axColor(0.6, 0.6, 0.6),
    //                    axColor("#AAAAAA"),
    //                    axColor("#BBBBBB"),
    //                    128,               
    //                    axSize( 32, 32 ),  
    //                    "knob.png",         
    //                    "knobSelected.png");

    // axEvtFunction(axKnobMsg) evt(GetOnStandardDeviation());
    // axKnob* knob = new axKnob(app, this, axRect(25, 15, 32, 32),
    //                         axKnobEvents(evt), knob_info);

    axNumberBoxInfo box_info(axColor(0.7, 0.7, 0.7),
                             axColor(0.3, 0.3, 0.3),
                             axColor(0.2, 0.2, 0.2),
                             axColor(0.2, 0.2, 0.2),
                             axColor(0.0, 0.0, 0.0),
                             axColor(0.0, 0.0, 0.0));

//    axEvtFunction(axNumberBoxMsg) evt(GetOnStandardDeviation());
    axNumberBoxEvents numEvents;
    numEvents.value_change = GetOnStandardDeviation();

    axNumberBox* box1 = new axNumberBox(this,
                           axRect(15, 10, 40, 20), 
                           numEvents,
                           box_info);

}

void MidiVelocity::SetPreset(TrackInfo* info)
{
    SetNumberOfSlider(info->nSubTrack);

    for(int j = 0; j < 3; j++)
    {
        for(int i = 0; i < 16; i++)
        {
            _sliders[i]->SetValue(j, info->velocity[j][i]);
        }
    }
}

void MidiVelocity::OnChangeVelocity(const MultipleSliderMsg& msg)
{    
    if(_velocity_fct)
    {
        _velocity_fct(msg);
    }
    
    // cout << msg.bar_index << " " << msg.index << " " << msg.value << endl;
}

void MidiVelocity::OnStandardDeviation(const axNumberBoxMsg& msg)
{
    if(_standard_deviation_fct)
    {
       //------------------------------------------------------------------------------------------
      //_standard_deviation_fct(new const axNumberBoxMsg(*msg));
    }
}


void MidiVelocity::OnPaint()
{
  axGC* gc = GetGC();
  axRect rect(GetRect());
  axRect rect0(axPoint(0, 0), rect.size);

  gc->SetColor(axColor(0.6, 0.6, 0.6), 1.0);
  gc->DrawRectangle(rect0);

  gc->SetColor(axColor(0.7, 0.7, 0.7), 1.0);

  for(int i = 0; i < 20; i++)
  {
    int y_pos = (i / double(20)) * (rect0.size.y);
    gc->DrawLine(axPoint(60, y_pos), axPoint(rect.size.x-14, y_pos));
  }

  gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);

  for(int i = 0; i < 16+1; i++)
  {
    int x_pos = (i / double(16)) * (rect0.size.x - 60 - 14) + 60;
    gc->DrawLine(axPoint(x_pos, 0), axPoint(x_pos, rect0.size.y));
  }

  gc->DrawRectangleContour(rect0);
}