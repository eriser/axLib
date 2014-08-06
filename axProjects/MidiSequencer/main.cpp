#include "main.h"

//-----------------------------------------------------------------------------
// MidiPartitionTrack.
//-----------------------------------------------------------------------------
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

MidiPartition::MidiPartition(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect),
			_heightlighted_bar(-1),
			_nSlice(10),
			_heighlighted_circle(0),
			_selected_bar(0), 
			_selected_circle(0)
			
{
	_circle = new axImage("round.png");
	_selectedCircle = new axImage("round_sel.png");

	_bars.push_back(1);
 	_bars.push_back(3);
	_bars.push_back(2);
}

void MidiPartition::OnMouseLeave()
{
	_heightlighted_bar = -1;
	Update();
}

void MidiPartition::OnMouseMotion(const axPoint& mouse)
{

	double pos = (mouse - GetAbsoluteRect().position).x;

	int temp_index = _nSlice * pos / GetRect().size.x;

	if(_heightlighted_bar != temp_index)
	{
		_heightlighted_bar = temp_index;
		Update();
	}
}

void MidiPartition::DrawSelectedCircle(axGC* gc, const axRect& rect0)
{

	int circle_size = (1.0 / double(_nSlice)) * rect0.size.x * 0.5;
	int left_x = (_selected_bar / double(_nSlice)) * rect0.size.x;
	int right_x = ((_selected_bar+1) / double(_nSlice)) * rect0.size.x;
	int size_x = right_x - left_x;
	axPoint pos(left_x + (size_x - circle_size) * 0.5,
						(_selected_circle + 1.0) * rect0.size.y / (_bars[_selected_bar] + 1.0) - circle_size * 0.5);
	
	gc->DrawImageResize(_selectedCircle, 
						axPoint(pos.x, pos.y), 
						axSize(circle_size, circle_size));
}

void MidiPartition::DrawLines(axGC* gc, const axRect& rect0)
{
	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	// int nSlice = 16;
	for(int i = 0; i < _nSlice; i++)
	{
		int left_x = (i / double(_nSlice)) * rect0.size.x;
		gc->DrawLine(axPoint(left_x, 0), axPoint(left_x, rect0.size.y));
	}
}

void MidiPartition::DrawCircles(axGC* gc, const axRect& rect0)
{
	int circle_size = (1.0 / double(_nSlice)) * rect0.size.x * 0.5;

	for(int i = 0; i < _bars.size(); i++)
	{
		for(int k = 0; k < _bars[i]; k++)
		{
			int left_x = (i / double(_nSlice)) * rect0.size.x;
			int right_x = ((i+1) / double(_nSlice)) * rect0.size.x;
			int size_x = right_x - left_x;

			axPoint pos(left_x + (size_x - circle_size) * 0.5,
						(k + 1.0) * rect0.size.y / (_bars[i] + 1.0) - circle_size * 0.5);


			if(_heighlighted_circle == k && _heightlighted_bar == i)
			{
				// int select_size = 20;
				gc->SetColor(axColor(0.55, 0.35, 0.35), 1.0);
				gc->DrawRectangle(axRect(axPoint(pos.x - 3, pos.y - 3), 
								  axSize(circle_size + 6, circle_size + 6)));
			}

			

			gc->DrawImageResize(_circle, 
								axPoint(pos.x, pos.y), 
								axSize(circle_size, circle_size));
		}
	}
}

void MidiPartition::DrawHeighlightedCircle(axGC* gc, const axRect& rect0)
{
	if(_heightlighted_bar != -1)
	{
		int left_x = (_heightlighted_bar / double(_nSlice)) * rect0.size.x;
		int right_x = ((_heightlighted_bar+1) / double(_nSlice)) * rect0.size.x;

		gc->SetColor(axColor(0.35, 0.35, 0.35), 1.0);
		gc->DrawRectangle(axRect(left_x, 0, right_x - left_x - 1, rect0.size.y));
	}
}

void MidiPartition::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.3, 0.3, 0.3), 1.0);
	gc->DrawRectangle(rect0);

	DrawLines(gc, rect0);
	DrawHeighlightedCircle(gc, rect0);
	DrawCircles(gc, rect0);
	DrawSelectedCircle(gc, rect0);

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);
}


