#include "main.h"





// MultipleSlider::MultipleSlider(axApp* app, axWindow* parent, const axRect& rect, const axColor& bgColor):
// 			axPanel(app, parent, rect)
// {
// 	_bgColor = bgColor;

// 	axSliderInfo sld_info("",//"sliderPlain.png",		
// 				 		  axSize(8, 8),
// 				 		  axColor(0.9, 0.9, 0.9), // Bg
// 				 		  axColor(1.0, 1.0, 1.0), // bg hover.
// 				 		  axColor(0.9, 0.9, 0.9), // Bg clicking.
// 				 		  axColor(0.8, 0.0, 0.0), // Slider normal
// 				 		  axColor(0.9, 0.0, 0.0), // Slider hover
// 				 		  axColor(0.8, 0.0, 0.0), // Slider clicking.
// 				 		  axColor(0.0, 0.0, 0.0), // Slider contour.
// 				 		  axColor(0.0, 0.0, 0.0), // COntour
// 				 		  axColor(0.9, 0.9, 0.9), // Back Slider
// 				 		  axColor(0.9, 0.9, 0.9), // Back Slider contour.
// 				 		  4);
	
	
// 	_nSlider = 3;
// 	int size_x = rect.size.x / double(_nSlider);

	
// 	axFlag slider_flags = axSLIDER_FLAG_VERTICAL | 
// 						  axSLIDER_FLAG_CLICK_ANYWHERE | 
// 					      axSLIDER_FLAG_RIGHT_ALIGN;

	
// 	sliders[0] = new axSlider(app, this, 
// 									axRect(0, 0, size_x, rect.size.y), 
// 									axSliderEvents(), sld_info, slider_flags);

// 	sliders[1] = new axSlider(app, this, 
// 									axRect(size_x, 0, size_x, rect.size.y), 
// 									axSliderEvents(), sld_info, slider_flags);

// 	sliders[2] = new axSlider(app, this, 
// 							  axRect(size_x * 2, 0, size_x, rect.size.y), 
// 							  axSliderEvents(), sld_info, slider_flags);

// 	sliders[0]->SetBackgroundAlpha(0.0);
// 	sliders[1]->SetBackgroundAlpha(0.0);
// 	sliders[2]->SetBackgroundAlpha(0.0);

// 	// sliders[2]->Hide();

// 	SetNumberOfSlider(2);

// }

// void MultipleSlider::OnPaint()
// {
// 	axGC* gc = GetGC();
// 	axRect rect(GetRect());
// 	axRect rect0(axPoint(0, 0), rect.size);

// 	// Draw background.
// 	gc->SetColor(_bgColor, 1.0);
// 	gc->DrawRectangle(rect0);

// 	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
// 	gc->DrawRectangleContour(rect0);
// }

// MidiTrackSequence::MidiTrackSequence(axApp* app, axWindow* parent, 
// 							 		const axRect& rect, Audio* audio, int num):
// 									axPanel(app, parent, rect),
// 									_track_number(num)
// {
// 	_audio = static_cast<AudioMidiSeq*>(audio);
// 	_nbars = 16;
// 	_highLightIndex = -1;
// 	_notes.resize(_nbars);
// 	_hightColor = axColor(0.7, 0.7, 0.7);
// 	_nSubTrack = 2;
// 	_selectedSeparationNumber = 0;

// }

// void MidiTrackSequence::SetColorSelection(const ColorChoice& color)
// {
// 	_choice = color;
// }

// void MidiTrackSequence::OnMouseMotion(const axPoint& mousePos)
// {
// 	axPoint pos = mousePos - GetAbsoluteRect().position;

// 	int index = pos.x / double(GetRect().size.x) * _nbars;
// 	int selected = (pos.y / double(GetRect().size.y)) * _nSubTrack;

// 	if(index != _highLightIndex || selected != _selectedSeparationNumber)
// 	{
// 		_highLightIndex = index;
// 		_selectedSeparationNumber = selected;
// 		Update();
// 	}
// }

