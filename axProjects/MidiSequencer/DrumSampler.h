#ifndef __DRUM_SAMPLER__
#define __DRUM_SAMPLER__

#include "axLib.h"
#include <vector>
#include <string>
#include <cmath>

#include "axAudio.h"
#include "AudioWaveform.h"

class SynthControl : public axPanel
{
public:
	SynthControl(axApp* app,
		axWindow* parent,
		const axRect& rect, const string& snd_path);

	axEVENT(axButtonMsg, OnOpenFile);

private:
	// Events.
	axImage* _bgImg;

	void OnOpenFile(const axButtonMsg& msg);
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
	DrumPad(axApp* app,
		axWindow* parent,
		const axRect& rect, 
		const int& track_id,
		axEvtFunction(DrumPadMsg) click_evt);

	//axEVENT(axButtonMsg, OnOpenFile);

private:
	axImage* _bgImg;
	int _trackId;

	axEvtFunction(DrumPadMsg) _clickEvent;
	// Events.

	//void OnOpenFile(const axButtonMsg& msg);
	virtual void OnMouseLeftDown(const axPoint& pos);
	virtual void OnPaint();
};

class DrumSampler : public axPanel
{
public:
	DrumSampler(axApp* app,
		axWindow* parent,
		const axRect& rect, Audio* audio);

	//axEVENT(axButtonMsg, OnOpenFile);

	axEVENT(DrumPadMsg, OnDrumPadClick);

private:
	AudioMidiSeq* _audio;
	vector<SynthControl*> _synths;
	vector<DrumPad*> _pads;

	axImage* _topBar;

	
	//function<void(axButtonMsg)> button_click;

	// Events.

	void OnDrumPadClick(const DrumPadMsg& msg);
	virtual void OnPaint();
};


#endif // __DRUM_SAMPLER__