//-----------------------------------------------------------------------------
// SynthControl.
//-----------------------------------------------------------------------------
SynthControl::SynthControl(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect)
{

	axButtonInfo btn_info(axColor(0.2, 0.2, 0.2),
                        axColor(0.3, 0.3, 0.3),
                        axColor(0.2, 0.2, 0.2),
                        axColor(0.2, 0.2, 0.2),
                        axColor(0.0, 0.0, 0.0),
                        axColor(0.8, 0.8, 0.8));

	function<void (axButtonMsg)> btnFct(GetOnOpenFile());


	axButton* btn = new axButton(app, this, 
	               axRect(5, 5, 45, 20), 
	               axButtonEvents(btnFct), 
	               btn_info, "", "Open", axBUTTON_SINGLE_IMG | axBUTTON_IMG_RESIZE);

	axWaveform* wave = new axWaveform(app, this, 
						   axRect(20, 30, rect.size.x - 40, 80),
						   axWaveformEvents(),
						   axWaveformInfo(axColor(0.0, 0.0, 0.0), // Waveform.
                    					  axColor(0.4, 0.4, 0.4), // Background.
                    					  axColor(0.5, 0.5, 0.5), // Lines.
                    					  axColor(0.0, 0.0, 0.0))); // Contour.

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

void SynthControl::OnOpenFile(const axButtonMsg& msg)
{
	// cout << "FILE" << endl;
	// string f = OpenFileDialog("/home/alexarse/Desktop/axLib/axProjects/FileDialog/main");
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


//-----------------------------------------------------------------------------
// MidiSequencer.
//-----------------------------------------------------------------------------
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

	_trackSize = axSize(rect.size.x, 90);

	LineSelection* lines = new LineSelection(app, this, axRect(60, 0, rect.size.x - 60, 20));

	AddNewTrack("Kick", audio, 0);
	AddNewTrack("Snare", audio, 1);
	AddNewTrack("HiHat", audio, 2);
	AddNewTrack("Clap", audio, 3);	
	AddNewTrack("Tom", audio, 4);	

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

void MidiSequencer::SetPreset(DrumMachinePreset* preset)
{
	if(preset->info != nullptr)
		{
			cout << __PRETTY_FUNCTION__ << endl;
			cout << "NB TRACKS = " << preset->nTracks << endl;

			if(preset->nTracks <= _midiTracks.size())
			{
				for(int i = 0; i < preset->nTracks; i++)
				{
					// TrackInfo& t_info(preset.info[i]);
					 _midiTracks[i]->SetPreset(&preset->info[i]);
				}
			}
			else
			{
				cerr << "Error : Too many tracks in presets." << endl;
			}
		}
		else
		{
			cerr << "Error : Preset is not loaded." << endl;
		}
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

void DrumMachine::ExecApplication(const string& app_name)
{
	pid_t p_id = fork();

	// Child.
	if (p_id == 0)                
	{
		execl(app_name.c_str(), 0);
		exit(0);
	}

	// Failed to fork.
	else if (p_id < 0)            
	{
	    cerr << "Failed to open " << app_name << endl;
	    exit(1);
	    // Throw exception
	}
}

//-----------------------------------------------------------------------------
// DrumMachine.
//-----------------------------------------------------------------------------
DrumMachine::DrumMachine(axApp* app, 
				   		 axWindow* parent, 
				   		 const axRect& rect, Audio* audio):
						 axPanel(app, parent, rect),
						 _audio(audio)
{
	function<void (axButtonMsg)> btnFct(GetOnChangeTemplate());

	string dir(app->GetCurrentAppDirectory());

	// axButtonInfo btn_info(axColor(0.8, 0.0, 0.0),
	// 					  axColor(0.9, 0.0, 0.0),
	// 					  axColor(0.8, 0.0, 0.0),
	// 					  axColor(0.8, 0.0, 0.0),
	// 					  axColor(0.0, 0.0, 0.0),
	// 					  axColor(0.0, 0.0, 0.0));

	// int x = 0, y = 0, xDelta = 20;

	axButton* btn = new axButton(app, this, 
								 axRect(0, 0, 40, 18), 
								 axButtonEvents(btnFct), 
								 axButtonInfo(dir + "ressources/axStandardButton.axobj"), 
								 "", "Open");

	_midiSeq = new MidiSequencer(app, this, axRect(0, 20, rect.size.x, 50), _audio);

	axEvtFunction(int) trackResizeFct(GetOnChangeTrackHeight());
	_midiSeq->SetTrackResizeFct(trackResizeFct);

	// int y = _midiSeq->GetBottomLeftPosition().y;
	// _synth = new SynthControl(app, this, axRect(0, y, rect.size.x, 180));

	_midiPartition = new MidiPartition(app, this, 
			axRect(_midiSeq->GetBottomLeftPosition(), axSize(rect.size.x, 70)));

	// _side_img = new axImage("woodSide.png");

}

void DrumMachine::OnChangeTemplate(const axButtonMsg& msg)
{
	//cout << "Change template." << endl;
	string f = OpenFileDialog("/home/alexarse/Desktop/axLib/axProjects/FileDialog/main", 
							  "/home/alexarse/Desktop/axLib/axProjects/MidiSequencer/axPresets");

	// cout << "FILE : " << f << endl;
	// cout << "EXT : " << axGetExtension(f) << endl;

	// if(axGetExtension(f) == "seq")
	{
		// SetPreset("axPresets/template1.txt");
		SetPreset(f);
	}
}

void DrumMachine::OnChangeTrackHeight(const int& msg)
{
	// cout << "DRUM MACHINE RESIZE" << endl;
	// _synth->SetPosition(_midiSeq->GetBottomLeftPosition());
	 _midiPartition->SetPosition(_midiSeq->GetBottomLeftPosition() + axPoint(0, 5));
}

void DrumMachine::SetPreset(const string& file_path)
{
	cout << __PRETTY_FUNCTION__ << endl;
	DrumMachinePreset preset(file_path);
	AudioMidiSeq* audio = static_cast<AudioMidiSeq*>(_audio);
	audio->SetPreset(&preset);

	_midiSeq->SetPreset(&preset);
}

void DrumMachine::OnPaint()
{
	
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.1, 0.1, 0.1), 1.0);
	gc->DrawRectangle(rect0);

	// gc->DrawImage(_side_img, axPoint(0, 0));
	// gc->DrawImage(_side_img, axPoint(rect0.size.x - 45, 0));

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);
}