// void MidiTrackSequence::OnMouseLeftDown(const axPoint& mousePos)
// {
// 	if(_highLightIndex != -1)
// 	{
// 		_notes[_highLightIndex].active = true;

// 		if(_notes[_highLightIndex].actives[_selectedSeparationNumber] == true)
// 		{
// 			_notes[_highLightIndex].actives[_selectedSeparationNumber] = false;
// 		}
// 		else
// 		{
// 			_notes[_highLightIndex].actives[_selectedSeparationNumber] = true;

// 			if(_choice == CHOICE_RED)
// 			{
// 				_notes[_highLightIndex].colors[_selectedSeparationNumber] = axColor(0.8, 0.0, 0.0);
// 			}
// 			else if(_choice == CHOICE_GREEN)
// 			{
// 				_notes[_highLightIndex].colors[_selectedSeparationNumber] = axColor(0.0, 0.8, 0.0);
// 			}	
// 			else if(_choice == CHOICE_BLUE)
// 			{
// 				_notes[_highLightIndex].colors[_selectedSeparationNumber] = axColor(0.0, 0.0, 0.8);
// 			}	
// 		}

// 		// _audio->SetBeat(_track_number, _highLightIndex, true);
		
// 		Update();
// 	}
// }

// void MidiTrackSequence::OnMouseLeave()
// {
// 	if(_highLightIndex != -1)
// 	{
// 		_highLightIndex = -1;
// 		Update();
// 	}
// }

// void MidiTrackSequence::DrawMidiSequence(axGC* gc, const axRect& rect0)
// {
// 	axColor colors[3] = {axColor(0.8, 0.0, 0.0), axColor(0.0, 0.8, 0.0), axColor(0.0, 0.0, 0.8)};

// 	for(int i = 0; i < _nbars; i++)
// 	{
// 		if(_notes[i].active == true)
// 		{

// 			double ratio = rect0.size.x / double(_nbars);
// 			int left_x = i * ratio;;
// 			int right_x = (i+1.0) * ratio;


// 			for(int k = 0; k < _nSubTrack; k++)
// 			{
// 				if(_notes[i].actives[k] == true)
// 				{
// 					int pos_y = k * rect0.size.y / double(_nSubTrack);

// 					int size_y = rect0.size.y / double(_nSubTrack);
// // 
// 					// gc->SetColor(colors[k]);
// 					gc->SetColor(_notes[i].colors[k]);
// 					gc->DrawRectangle(axRect(left_x, pos_y, right_x - left_x, size_y));
// 				}
// 			}
// 		}
// 	}
// }

// void MidiTrackSequence::OnPaint()
// {
// 	string path = GetCurrentAppDirectory();

// 	axGC* gc = GetGC();
// 	axRect rect(GetRect());
// 	axRect rect0(axPoint(0, 0), rect.size);

// 	// Draw background.
// 	gc->SetColor(axColor("#888888"), 1.0);
// 	gc->DrawRectangle(rect0);

	
// 	// Draw hightlighted note.
// 	if(_highLightIndex != -1)
// 	{
// 		int height_y = rect0.size.y / double(_nSubTrack);

// 		int left_x = (_highLightIndex / double(_nbars)) * rect0.size.x;
// 		int right_x = ((_highLightIndex+1.0) / double(_nbars)) * rect0.size.x;
// 		int y_top = height_y * _selectedSeparationNumber;
// 		int y_bot = height_y * (1.0 + _selectedSeparationNumber);


// 		if(_notes[_highLightIndex].active == true)
// 		{
// 			gc->SetColor(1.0, 0.0, 0.0);
// 		}
// 		else
// 		{
// 			gc->SetColor(_hightColor);
// 		}

// 		gc->DrawRectangle(axRect(left_x, y_top, right_x - left_x, y_bot - y_top));
// 	}

