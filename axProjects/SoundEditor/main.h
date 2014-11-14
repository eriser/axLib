#ifndef __MIDI_SEUENCER__
#define __MIDI_SEUENCER__

#include "axLib.h"

class SoundEditor: public axPanel
{
public:
	SoundEditor(axApp* app,
                axWindow* parent,
                const axRect& rect);
private:

	// Events.
	virtual void OnPaint();
};


class axMain
{
public:
    axMain()
    {
    }
    
    static void MainEntryPoint(axApp* app)
    {
        SoundEditor* sound_editor = new SoundEditor(app,
                                                    nullptr,
                                                    axRect(0, 0, 600, 400));
    }
};


#endif // __MIDI_SEUENCER__