ScrollDrumMachine::ScrollDrumMachine(axApp* app, axWindow* parent, const axRect& rect, Audio* audio):
			axPanel(app, parent, rect)
{
	function<void (axScrollBarMsg)> scroll(GetOnScroll());
	scroll_bar = new axScrollBar(app, this, 
								 axRect(rect.size.x -14 - 45, 0, 14, rect.size.y), 
								 axScrollBarEvents(scroll), 
								 axScrollBarInfo());


	scroll_bar->setInputInfo(rect.size.y, 1000, 0);
	_drum = new DrumMachine(app, this, axRect(45, 0, rect.size.x - 90 - 14, 1000), audio);

	_side_img = new axImage("woodSide.png");

	_last_delta = 0;
}

void ScrollDrumMachine::OnScroll(const axScrollBarMsg& msg)
{
	
	int delta = - stof(msg.GetMsg());

	if(_last_delta != delta)
	{
		// cout << "pos : " << GetRect().position.y << endl;
		_drum->SetPosition(axPoint(45, delta));
		cout << delta << endl;
		_last_delta = delta;
	}
	// scroll_win->SetScrollDecay(axPoint(0, stof(msg.GetMsg())));
	
}

void ScrollDrumMachine::OnPaint()
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
	

	// pid_t dialog = fork();

	// if(dialog)
	// {
	// 	// axSize size(50, 50);
	// 	// axApp app_test(size);
	// 	// app_test.MainLoop();

	// 	exit(0);
	// }
	// else if(dialog == -1)
	// {
	// 	cerr << "Error creating process." << endl;
	// }
	// else
	// {
		AudioMidiSeq* audio = new AudioMidiSeq();

		axApp* app = new axApp(axSize(570, 600));

		// axApp app2(axSize(570, 600));
		ScrollDrumMachine* machine = new ScrollDrumMachine(app, 
										   nullptr, 
										   axRect(0, 0, 570, 600),
										   audio);

		
		// DrumMachinePreset test("presets/presets.txt");

		audio->StartAudio();

		// machine->SetPreset("presets/presets.txt");
		app->MainLoop();
	// }

	

	return 0;
}