// 	// Draw separation.
// 	int height_y = rect0.size.y / double(_nSubTrack);
// 	int y_pos = height_y;


// 	// Draw contour.
// 	int delta = double(_nbars) * rect0.size.x;
// 	for(int i = 0; i < _nbars; i++)
// 	{
// 		// gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
// 		int x_pos = (i / double(_nbars)) * rect0.size.x;
// 		// gc->DrawLine(axPoint(x_pos, 0), axPoint(x_pos, rect0.size.y));

// 		gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
// 		int x_pos2 = ((i+1) / double(_nbars)) * rect0.size.x;

// 		if(i % 2)
// 		{
// 			gc->SetColor(axColor(0.8, 0.8, 0.8), 1.0);
// 		}
// 		else
// 		{
// 			gc->SetColor(axColor(0.75, 0.75, 0.75), 1.0);
// 		}
		
// 		gc->DrawRectangle(axRect(x_pos, 0, x_pos2 - x_pos, rect0.size.y));
// 	}

// 	DrawMidiSequence(gc, rect0);


// 	// Draw contour.
// 	// int delta = double(_nbars) * rect0.size.x;
// 	for(int i = 0; i < _nbars; i++)
// 	{
// 		gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
// 		int x_pos = (i / double(_nbars)) * rect0.size.x;
// 		gc->DrawLine(axPoint(x_pos, 0), axPoint(x_pos, rect0.size.y));

// 		// gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
// 		// int x_pos2 = ((i+1) / double(_nbars)) * rect0.size.x;

// 		// if(i % 2)
// 		// {
// 		// 	gc->SetColor(axColor(0.8, 0.8, 0.8), 1.0);
// 		// }
// 		// else
// 		// {
// 		// 	gc->SetColor(axColor(0.75, 0.75, 0.75), 1.0);
// 		// }
		
// 		// gc->DrawRectangle(axRect(x_pos, 0, x_pos2 - x_pos, rect0.size.y));
// 	}




// 	gc->SetColor(axColor(0.4, 0.4, 0.4), 1.0);
// 	for(int i = 0; i < _nSubTrack; i++)
// 	{
// 		gc->DrawLine(axPoint(0.0, y_pos), axPoint(rect0.size.x, y_pos));
// 		y_pos += height_y;
// 	}

	
// 	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
// 	gc->DrawRectangleContour(rect0);

// 	// gc->DrawRectangleContour(axRect(1, 1, rect0.size.x - 2, rect0.size.y - 2));


// }

// MidiTrackName::MidiTrackName(axApp* app, axWindow* parent, 
// 							 const axRect& rect, 
// 							 const string& name):
// 			axPanel(app, parent, rect),
// 			_trackName(name)
// {

// }


// void MidiTrackName::OnPaint()
// {
// 	string path = GetCurrentAppDirectory();

// 	axGC* gc = GetGC();
// 	axRect rect(GetRect());
// 	axRect rect0(axPoint(0, 0), rect.size);

// 	gc->SetColor(axColor("#888888"), 1.0);
// 	gc->DrawRectangle(rect0);

// 	// cout << "TEST" << endl;

// 	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
// 	// gc->SetFontType(path+string("FreeSans.ttf"));
// 	gc->SetFontSize(13);
// 	// gc->DrawString(_trackName, axPoint(0, 0));
// 	gc->DrawStringAlignedCenter(_trackName, rect0);
// 	gc->DrawRectangleContour(rect0);
// }
// //-----------------------------------------------------------------------------


// MidiTrack::MidiTrack(axApp* app, axWindow* parent, const axRect& rect, 
// 					 const string& trackName, Audio* audio, int track_number):
// 			axPanel(app, parent, rect),
// 			_track_number(track_number),
// 			_nSubTrack(2)
// {

