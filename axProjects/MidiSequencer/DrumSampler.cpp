#include "DrumSampler.h"

//-----------------------------------------------------------------------------
// SynthControl.
//-----------------------------------------------------------------------------
SynthControl::SynthControl(axApp* app, axWindow* parent, 
						   const axRect& rect, 
						   const string& snd_path) :
			axPanel(app, parent, rect)
{
	_bgImg = new axImage("synth_bg.png");

	//axButtonInfo btn_info(axColor(0.2, 0.2, 0.2),
	//	axColor(0.3, 0.3, 0.3),
	//	axColor(0.2, 0.2, 0.2),
	//	axColor(0.2, 0.2, 0.2),
	//	axColor(0.0, 0.0, 0.0),
	//	axColor(0.8, 0.8, 0.8));

	//function<void(axButtonMsg)> btnFct(GetOnOpenFile());


	//axButton* btn = new axButton(app, this,
	//	axRect(5, 5, 45, 20),
	//	axButtonEvents(btnFct),
	//	btn_info, "", "Open", axBUTTON_SINGLE_IMG | axBUTTON_IMG_RESIZE);

	axWaveformInfo waveinfo(axColor(0.7, 0.7, 0.7), // Waveform.
							axColor(0.4, 0.4, 0.4), // Background.
							axColor(0.5, 0.5, 0.5), // Lines.
							axColor(0.0, 0.0, 0.0));

	axWaveform* wave = new axWaveform(app, this,
									  axRect(20, 30, rect.size.x - 40, 80),
									  axWaveformEvents(),
									  waveinfo,
									  snd_path
									  ); // Contour.

	wave->SetBackgroundAlpha(0.0);

	//axNumberBoxInfo box_info(axColor(0.2, 0.2, 0.2),
	//	axColor(0.3, 0.3, 0.3),
	//	axColor(0.2, 0.2, 0.2),
	//	axColor(0.2, 0.2, 0.2),
	//	axColor(0.0, 0.0, 0.0),
	//	axColor(0.8, 0.8, 0.8));

	//// function<void (axButtonMsg)> btnFct(GetOnAddSeparation());


	//axNumberBox* box1 = new axNumberBox(app, this,
	//	axRect(20, 140, 40, 20),
	//	axNumberBoxEvents(),
	//	box_info);

	//axNumberBox* box2 = new axNumberBox(app, this,
	//	axRect(70, 140, 40, 20),
	//	axNumberBoxEvents(),
	//	box_info);

	//axNumberBox* box3 = new axNumberBox(app, this,
	//	axRect(120, 140, 40, 20),
	//	axNumberBoxEvents(),
	//	box_info);

	axKnobInfo knob_info(axColor(0.3, 0.3, 0.3),
		axColor("#AAAAAA"),
		axColor("#BBBBBB"),
		128,
		axSize(46, 46),
		"knob_dark.png",
		"knob_dark.png");

	axKnob* knob = new axKnob(app, this, axRect(19, 152, 46, 46),
		axKnobEvents(), knob_info);

	knob->SetBackgroundAlpha(0.0);

	axKnob* res = new axKnob(app, this, axRect(59, 152, 46, 46),
		axKnobEvents(), knob_info);
	res->SetBackgroundAlpha(0.0);

	axKnob* env = new axKnob(app, this, axRect(98, 152, 46, 46),
		axKnobEvents(), knob_info);
	env->SetBackgroundAlpha(0.0);
	//axKnob* knob2 = new axKnob(app, this, axRect(230, 130, 32, 32),
	//	axKnobEvents(), knob_info);

	//axKnob* knob3 = new axKnob(app, this, axRect(380, 130, 32, 32),
	//	axKnobEvents(), knob_info);

	//axKnob* knob4 = new axKnob(app, this, axRect(425, 130, 32, 32),
	//	axKnobEvents(), knob_info);

}

void SynthControl::OnOpenFile(const axButtonMsg& msg)
{
	// cout << "FILE" << endl;
	// string f = OpenFileDialog("/home/alexarse/Desktop/axLib/axProjects/FileDialog/main");
}

