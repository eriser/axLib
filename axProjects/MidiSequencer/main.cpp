#include "main.h"

MidiPartitionTrack::MidiPartitionTrack(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect),
			_nPart(4)
{

}

void MidiPartitionTrack::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor("#AAAAAA"), 1.0);
	gc->DrawRectangle(rect0);

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);

	int delta = rect0.size.y / double(_nPart + 1.0);
	int y = delta;

	for(int i = 0; i < _nPart; i++)
	{
		gc->DrawCircle(axPoint(rect0.size.x * 0.5 - 1, y), 
			   		   rect0.size.x * 0.5 - 2, 10);

		y += delta;
	}


	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);
}

// struct CircleInfo
// {
// 	CircleInfo()
// 	{
// 		parent_bar = -1;
// 		parent_section = -1;
// 		parent_circle = -1;
// 	}
	
// 	int parent_bar;
// 	int parent_section;
// 	int parent_circle;
// };

// struct SectionInfo
// {
// 	int nCircle;

// 	vector<CircleInfo> circles;

// };

MidiPartition::MidiPartition(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect)
			
{

	_circle = new axImage("round.png");

	_circles.resize(16);

	// SectionInfo c1;
	// c1.nCircle = 2;
	// _circles[0].push_back(c1);

	// _circles[0]
	// pair<int, CircleInfo> c1;
	// c1.first = 2;

	// _circles[0].push_back(c1);



	// c1.first = 2;
	// c1.second.parent_bar = 0;
	// c1.second.parent_section = 0;
	// c1.second.parent_circle = 0;
	// _circles[1].push_back(c1);

	// c1.first = 2;
	// c1.second.parent_bar = 0;
	// c1.second.parent_section = 0;
	// c1.second.parent_circle = 1;
	// _circles[1].push_back(c1);


	_circles[0].push_back(1);
	_circles[0].push_back(1);
	// SectionInfo c2;
	// c2.nCircle = 2;

	// CircleInfo cinfo;
	// cinfo.parent_bar = 0;
	// cinfo.parent_section = 0;
	// cinfo.parent_circle = 1;

	// c2.circles.push_back(cinfo);
	// c2.circles.push_back(cinfo);

	// _circles[1]
	_circles[1].push_back(2);
	_circles[1].push_back(2);
	// _circles[1].push_back(1);

	_circles[2].push_back(2);
	_circles[2].push_back(2);
	_circles[2].push_back(1);
	_circles[2].push_back(1);

	_circles[3].push_back(2);
	_circles[3].push_back(1);

	_circles[4].push_back(1);
	_circles[4].push_back(1);
	_circles[5].push_back(1);
}

axPoint MidiPartition::GetCirclePosition(const int& bar_index, 
						  const int& nSection, 
						  const int& section_index,
						  const int& nCircle, 
						  const int& circle_index)
{
	axRect rect0(axPoint(0, 0), GetSize());

	if(nSection)
	{
		int circle_size = (1.0 / double(16)) * rect0.size.x * 0.5;
		int x_delta_pos = (1.0 / double(16)) * rect0.size.x * 0.25;

		int separation_height = rect0.size.y / double(nSection);
		int left_x = (bar_index / double(16)) * rect0.size.x;
		int right_x = ((bar_index + 1) / double(16)) * rect0.size.x;

		int separation_y_pos = separation_height * section_index;
		int y_delta = separation_height / double(nCircle);
		
		int y_pos = separation_y_pos + y_delta * 0.5 - circle_size * 0.5;
		y_pos += (y_delta * circle_index);

		return axPoint(left_x + x_delta_pos, y_pos);
	}

	return axPoint(0, 0);
}