// 	axButtonInfo btn_info(axColor(0.6, 0.6, 0.6),
// 						  axColor(0.8, 0.8, 0.8),
// 						  axColor(0.6, 0.6, 0.6),
// 						  axColor(0.6, 0.6, 0.6),
// 						  axColor(0.0, 0.0, 0.0),
// 						  axColor(0.0, 0.0, 0.0));

// 	function<void (axButtonMsg)> btnFct(GetOnAddSeparation());

// 	int x = 10, y = 10, xDelta = 25;

// 	int height = 30.0 / double(_nSubTrack);

// 	_addBtn = new axButton(app, this, 
// 						   axRect(rect.size.x - 14, 0, 14, height), 
// 						   axButtonEvents(btnFct), 
// 						   btn_info, "", "+");

// 	btnFct = GetOnRemoveSeparation();
// 	_removeBtn = new axButton(app, this, 
// 						   axRect(rect.size.x - 14, height, 14, height), 
// 						   axButtonEvents(btnFct), 
// 						   btn_info, "", "-");


// 	// _trackName = new MidiTrackName(app, this, 
// 	// 							   axRect(0, 0, 60, 30), 
// 	// 							   trackName);

// 	btnFct = GetOnMinimize();
// 	axButton* name = new axButton(app, this, 
// 						   axRect(0, 0, 60, 30), 
// 						   axButtonEvents(btnFct), 
// 						   btn_info, "", trackName);

// 	_trackSeq = new MidiTrackSequence(app, this, 
// 									  axRect(60, 0, 
// 									  		 rect.size.x - 60 - 14, 
// 									  		 30), 
// 									  		audio, _track_number);

// 	_velocity = new MidiVelocity(app, this, axRect(0, 30, 480, 60));


// }

// void MidiTrack::OnAddSeparation(const axButtonMsg& msg)
// {
// 	_trackSeq->SetNumberOfSubTrack(++_nSubTrack);
// 	_velocity->SetNumberOfSlider(_nSubTrack);
// }

// void MidiTrack::OnMinimize(const axButtonMsg& msg)
// {
// 	if(_velocity->IsShown())
// 	{
// 		_velocity->Hide();
// 		SetSize(GetRect().size - axSize(0, 60));

// 		if (_evt.minimize)
// 		{
// 			_evt.minimize(0);
// 		}

// 	}
// 	else
// 	{
// 		_velocity->Show();
// 		SetSize(GetRect().size + axSize(0, 60));

// 		if (_evt.minimize)
// 		{
// 			_evt.minimize(0);
// 		}
// 	}

// }

// void MidiTrack::OnRemoveSeparation(const axButtonMsg& msg)
// {
// 	_nSubTrack--;
// 	if(_nSubTrack <= 0)
// 	{
// 		_nSubTrack = 1;
// 	}
// 	_trackSeq->SetNumberOfSubTrack(_nSubTrack);
// 	_velocity->SetNumberOfSlider(_nSubTrack);
// }

// //------------------------------------------------------------------------

// void MidiTrack::SetColorSelection(const ColorChoice& choice)
// {
// 	_trackSeq->SetColorSelection(choice);
// }

// void MidiTrack::OnPaint()
// {
// 	axGC* gc = GetGC();
// 	axRect rect(GetRect());
// 	axRect rect0(axPoint(0, 0), rect.size);

// 	gc->SetColor(axColor("#888800"), 1.0);
// 	gc->DrawRectangle(rect0);

// 	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
// 	gc->DrawRectangleContour(rect0);
// }


// LineSelection::LineSelection(axApp* app, axWindow* parent, const axRect& rect):
// 			axPanel(app, parent, rect)
// {
	
// }


// void LineSelection::OnPaint()
// {
// 	axGC* gc = GetGC();
// 	axRect rect(GetRect());
// 	axRect rect0(axPoint(0, 0), rect.size);

// 	gc->SetColor(axColor("#888888"), 1.0);
// 	gc->DrawRectangle(rect0);

// 	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
// 	gc->DrawRectangleContour(rect0);
// }

