#ifndef __MIDI_SEUENCER__
#define __MIDI_SEUENCER__

#include "axLib.h"
//#include "axImage.h"

#include <vector>
#include <string>
#include <cmath>
#include "MidiTrack.h"
#include "axAudio.h"
#include "AudioWaveform.h"


class MidiPartitionTrack: public axPanel
{
public:
	MidiPartitionTrack(axApp* app, 
				   axWindow* parent, 
				   const axRect& rect);

private:
	int _nPart;

	// Events.
	virtual void OnPaint();
};

struct CircleInfo
{
	CircleInfo()
	{
		parent_bar = -1;
		parent_section = -1;
		parent_circle = -1;
	}

	int parent_bar;
	int parent_section;
	int parent_circle;
};

// struct SectionInfo
// {
// 	int nCircle;

// 	vector<CircleInfo> circles;

// };

class MidiPartition: public axPanel
{
public:
	MidiPartition(axApp* app, 
				   axWindow* parent, 
				   const axRect& rect);

private:

	axImage* _circle, *_selectedCircle;

	// axPoint GetCirclePosition(const int& bar_index, 
	// 					  const int& nSection, 
	// 					  const int& section_index,
	// 					  const int& nCircle, 
	// 					  const int& circle_index);


	vector<int> _bars;

	int _heightlighted_bar;

	int _nSlice;
	int _heighlighted_circle;

	int _selected_bar, _selected_circle;

	void DrawSelectedCircle(axGC* gc, const axRect& rect0);
	void DrawLines(axGC* gc, const axRect& rect0);
	void DrawCircles(axGC* gc, const axRect& rect0);
	void DrawHeighlightedCircle(axGC* gc, const axRect& rect0);
	// vector<vector<int>> _circles;
	// vector<vector<pair<int,CircleInfo>>> _circles;

	// Events.
	virtual void OnPaint();
	virtual void OnMouseMotion(const axPoint& mouse);
	virtual void OnMouseLeave();
};



class SynthControl: public axPanel
{
public:
	SynthControl(axApp* app, 
				   axWindow* parent, 
				   const axRect& rect);

	axEVENT(axButtonMsg, OnOpenFile);

private:
	// Events.

	void OnOpenFile(const axButtonMsg& msg);
	virtual void OnPaint();
};

//-----------------------------------------------------------------------------
// Desktop app.
//-----------------------------------------------------------------------------
class MidiSequencer: public axPanel
{
public:
	MidiSequencer(axApp* app, axWindow* parent, const axRect& rect, Audio* audio);

	void AddNewTrack(const string& trackName, Audio* audio, int num);

	void SetTrackResizeFct(axEvtFunction(int) fct)
	{
		_track_resize_parent_fct = fct;
	}

	void SetPreset(DrumMachinePreset* preset);
	

	axEVENT(axButtonMsg, OnColorChange);
	axEVENT(int, OnChangeTrackHeight);

private:
	vector<MidiTrack*> _midiTracks;
	ColorChoice _choice;


	axPoint GetNextTrackPosition() const;
	axSize _trackSize;
	vector<axButton*> btns;

	axImage* _side_img;

	axEvtFunction(int) _track_resize_parent_fct;

	void OnChangeTrackHeight(const int& msg);

	void OnColorChange(const axButtonMsg& msg);
	// vector<axButton*> _btns;
	// axImage* _bgImg;

	virtual void OnPaint();
};

class DrumMachine: public axPanel
{
public:
	DrumMachine(axApp* app, 
				   axWindow* parent, 
				   const axRect& rect, Audio* audio);

	axEVENT(int, OnChangeTrackHeight);
	axEVENT(axButtonMsg, OnChangeTemplate);

	void ExecApplication(const string& app_name);

	void SetPreset(const string& file_path);

private:
	

	SynthControl* _synth;
	MidiSequencer* _midiSeq;
	MidiPartition* _midiPartition;
	Audio* _audio;

	void OnChangeTrackHeight(const int& msg);
	void OnChangeTemplate(const axButtonMsg& msg);


	// axEvtFunction(int) _evt_track_size;


	// Events.
	virtual void OnPaint();
};

class ScrollDrumMachine: public axPanel
{
public:
	ScrollDrumMachine(axApp* app, 
				   axWindow* parent, 
				   const axRect& rect, Audio* audio);

	axEVENT(axScrollBarMsg, OnScroll);
private:
	axScrollBar* scroll_bar;
	DrumMachine* _drum;
	axImage* _side_img;
	int _last_delta;

	void OnScroll(const axScrollBarMsg& msg);
	// Events.
	virtual void OnPaint();
};


#endif // __MIDI_SEUENCER__