void SynthControl::OnPaint()
{
	axGC* gc = GetGC();
	//BlockDrawing();

	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	cout << "SYNTH : " << rect0.size.x << " " << rect0.size.y << endl;

	gc->SetColor(axColor(0.3, 0.3, 0.3), 1.0);
	gc->DrawRectangle(rect0);

	gc->DrawImage(_bgImg, axPoint(0, 0));

	//gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	//gc->DrawRectangleContour(rect0);

	// gc->SetColor(axColor(0.0, 1.0, 0.0), 1.0);
	// gc->DrawRectangle(axRect(rect0.position.x, rect0.position.y, 800, 30));

	//UnBlockDrawing();
}


//-----------------------------------------------------------------------------
DrumPad::DrumPad(axApp* app, axWindow* parent,
	const axRect& rect, const int& track_id, axEvtFunction(DrumPadMsg) click_evt) :
	axPanel(app, parent, rect)
{
	_clickEvent = click_evt;
	_trackId = track_id;
	_bgImg = new axImage("pad.png");

}

void DrumPad::OnMouseLeftDown(const axPoint& pos)
{
	if (_clickEvent)
	{
		DrumPadMsg msg(this, _trackId);
		_clickEvent(msg);
	}
}
//
//void SynthControl::OnOpenFile(const axButtonMsg& msg)
//{
//	// cout << "FILE" << endl;
//	// string f = OpenFileDialog("/home/alexarse/Desktop/axLib/axProjects/FileDialog/main");
//}

void DrumPad::OnPaint()
{
	axGC* gc = GetGC();
	//BlockDrawing();

	

	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	//cout << "PAD SIZE : " << rect0.size.x << " " << rect0.size.y << endl;
	gc->SetColor(axColor(0.6, 0.6, 0.6), 1.0);
	gc->DrawRectangle(rect0);

	gc->DrawImage(_bgImg, axPoint(0, 0));
	//gc->DrawImageResize(_bgImg, axPoint(0, 0), axSize(rect0.size));

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->SetFontSize(12);
	gc->DrawStringAlignedCenter("Kick", axRect(0, 0, rect0.size.x, 20));

	//gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	//gc->DrawRectangleContour(rect0);

	// gc->SetColor(axColor(0.0, 1.0, 0.0), 1.0);
	// gc->DrawRectangle(axRect(rect0.position.x, rect0.position.y, 800, 30));

	//UnBlockDrawing();
}



DrumSampler::DrumSampler(axApp* app, axWindow* parent, const axRect& rect, Audio* audio) :
			 axPanel(app, parent, rect)
{
	_topBar = new axImage("tool.png");

	_audio = static_cast<AudioMidiSeq*>(audio);
	cout << "NUMBER OF AUDIO TRACK " << _audio->GetNumberOfTrack() << endl;
	_synths.resize(_audio->GetNumberOfTrack());
	_pads.resize(8);

	int x = 7;
	int pad_size = (rect.size.x - (8+1) * 5.0) / double(_pads.size());

	axEvtFunction(DrumPadMsg) padClickFct(GetOnDrumPadClick());
	for (int i = 0; i < 8; i++)
	{
		_pads[i] = new DrumPad(app, this, axRect(x, 25, pad_size, 60), 
							   i, padClickFct);
		x += pad_size + 5;
	}


	for (int i = 0; i < _synths.size(); i++)
	{
		_synths[i] = new SynthControl(app, this, 
						axRect(7, 93, rect.size.x - 14, 220/* rect.size.y - 80*/),
						_audio->GetSoundPath(i));

		_synths[i]->Hide();
	}

	_synths[0]->Show();
}

void DrumSampler::OnDrumPadClick(const DrumPadMsg& msg)
{
	cout << "Drum pad click " << msg.GetMsg() << endl;
	for (int i = 0; i < _synths.size(); i++)
	{
		//if (_synths[i]->IsShown())
		//{
			_synths[i]->Hide();
		//}
	}

	_synths[msg.GetMsg()]->Show();

}

void DrumSampler::OnPaint()
{
	axGC* gc = GetGC();
	//BlockDrawing();

	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.2, 0.2, 0.2), 1.0);
	gc->DrawRectangle(rect0);

	gc->DrawImage(_topBar, axPoint(0, 0));

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);
}