// MidiVelocity::MidiVelocity(axApp* app, axWindow* parent, const axRect& rect):
// 			axPanel(app, parent, rect)
// {
// 	// int size_x = (1.0 / double(16)) * (rect.size.x - 60 - 14);
// 	// int pos = 60;



// 	for(int i = 0; i < 16; i++)
// 	{
// 		double ratio = (rect.size.x - 60 - 14) / double(16);
// 		int left_x = i * ratio;
// 		int right_x = (i+1.0) * ratio;

// 		axRect r(60 + left_x, 0, right_x - left_x, rect.size.y);

// 		axColor color(0.75, 0.75, 0.75);
// 		if(i % 2)
// 		{
// 			color = axColor(0.8, 0.8, 0.8);
// 		}
// 		// else
// 		// {
// 		// 	gc->SetColor(axColor(0.75, 0.75, 0.75), 1.0);
// 		// }

// 		_sliders.push_back(new MultipleSlider(app, this, r, color));
// 		//pos += size_x;
// 	}
// 	// axSliderInfo sld_info("sliderPlain.png",
// 	// 			 		  axSize(8, 8),
// 	// 			 		  axColor(0.9, 0.9, 0.9), // Bg
// 	// 			 		  axColor(1.0, 1.0, 1.0), // bg hover.
// 	// 			 		  axColor(0.9, 0.9, 0.9), // Bg clicking.
// 	// 			 		  axColor(0.8, 0.0, 0.0), // Slider normal
// 	// 			 		  axColor(0.9, 0.0, 0.0), // Slider hover
// 	// 			 		  axColor(0.8, 0.0, 0.0), // Slider clicking.
// 	// 			 		  axColor(0.0, 0.0, 0.0), // Slider contour.
// 	// 			 		  axColor(0.0, 0.0, 0.0), // COntour
// 	// 			 		  axColor(0.9, 0.9, 0.9), // Back Slider
// 	// 			 		  axColor(0.9, 0.9, 0.9), // Back Slider contour.
// 	// 			 		  4);
	
// 	// int size_x = (1.0 / double(16)) * (rect.size.x - 60 - 14);
// 	// axSlider* slider = new axSlider(app, this, 
// 	// 								axRect(60, 0, size_x / 3.0, rect.size.y), 
// 	// 								axSliderEvents(), sld_info,
// 	// 								axSLIDER_FLAG_VERTICAL | 
// 	// 								axSLIDER_FLAG_CLICK_ANYWHERE | 
// 	// 								axSLIDER_FLAG_RIGHT_ALIGN);

// 	// slider->SetBackgroundAlpha(0.2);

// 	// cout << "Size : " << size_x / 3.0 << endl;

// 	// slider = new axSlider(app, this, 
// 	// 					  axRect(60 + size_x / 3.0, 0, size_x / 3.0, rect.size.y), 
// 	// 					  axSliderEvents(), sld_info,
// 	// 					  axSLIDER_FLAG_VERTICAL | 
// 	// 					  axSLIDER_FLAG_CLICK_ANYWHERE | 
// 	// 					  axSLIDER_FLAG_RIGHT_ALIGN);
// 	// slider->SetBackgroundAlpha(0.2);

// 	// slider = new axSlider(app, this, 
// 	// 					  axRect(60 + 2.0 * size_x / 3.0, 0, size_x / 3.0+1, rect.size.y), 
// 	// 					  axSliderEvents(), sld_info,
// 	// 					  axSLIDER_FLAG_VERTICAL | 
// 	// 					  axSLIDER_FLAG_CLICK_ANYWHERE | 
// 	// 					  axSLIDER_FLAG_RIGHT_ALIGN);

// 	// slider->SetBackgroundAlpha(0.2);
// }


// void MidiVelocity::OnPaint()
// {
// 	axGC* gc = GetGC();
// 	axRect rect(GetRect());
// 	axRect rect0(axPoint(0, 0), rect.size);

