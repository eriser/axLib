#include "DrumSampler.h"

//-----------------------------------------------------------------------------
// SynthControl.
//-----------------------------------------------------------------------------
SynthControl::SynthControl(axWindow* parent,
						const axRect& rect,
						const string& snd_path,
						axAudio* audio,
						const int& track_num) :
	axPanel(parent, rect),
	_trackNum(track_num)
{
	_audio = static_cast<AudioMidiSeq*>(audio);
	_bgImg = new axImage("/Users/alexarse/Project/axLib/axProjects/MidiSequencer/synth_bg.png");

    axButton::Info btn_info(axColor(0.2, 0.2, 0.2),
		axColor(0.3, 0.3, 0.3),
		axColor(0.2, 0.2, 0.2),
		axColor(0.2, 0.2, 0.2),
		axColor(0.0, 0.0, 0.0),
		axColor(0.8, 0.8, 0.8));

    axButton::Events btnEvents;
    btnEvents.button_click = GetOnOpenFile();
	axButton* btn = new axButton(this, axRect(20 + 42, 5, 60, 20),
                                 btnEvents, btn_info, "", "Open");

    
    axWaveform::Info waveinfo(axColor(0.3, 0.3, 0.3, 0.0), // Background.
                              axColor(0.6, 0.6, 0.6), // Waveform.
                              axColor(0.0, 0.0, 0.0), // Contour.
                              axColor(0.0, 0.0, 0.0), // Grid main.
                              axColor(0.5, 0.5, 0.5), // Grid second.
                              axColor(0.5, 0.5, 0.5), // Middle line.
                              axColor(0.5, 0.5, 0.5)); // Env.

    _wave = new axWaveform(this, axRect(20 + 42, 30, 405, 80), waveinfo);
    _wave->SetAudioBuffer(_audio->GetAudioBufferFromTrack(_trackNum));

	axKnob::Info knob_info(axColor(0.3, 0.3, 0.3, 0.0),
						 axColor(0.3, 0.3, 0.3, 0.0),
						 axColor(0.3, 0.3, 0.3, 0.0),
						 128,
						 axSize(46, 46),
						 "/Users/alexarse/Project/axLib/axProjects/MidiSequencer/knob_dark.png",
						 "/Users/alexarse/Project/axLib/axProjects/MidiSequencer/knob_dark.png");

	axKnob::Events freq_evt;
    freq_evt.value_change = GetOnFilterFrequency();
	axKnob* knob = new axKnob(this, axRect(19 + 42, 156, 46, 46),
							  freq_evt, knob_info);

//	knob->SetBackgroundAlpha(0.0);
	knob->SetValue(1.0);

	freq_evt.value_change = GetOnFilterRes();
	axKnob* res = new axKnob(this, axRect(59 + 42, 156, 46, 46),
							 freq_evt, knob_info);
//	res->SetBackgroundAlpha(0.0);
	res->SetValue(0.00707);

	freq_evt.value_change = GetOnFilterEnvelope();
	axKnob* env = new axKnob(this, axRect(98 + 42, 156, 46, 46),
		freq_evt, knob_info);
//	env->SetBackgroundAlpha(0.0);


	freq_evt.value_change = GetOnAttack();
	axKnob* att = new axKnob(this, axRect(163 + 42, 156, 46, 46),
		freq_evt, knob_info);
//	att->SetBackgroundAlpha(0.0);

	freq_evt.value_change = GetOnDecay();
	axKnob* dec = new axKnob(this, axRect(202 + 42, 156, 46, 46),
		freq_evt, knob_info);
//	dec->SetBackgroundAlpha(0.0);
	dec->SetValue(1.0);

	freq_evt.value_change = GetOnTuning();
	axKnob* pitch = new axKnob(this, axRect(269 + 42, 156, 46, 46),
		freq_evt, knob_info);
//	pitch->SetBackgroundAlpha(0.0);
	pitch->SetValue(0.5);

	freq_evt.value_change = GetOnTuningEnv();
	axKnob* env_pitch = new axKnob(this, axRect(308 + 42, 156, 46, 46),
		freq_evt, knob_info);
//	env_pitch->SetBackgroundAlpha(0.0);
	env_pitch->SetValue(0.0);

	freq_evt.value_change = GetOnGain();
	axKnob* gain = new axKnob(this, axRect(376 + 42, 156, 46, 46),
		freq_evt, knob_info);
//	gain->SetBackgroundAlpha(0.0);
	gain->SetValue(0.5);
}

void SynthControl::OnOpenFile(const axButton::Msg& msg)
{
	string f;
	if ((f = GetApp()->OpenFileDialog()) != "")
	{
		_audio->GetAudioBufferFromTrack(_trackNum)->OpenSoundFile(f);
        _wave->SetAudioBuffer(_audio->GetAudioBufferFromTrack(_trackNum));
		//_wave->SetBufferPtr(_audio->GetAudioBufferFromTrack(_trackNum));
	}
}

void SynthControl::OnFilterFrequency(const axKnob::Msg& msg)
{
	axFloat freq = msg.GetValue() * (20000.0 - 20.0) + 20.0;
	_audio->GetAudioTrack(_trackNum)->SetFilterFrequency(freq);
}

void SynthControl::OnFilterRes(const axKnob::Msg& msg)
{
	axFloat q = msg.GetValue() * 5.0;
	_audio->GetAudioTrack(_trackNum)->SetFilterRes(q);
}

void SynthControl::OnFilterEnvelope(const axKnob::Msg& msg)
{
	_audio->GetAudioTrack(_trackNum)->SetFilterFrequencyEnvelopeAmount(msg.GetValue());
}