void MidiPartition::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.3, 0.3, 0.3), 1.0);
	gc->DrawRectangle(rect0);

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);

	int nSlice = 16;
	for(int i = 0; i < nSlice; i++)
	{
		int left_x = (i / double(nSlice)) * rect0.size.x;

		gc->DrawLine(axPoint(left_x, 0), axPoint(left_x, rect0.size.y));
	}


	int circle_size = (1.0 / double(nSlice)) * rect0.size.x * 0.5;

	// for(int i = 0; i < nSlice; i++)
	// {
	// 	for(int k = 0; k < _circles[i].size(); k++)
	// 	{
	// 		// for(int n = 0; n < _circles[i][k]; n++)
	// 		for(int n = 0; n < _circles[i][k].first; n++)
	// 		{
	// 			axPoint pos(GetCirclePosition(i, _circles[i].size(), k, _circles[i][k].first, n));

	// 			// Draw lines.
	// 			if(i > 0)
	// 			{
	// 				int parent_bar_index = _circles[i][k].second.parent_bar;
	// 				int parent_nSection = _circles[parent_bar_index].size();

	// 				int parent_section_index = _circles[i][k].second.parent_section;

	// 				int parent_nCircle = _circles[parent_bar_index][parent_section_index].first;
	// 				int parent_circle_index = _circles[i][k].second.parent_circle;

	// 				axPoint parent_pos(GetCirclePosition(parent_bar_index, 
	// 													 parent_nSection ,
	// 													 parent_section_index, 
	// 													 parent_nCircle, 
	// 													 parent_circle_index));

	// 				int x_delta_pos = (1.0 / double(16)) * rect0.size.x * 0.25;
	// 				int circle_size = (1.0 / double(16)) * rect0.size.x * 0.5;

	// 				axPoint offset(x_delta_pos, circle_size * 0.5);
	// 				gc->DrawLine(pos + offset, parent_pos + offset);
	// 			}
	// 		}
	// 	}
	// }

	for(int i = 0; i < nSlice; i++)
	{
		for(int k = 0; k < _circles[i].size(); k++)
		{
			for(int n = 0; n < _circles[i][k]; n++)
			// for(int n = 0; n < _circles[i][k].first; n++)
			{
				axPoint pos(GetCirclePosition(i, _circles[i].size(), k, _circles[i][k], n));
				// axPoint pos(GetCirclePosition(i, _circles[i].size(), k, _circles[i][k].first, n));

				// Draw lines.
				// if(i > 0)
				// {
				// 	int parent_bar_index = _circles[i][k].second.parent_bar;
				// 	int parent_nSection = _circles[parent_bar_index].size();

				// 	int parent_section_index = _circles[i][k].second.parent_section;

				// 	int parent_nCircle = _circles[parent_bar_index][parent_section_index].first;
				// 	int parent_circle_index = _circles[i][k].second.parent_circle;

				// 	axPoint parent_pos(GetCirclePosition(parent_bar_index, 
				// 										 parent_nSection ,
				// 										 parent_section_index, 
				// 										 parent_nCircle, 
				// 										 parent_circle_index));

				// 	int x_delta_pos = (1.0 / double(16)) * rect0.size.x * 0.25;
				// 	int circle_size = (1.0 / double(16)) * rect0.size.x * 0.5;

				// 	axPoint offset(x_delta_pos, circle_size * 0.5);
				// 	gc->DrawLine(pos + offset, parent_pos + offset);
				// }

				gc->DrawImageResize(_circle, 
									axPoint(pos.x, pos.y), 
									axSize(circle_size, circle_size));

			}
		}
	}

	// int circle_size = (1.0 / double(nSlice)) * rect0.size.x * 0.5;
	// int x_delta_pos = (1.0 / double(nSlice)) * rect0.size.x * 0.25;



	// For each bar.
	// for(int i = 0; i < nSlice; i++)
	// {
	// 	int nbSeparation = _circles[i].size();

	// 	if(nbSeparation)
	// 	{
	// 		int separation_height = rect0.size.y / double(nbSeparation);
	// 		int separation_y_pos = 0;

	// 		int left_x = (i / double(nSlice)) * rect0.size.x;
	// 		int right_x = ((i+1) / double(nSlice)) * rect0.size.x;

	// 		for(int k = 0; k < nbSeparation; k++)
	// 		{
	// 			axRect separation_rect(separation_y_pos, 0, right_x - left_x, separation_height);
	// 			int nbCircle = _circles[i][k];
	// 			int y = separation_height / double(nbCircle);

	// 			int y_pos = separation_y_pos + y * 0.5 - circle_size * 0.5;
				
	// 			for(int n = 0; n < nbCircle; n++)
	// 			{
	// 				gc->DrawImageResize(_circle, 
	// 									axPoint(left_x + x_delta_pos, y_pos), 
	// 									axSize(circle_size, circle_size));
	// 				y_pos += y;
	// 			}

	// 			separation_y_pos += separation_height;
	// 		}			
	// 	}

	// 	// cout << "size : " << size << endl;


	// }

	// gc->SetColor(axColor(0.5, 0.5, 0.1), 1.0);
	// gc->DrawCircle(axPoint(3, 3), 5, 10);

	

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);
}

