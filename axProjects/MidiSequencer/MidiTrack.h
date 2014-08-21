#ifndef __MIDI_TRACK__
#define __MIDI_TRACK__

#include "axLib.h"
#include <vector>
#include <string>
#include <cmath>

#include "axAudio.h"

//#include <sys/types.h>
//#include <unistd.h>

enum ColorChoice
{
	CHOICE_RED,
	CHOICE_GREEN,
	CHOICE_BLUE,
	CHOICE_NUM
};

string GetCurrentAppDirectory();

void ExecApplication(const string& app_name);

struct MultipleSliderMsg
{
	int index;
	double value;
	int bar_index;
};

//-----------------------------------------------------------------------------
// MultipleSlider.
//-----------------------------------------------------------------------------
class MultipleSlider: public axPanel
{
public:
	MultipleSlider(axApp* app, 
				   axWindow* parent, 
				   const axRect& rect, 
				   const axColor& bgColor,
				   const int& barIndex,
				   axEvtFunction(MultipleSliderMsg)& slider_fct);

	void SetNumberOfSlider(const int& nb);

	void SetValue(const int& index, const double& value);

	axEVENT(axSliderMsg, OnSlider1Move);
	axEVENT(axSliderMsg, OnSlider2Move);
	axEVENT(axSliderMsg, OnSlider3Move);

private:
		int _nSlider;
		axColor _bgColor;
		axSlider* sliders[3]; 
		int _barIndex;

		axEvtFunction(MultipleSliderMsg) _slider_fct;

	// Events.
	virtual void OnPaint();

	void OnSlider1Move(const axSliderMsg& msg);
	void OnSlider2Move(const axSliderMsg& msg);
	void OnSlider3Move(const axSliderMsg& msg);
};

//-----------------------------------------------------------------------------
// MidiTrackName
//-----------------------------------------------------------------------------
class MidiTrackName: public axPanel
{
public:
	MidiTrackName(axApp* app, axWindow* parent, const axRect& rect, const string& name);

private:
	string _trackName;

	virtual void OnPaint();
};

struct MidiNoteParams
{
	MidiNoteParams():
		velocities(3, 0.7),
		actives(3, false),
		colors(3, axColor(0.8, 0.0, 0.0))
	{
		active = false;
		velocity = 0.0;
	}

	bool active;
	float velocity;

	vector<bool> actives;
	vector<float> velocities;
	vector<axColor> colors;
};

class MidiVelocity: public axPanel
{
public:
	MidiVelocity(axApp* app, axWindow* parent, 
				 const axRect& rect, 
				 axEvtFunction(MultipleSliderMsg) fct,
				 axEvtFunction(axNumberBoxMsg) sfct);

	void SetNumberOfSlider(const int& nb)
	{
		for(int i = 0; i < _sliders.size(); i++)
		{
			_sliders[i]->SetNumberOfSlider(nb);
		}
	}

	void SetPreset(TrackInfo* info);

	axEVENT(MultipleSliderMsg, OnChangeVelocity);
	axEVENT(axNumberBoxMsg, OnStandardDeviation);

private:

	vector<MultipleSlider*> _sliders;

	axEvtFunction(MultipleSliderMsg) _velocity_fct;
	axEvtFunction(axNumberBoxMsg) _standard_deviation_fct;

	void OnChangeVelocity(const MultipleSliderMsg& vel);

	void OnStandardDeviation(const axNumberBoxMsg& msg);




	virtual void OnPaint();
};

//-----------------------------------------------------------------------------
// MidiTrackSequence
//-----------------------------------------------------------------------------
class MidiTrackSequence: public axPanel
{
public:
	MidiTrackSequence(axApp* app, axWindow* parent, const axRect& rect, Audio* audio, int num);

	void SetNumberOfSubTrack(const int& n)
	{
		_nSubTrack = n;
		Update();
	}

	void SetColorSelection(const ColorChoice& color);

	void SetPreset(TrackInfo* info);

	void SetVelocity(const int& index, const int& subtrack, const float& value);

private:
	vector<MidiNoteParams> _notes;

	int _nbars, // Number of bars.
		_highLightIndex, // Mouse over index.
		_track_number, // To call audio functions.
		_nSubTrack, // Number of subtracks.
		_selectedSeparationNumber; // Index of subtrack selected.

	// Color probability chosen.
	ColorChoice _choice;

	// Images to put over rectangle.
	axImage* _bgImg, *_hightlighImg; 

	// Background colors when no notes on.
	axColor _bgColors[4]; 

	// Notes color.
	axColor _colorChoice[CHOICE_NUM]; 

	// Probability value associated with color.
	float _probabilityValues[CHOICE_NUM]; 

	// Colors of the background order in sequence.
	vector<axColor*> _sequenceColors;

	// Rectangles of the background order in sequence.
	vector<axRect> _gridRects;
	
	AudioMidiSeq* _audio;

	// Events.
	virtual void OnPaint();
	virtual void OnMouseMotion(const axPoint& mousePos);
	virtual void OnMouseLeftDown(const axPoint& mousePos);
	virtual void OnMouseLeave();
};

//-----------------------------------------------------------------------------
// LineSelection.
//-----------------------------------------------------------------------------
class LineSelection: public axPanel
{
public:
	LineSelection(axApp* app, axWindow* parent, const axRect& rect);

private:
	// Events.
	virtual void OnPaint();
};


struct MidiTrackEvents
{
	std::function<void (int)> minimize;
	
	MidiTrackEvents(){}
	MidiTrackEvents(std::function<void (int)>& fct){ minimize = fct; }
};


//-----------------------------------------------------------------------------
// MidiTrack.
//-----------------------------------------------------------------------------
class MidiTrack: public axPanel
{
public:
	MidiTrack(axApp* app, axWindow* parent, 
			  const axRect& rect, 
			  const string& trackName, 
			  Audio* audio, int track_number);

	void SetEvent(MidiTrackEvents& evt)
	{
		_evt = evt;
	}

	void SetBtnClickEvent(axEvtFunction(int) fct)
	{
		_btn_evt_fct = fct;
	}

	void SetPreset(TrackInfo* info);


	void SetColorSelection(const ColorChoice& choice);

	axEVENT(axButtonMsg, OnAddSeparation);
	axEVENT(axButtonMsg, OnRemoveSeparation);
	axEVENT(axButtonMsg, OnMinimize);
	axEVENT(MultipleSliderMsg, OnVelocity);
	axEVENT(axNumberBoxMsg, OnStandardDeviation);
	
	
private:
	MidiTrackName* _trackName;
	MidiTrackSequence* _trackSeq;
	MidiVelocity* _velocity;
	MidiTrackEvents _evt;
	AudioMidiSeq* _audio;

	axEvtFunction(int) _btn_evt_fct;

	axButton *_addBtn, *_removeBtn;

	int _nSubTrack;

	int _track_number;

	// Events.
	virtual void OnPaint();

	void OnAddSeparation(const axButtonMsg& msg);
	void OnRemoveSeparation(const axButtonMsg& msg);
	void OnMinimize(const axButtonMsg& msg);

	void OnVelocity(const MultipleSliderMsg& msg);
	void OnStandardDeviation(const axNumberBoxMsg& msg);

};





#endif // __MIDI_TRACK__