void SynthControl::OnAttack(const axKnob::Msg& msg)
{
	_audio->GetAudioTrack(_trackNum)->SetAttack(msg.GetValue());
}

void SynthControl::OnDecay(const axKnob::Msg& msg)
{
	_audio->GetAudioTrack(_trackNum)->SetDecay(msg.GetValue());
}

void SynthControl::OnTuning(const axKnob::Msg& msg)
{
	axFloat value = (msg.GetValue() * 2.0);
	_audio->GetAudioTrack(_trackNum)->SetSpeed(value);
}

void SynthControl::OnTuningEnv(const axKnob::Msg& msg)
{
	_audio->GetAudioTrack(_trackNum)->SetPitchEnvelopeAmount(msg.GetValue());
}


void SynthControl::OnGain(const axKnob::Msg& msg)
{
	_audio->GetAudioTrack(_trackNum)->SetGain(msg.GetValue());
}

void SynthControl::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.1, 0.1, 0.1), 1.0);
	gc->DrawRectangle(rect0);

	gc->DrawImage(_bgImg, axPoint(42, 0));
}


//-----------------------------------------------------------------------------
DrumPad::DrumPad(axWindow* parent,
	const axRect& rect, const int& track_id, axEvtFunction(DrumPadMsg) click_evt) :
	axPanel(parent, rect)
{
	_trackName = "";
	_clickEvent = click_evt;
	_trackId = track_id;
	_bgImg = new axImage("/Users/alexarse/Project/axLib/axProjects/MidiSequencer/pad.png");
	_selected = false;
    
    _font = new axFont(0);

}

void DrumPad::SetTrackName(const string& name)
{
	_trackName = name;
}

void DrumPad::SetSelected()
{
	if (_selected == false)
	{
		_selected = true;
		Update();
	}
	
}
void DrumPad::SetUnselected()
{
	if (_selected == true)
	{
		_selected = false;
		Update();
	}
}

void DrumPad::OnMouseLeftDown(const axPoint& pos)
{
	if (_clickEvent)
	{
		DrumPadMsg msg(this, _trackId);
		_clickEvent(msg);
	}
}

void DrumPad::OnMouseEnter()
{
	_highlight = true;
	Update();
}

void DrumPad::OnMouseLeave()
{
	_highlight = false;
	Update();
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
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

//    gc->DrawImage(_bgImg, axPoint(0, 0));
    
	if (_selected)
	{
		gc->SetColor(axColor(0.95, 0.4, 0.08, 1.0));
	}
	else
	{
		if (_highlight)
		{
			gc->SetColor(axColor(0.6, 0.6, 0.6, 1.0));
			
		}
		else
		{
			gc->SetColor(axColor(0.4, 0.4, 0.4, 1.0));
		}
	}

	
	gc->DrawRectangle(rect0);

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
//	gc->SetFontSize(12);
	gc->DrawStringAlignedCenter(*_font, _trackName, axRect(0, 0, rect0.size.x, 20));
}



DrumSampler::DrumSampler(axWindow* parent, const axRect& rect, axAudio* audio) :
			 axPanel(parent, rect)
{
	//_topBar = new axImage("/Users/alexarse/Project/axLib/axProjects/MidiSequencer/tool.png");

	_audio = static_cast<AudioMidiSeq*>(audio);
	cout << "NUMBER OF AUDIO TRACK " << _audio->GetNumberOfTrack() << endl;
	_synths.resize(_audio->GetNumberOfTrack());
	_pads.resize(8);

	int x = 7;
	int pad_size = (rect.size.x - (8+1) * 5.0) / double(_pads.size());

	axEvtFunction(DrumPadMsg) padClickFct(GetOnDrumPadClick());
    
    
	for (int i = 0; i < 8; i++)
	{
		_pads[i] = new DrumPad(this, axRect(x, 25, pad_size, 60),
							   i, padClickFct);
		x += pad_size + 5;
		_pads[i]->SetTrackName(_audio->GetAudioTrack(i)->GetTrackName());
	}

    
	for (int i = 0; i < _synths.size(); i++)
	{
		_synths[i] = new SynthControl(this, 
						axRect(7, 93, rect.size.x - 14, 220/* rect.size.y - 80*/),
						_audio->GetSoundPath(i), audio, i);

		_synths[i]->Hide();
	}

	_synths[0]->Show();
	_pads[0]->SetSelected();
}

void DrumSampler::OnDrumPadClick(const DrumPadMsg& msg)
{
	cout << "Drum pad click " << msg.GetMsg() << endl;
	for (int i = 0; i < _synths.size(); i++)
	{
		//if (_synths[i]->IsShown())
		//{
			_synths[i]->Hide();
			_pads[i]->SetUnselected();
		//}
	}

	_synths[msg.GetMsg()]->Show();
	_pads[msg.GetMsg()]->SetSelected();

}

void DrumSampler::OnPaint()
{    
	axGC* gc = GetGC();
	//BlockDrawing();

	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.2, 0.2, 0.2), 1.0);
	gc->DrawRectangle(rect0);

    gc->DrawRectangleColorFade(axRect(0, 0, rect0.size.x, 20),
                               axColor(0.15, 0.15, 0.15),
                               axColor(0.1, 0.1, 0.1));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(axRect(0, 0, rect0.size.x, 20));
    
    gc->SetColor(axColor(0.65, 0.65, 0.65));
    axFont font(0);
    font.SetFontSize(10);
    gc->DrawString(font, "SAMPLES", axPoint(10, 2));

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);
}