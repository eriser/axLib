#ifndef __DRUM_SAMPLER__
#define __DRUM_SAMPLER__

#include "axLib.h"
#include <vector>
#include <string>
#include <cmath>

#include "AudioDrumMachine.h"
//#include "AudioWaveform.h"
#include "axWaveform.h"

class SynthControl : public axPanel
{
public:
	SynthControl(axWindow* parent,
				 const axRect& rect, 
				 const string& snd_path,
				 axAudio* audio,
				 const int& track_num);



private:
	// Events.
	axImage* _bgImg;
	AudioMidiSeq* _audio;
	int _trackNum;
	axWaveform* _wave;

    axEVENT_ACCESSOR(axButton::Msg, OnOpenFile);
    axEVENT_ACCESSOR(axKnob::Msg, OnFilterFrequency);
    axEVENT_ACCESSOR(axKnob::Msg, OnFilterRes);
    axEVENT_ACCESSOR(axKnob::Msg, OnFilterEnvelope);
    axEVENT_ACCESSOR(axKnob::Msg, OnAttack);
    axEVENT_ACCESSOR(axKnob::Msg, OnDecay);
    axEVENT_ACCESSOR(axKnob::Msg, OnGain);
    axEVENT_ACCESSOR(axKnob::Msg, OnTuning);
    axEVENT_ACCESSOR(axKnob::Msg, OnTuningEnv);
	void OnOpenFile(const axButton::Msg& msg);
	void OnFilterFrequency(const axKnob::Msg& msg);
	void OnFilterRes(const axKnob::Msg& msg);
	void OnFilterEnvelope(const axKnob::Msg& msg);
	void OnAttack(const axKnob::Msg& msg);
	void OnDecay(const axKnob::Msg& msg);
	void OnGain(const axKnob::Msg& msg);
	void OnTuning(const axKnob::Msg& msg);
	void OnTuningEnv(const axKnob::Msg& msg);
	virtual void OnPaint();


};

class DrumPad;

class DrumPadMsg
{
public:
	DrumPadMsg()
	{
		_sender = nullptr;
	}

	DrumPadMsg(DrumPad* sender, const int& msg)
	{
		_sender = sender;
		_msg = msg;
	}

	DrumPad* GetSender() const
	{
		return _sender;
	}

	int GetMsg() const
	{
		return _msg;
	}

private:
	DrumPad* _sender;
	int _msg;
};

class DrumPad : public axPanel
{
public:
	DrumPad(axWindow* parent,
		const axRect& rect, 
		const int& track_id,
		axEvtFunction(DrumPadMsg) click_evt);

	void SetTrackName(const string& name);
	void SetSelected();
	void SetUnselected();

	//axEVENT(axButtonMsg, OnOpenFile);

private:
	axImage* _bgImg;
	int _trackId;
	string _trackName;
	bool _highlight, _selected;
    axFont* _font;

	axEvtFunction(DrumPadMsg) _clickEvent;
	// Events.

	//void OnOpenFile(const axButtonMsg& msg);
	virtual void OnMouseLeftDown(const axPoint& pos);
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void OnPaint();
};

class DrumSampler : public axPanel
{
public:
	DrumSampler(axWindow* parent,
		const axRect& rect, axAudio* audio);

	//axEVENT(axButtonMsg, OnOpenFile);

	axEVENT(DrumPadMsg, OnDrumPadClick);

private:
	AudioMidiSeq* _audio;
	vector<SynthControl*> _synths;
	vector<DrumPad*> _pads;

	axImage* _topBar;

	// Events.

	void OnDrumPadClick(const DrumPadMsg& msg);
	virtual void OnPaint();
};


#endif // __DRUM_SAMPLER__