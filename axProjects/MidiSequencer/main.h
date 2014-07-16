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


class MidiPartition: public axPanel
{
public:
	MidiPartition(axApp* app, 
				   axWindow* parent, 
				   const axRect& rect);

private:
	// Events.
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

	axEVENT(axButtonMsg, OnColorChange);
	axEVENT(int, OnChangeTrackHeight);

private:
	vector<MidiTrack*> _midiTracks;
	ColorChoice _choice;


	axPoint GetNextTrackPosition() const;
	axSize _trackSize;
	vector<axButton*> btns;

	void OnChangeTrackHeight(const int& msg)
	{
		for(int i = 1; i < _midiTracks.size(); i++)
		{
			_midiTracks[i]->SetPosition(_midiTracks[i-1]->GetBottomLeftPosition());
		}
	}

	void OnColorChange(const axButtonMsg& msg);
	// vector<axButton*> _btns;
	// axImage* _bgImg;

	virtual void OnPaint();
};