SynthControl::SynthControl(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect)
{

	axButtonInfo btn_info(axColor(0.2, 0.2, 0.2),
                        axColor(0.3, 0.3, 0.3),
                        axColor(0.2, 0.2, 0.2),
                        axColor(0.2, 0.2, 0.2),
                        axColor(0.0, 0.0, 0.0),
                        axColor(0.8, 0.8, 0.8));

	  // function<void (axButtonMsg)> btnFct(GetOnAddSeparation());


	  axButton* btn = new axButton(app, this, 
	               axRect(5, 5, 45, 20), 
	               axButtonEvents(), 
	               btn_info, "", "Open", axBUTTON_SINGLE_IMG | axBUTTON_IMG_RESIZE);

	axWaveform* wave = new axWaveform(app, this, 
						   axRect(20, 30, rect.size.x - 40, 80),
						   axWaveformEvents(),
						   axWaveformInfo(axColor(0.0, 0.0, 0.0), // Waveform.
                    					  axColor(0.4, 0.4, 0.4), // Background.
                    					  axColor(0.5, 0.5, 0.5), // Lines.
                    					  axColor(0.0, 0.0, 0.0))); // Contour.
	// SynthControlTrack* track = new SynthControlTrack(app, this, axRect(0, 0, 20, rect.size.y));



	axNumberBoxInfo box_info(axColor(0.2, 0.2, 0.2),
                        	 axColor(0.3, 0.3, 0.3),
                        	 axColor(0.2, 0.2, 0.2),
                        	 axColor(0.2, 0.2, 0.2),
                        	 axColor(0.0, 0.0, 0.0),
                        	 axColor(0.8, 0.8, 0.8));

	  // function<void (axButtonMsg)> btnFct(GetOnAddSeparation());


  	axNumberBox* box1 = new axNumberBox(app, this, 
	               					 axRect(20, 140, 40, 20), 
	               					 axNumberBoxEvents(), 
	               					 box_info);

    axNumberBox* box2 = new axNumberBox(app, this, 
	               					 axRect(70, 140, 40, 20), 
	               					 axNumberBoxEvents(), 
	               					 box_info);

	axNumberBox* box3 = new axNumberBox(app, this, 
	               					 axRect(120, 140, 40, 20), 
	               					 axNumberBoxEvents(), 
	               					 box_info);

  axKnobInfo knob_info(axColor(0.2, 0.2, 0.2),
                       axColor("#AAAAAA"),
                       axColor("#BBBBBB"),
                       128,               
                       axSize( 32, 32 ),  
                       "knob.png",         
					   "knobSelected.png");

  axKnob* knob = new axKnob(app, this, axRect(180, 130, 32, 32),
  							axKnobEvents(), knob_info);

  axKnob* knob2 = new axKnob(app, this, axRect(230, 130, 32, 32),
  							axKnobEvents(), knob_info);

	axKnob* knob3 = new axKnob(app, this, axRect(380, 130, 32, 32),
  							axKnobEvents(), knob_info);

	axKnob* knob4 = new axKnob(app, this, axRect(425, 130, 32, 32),
  							axKnobEvents(), knob_info);

}


