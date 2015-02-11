#ifndef __AX_MIDI__
#define __AX_MIDI__

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
//#include "axUtils.h"

#include "RtMidi.h"

class axMidiNoteMsg //: public axMsg
{
public:
    axMidiNoteMsg(const int& note, const int& velocity):
    _note(note), _velocity(velocity)
    {
    }
    
    int GetNote() const
    {
        return _note;
    }
    
    int GetVelocity() const
    {
        return _velocity;
    }

private:
    int _note, _velocity;
};

class axMidi
{
public:
	axMidi();
	~axMidi();

    
protected:
    virtual void OnMidiNoteOn(const axMidiNoteMsg& msg);
    virtual void OnMidiNoteOff(const axMidiNoteMsg& msg);
    
private:
    RtMidiIn* _midiInHandle;
    
    static void MidiCallBack(double deltatime,
                             std::vector<unsigned char>* message,
                             void* userData);
    
    
};
#endif // __AX_MIDI__