// 	gc->SetColor(axColor(0.6, 0.6, 0.6), 1.0);
// 	gc->DrawRectangle(rect0);

// 	gc->SetColor(axColor(0.7, 0.7, 0.7), 1.0);

// 	for(int i = 0; i < 20; i++)
// 	{
// 		int y_pos = (i / double(20)) * (rect0.size.y);
// 		gc->DrawLine(axPoint(60, y_pos), axPoint(rect.size.x-14, y_pos));
// 	}

// 	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);

// 	for(int i = 0; i < 16+1; i++)
// 	{
// 		int x_pos = (i / double(16)) * (rect0.size.x - 60 - 14) + 60;
// 		gc->DrawLine(axPoint(x_pos, 0), axPoint(x_pos, rect0.size.y));
// 	}

// 	gc->DrawRectangleContour(rect0);
// }

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
			axPanel(app, parent, rect)
{

	axWaveform* wave = new axWaveform(app, this, 
						   axRect(20, 30, rect.size.x - 40, rect.size.y / 4.0),
						   axWaveformEvents(),
						   axWaveformInfo(axColor(0.0, 0.0, 0.0), // Waveform.
                    					  axColor(0.4, 0.4, 0.4), // Background.
                    					  axColor(0.5, 0.5, 0.5), // Lines.
                    					  axColor(0.0, 0.0, 0.0))); // Contour.
	// MidiPartitionTrack* track = new MidiPartitionTrack(app, this, axRect(0, 0, 20, rect.size.y));
}


void MidiPartition::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor("#FF5555"), 1.0);
	gc->DrawRectangle(rect0);

	// //-----------------------------------------
	// // lines
	// //-----------------------------------------
	// gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	// gc->DrawRectangle(axRect(50, 85, 15, 15));
	// //-----------------------------------------

	gc->SetColor(axColor(1.0, 1.0, 1.0), 1.0);
	gc->DrawRectangleContour(rect0);
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

	int x = 10, y = 10, xDelta = 20;

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

	LineSelection* lines = new LineSelection(app, this, axRect(70, 10, 420, 20));



	AddNewTrack("Kick", audio, 0);
	AddNewTrack("Snare", audio, 1);
	AddNewTrack("HiHat", audio, 2);
	AddNewTrack("OpenHat", audio, 3);	

	// MidiPartition* midiPart = new MidiPartition(app, this, axRect(500, 80, 420, 300));

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
											axRect(axPoint(10, 30), 
											_trackSize), 
											trackName, audio, num));
	}
	else
	{
		_midiTracks.push_back(new MidiTrack(GetApp(), this, 
											axRect(GetNextTrackPosition(), 
											_trackSize), 
											trackName, audio, num));
	}

	function<void (int)> min_fct(GetOnChangeTrackHeight());
	MidiTrackEvents evt(min_fct);
	_midiTracks[_midiTracks.size() - 1]->SetEvent(evt);
}

axPoint MidiSequencer::GetNextTrackPosition() const
{
	return _midiTracks[_midiTracks.size() - 1]->GetBottomLeftPosition();
}


void MidiSequencer::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.1, 0.1, 0.1), 1.0);
	gc->DrawRectangle(rect0);

	// //-----------------------------------------
	// // lines
	// //-----------------------------------------
	// gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	// gc->DrawRectangle(axRect(50, 85, 15, 15));
	// //-----------------------------------------

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);
}

int main()
{
	AudioMidiSeq* audio = new AudioMidiSeq();

	axApp app(axSize(1000, 400));
	MidiSequencer* midiSeq = new MidiSequencer(&app, nullptr, 
								   axRect(0, 0, 500, 400), audio);


	MidiPartition* synth = new MidiPartition(&app, nullptr, axRect(500, 10, 400, 400 - 20));

	audio->StartAudio();
	app.MainLoop();

	return 0;
}