void SynthControl::OnPaint()
{
	axGC* gc = GetGC();
	BlockDrawing();

	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);
	

	gc->SetColor(axColor(0.2, 0.2, 0.2), 1.0);
	gc->DrawRectangle(rect0);

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);

	// gc->SetColor(axColor(0.0, 1.0, 0.0), 1.0);
	// gc->DrawRectangle(axRect(rect0.position.x, rect0.position.y, 800, 30));

	UnBlockDrawing();
}


MidiSequencer::MidiSequencer(axApp* app, axWindow* parent, const axRect& rect, Audio* audio):
			axPanel(app, parent, rect)
{
	string path = GetCurrentAppDirectory();

	_choice = CHOICE_RED;

	axFlag btnFlags = axBUTTON_IMG_RESIZE | axBUTTON_SINGLE_IMG;

	axButtonInfo btn_info(axColor(0.8, 0.0, 0.0),
						  axColor(0.9, 0.0, 0.0),
						  axColor(0.8, 0.0, 0.0),
						  axColor(0.8, 0.0, 0.0),
						  axColor(0.0, 0.0, 0.0),
						  axColor(0.0, 0.0, 0.0));

	function<void (axButtonMsg)> btnFct(GetOnColorChange());

	int x = 0, y = 0, xDelta = 20;

	btns.push_back(new axButton(app, this, 
								 axRect(x, y, 20, 20), 
								 axButtonEvents(btnFct), 
								 btn_info, "btn.png", "", btnFlags, 
								 to_string(CHOICE_RED)));

	x += xDelta;
	btn_info.normal = axColor(0.0, 0.8, 0.0);
	btn_info.hover = axColor(0.0, 0.9, 0.0);
	btn_info.clicking = axColor(0.0, 0.8, 0.0);
	btn_info.selected = axColor(0.0, 0.8, 0.0);
	btn_info.normal = axColor(0.0, 0.8, 0.0);

	btns.push_back(new axButton(app, this, 
								axRect(x, y, 20, 20), 
								axButtonEvents(btnFct), 
								btn_info, "btn.png", "", btnFlags, 
								to_string(CHOICE_GREEN)));

	x += xDelta;
	btn_info.normal = axColor(0.0, 0.0, 0.8);
	btn_info.hover = axColor(0.0, 0.0, 0.9);
	btn_info.clicking = axColor(0.0, 0.0, 0.8);
	btn_info.selected = axColor(0.0, 0.0, 0.8);
	btn_info.normal = axColor(0.0, 0.0, 0.8);

	btns.push_back(new axButton(app, this, 
							axRect(x, y, 20, 20), 
							axButtonEvents(btnFct),
							btn_info, "btn.png", "", btnFlags, to_string(CHOICE_BLUE)));

	_trackSize = axSize(480, 90);

	LineSelection* lines = new LineSelection(app, this, axRect(60, 0, 420, 20));

	AddNewTrack("Kick", audio, 0);
	AddNewTrack("Snare", audio, 1);
	AddNewTrack("HiHat", audio, 2);
	AddNewTrack("OpenHat", audio, 3);	

	// SynthControl* midiPart = new SynthControl(app, this, axRect(500, 80, 420, 300));

	// axPoint pos = GetNextTrackPosition();
	
}

void MidiSequencer::OnColorChange(const axButtonMsg& msg)
{
	_choice = (ColorChoice)stoi(msg.GetMsg());

	for(MidiTrack* t : _midiTracks)
	{
		t->SetColorSelection(_choice);
	}
}

void MidiSequencer::AddNewTrack(const string& trackName, Audio* audio, int num)
{
	if(_midiTracks.size() == 0)
	{
		_midiTracks.push_back(new MidiTrack(GetApp(), this, 
											axRect(axPoint(0, 20), 
											_trackSize), 
											trackName, audio, num));

		// SetSize(axSize(GetRect().size.x, GetRect().size.y + _trackSize.y));
		SetSize(axSize(GetRect().size.x, 
					   5 + _midiTracks[_midiTracks.size() - 1]->GetBottomLeftPosition().y));

	}
	else
	{
		_midiTracks.push_back(new MidiTrack(GetApp(), this, 
											axRect(GetNextTrackPosition(), 
											_trackSize), 
											trackName, audio, num));

		// SetSize(axSize(GetRect().size.x, GetRect().size.y + _trackSize.y));
		SetSize(axSize(GetRect().size.x, 
					   5 + _midiTracks[_midiTracks.size() - 1]->GetBottomLeftPosition().y));
	}

	function<void (int)> min_fct(GetOnChangeTrackHeight());
	MidiTrackEvents evt(min_fct);
	_midiTracks[_midiTracks.size() - 1]->SetEvent(evt);

	// _midiTracks[_midiTracks.size() - 1]->SetEvent(evt);
}

axPoint MidiSequencer::GetNextTrackPosition() const
{
	return _midiTracks[_midiTracks.size() - 1]->GetBottomLeftPosition();
}

void MidiSequencer::OnChangeTrackHeight(const int& msg)
{
	for(int i = 1; i < _midiTracks.size(); i++)
	{
		_midiTracks[i]->SetPosition(_midiTracks[i-1]->GetBottomLeftPosition());
	}

	SetSize(axSize(GetRect().size.x, 
				   5 + _midiTracks[_midiTracks.size() - 1]->GetBottomLeftPosition().y));


	if(_track_resize_parent_fct)
	{
		_track_resize_parent_fct(msg);
	}
}


void MidiSequencer::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.1, 0.1, 0.1), 1.0);
	gc->DrawRectangle(rect0);

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);
}

DrumMachine::DrumMachine(axApp* app, 
				   axWindow* parent, 
				   const axRect& rect, Audio* audio):
	axPanel(app, parent, rect)
{
	_midiSeq = new MidiSequencer(app, this, axRect(45, 20, 480, 50), audio);

	axEvtFunction(int) trackResizeFct(GetOnChangeTrackHeight());
	_midiSeq->SetTrackResizeFct(trackResizeFct);

	int y = _midiSeq->GetBottomLeftPosition().y;
	_synth = new SynthControl(app, this, axRect(45, y, 480, 180));

	_midiPartition = new MidiPartition(app, this, 
			axRect(_synth->GetBottomLeftPosition(), axSize(480, 180)));

	_side_img = new axImage("woodSide.png");
}

void DrumMachine::OnChangeTrackHeight(const int& msg)
{
	cout << "DRUM MACHINE RESIZE" << endl;
	_synth->SetPosition(_midiSeq->GetBottomLeftPosition());
	_midiPartition->SetPosition(_synth->GetBottomLeftPosition() + axPoint(0, 5));
}

void DrumMachine::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.1, 0.1, 0.1), 1.0);
	gc->DrawRectangle(rect0);

	gc->DrawImage(_side_img, axPoint(0, 0));
	gc->DrawImage(_side_img, axPoint(rect0.size.x - 45, 0));

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);
}



int main()
{
	AudioMidiSeq* audio = new AudioMidiSeq();

	axApp app(axSize(570, 600));
	DrumMachine* machine = new DrumMachine(&app, 
										   nullptr, 
										   axRect(0, 0, 570, 600),
										   audio);

	audio->StartAudio();
	app.MainLoop();

	return 0;
}
