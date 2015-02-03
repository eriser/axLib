#include "KrakenMidi.h"
#include "KrakenAudio.h"

#ifndef ST_RATIO
#define ST_RATIO (1.0594630943592952)
#endif

KrakenMidi::KrakenMidi():
axMidi()
{
//    double c5 = 220.0 * pow(ST_RATIO, 3);
//    c0 = c5 * pow(0.5, 5);
}

void KrakenMidi::OnMidiNoteOn(const axMidiNoteMsg& msg)
{
    std::cout << "Note on : " << msg.GetNote() << std::endl;
    
//    int midiNote = (int)msg.GetNote();
//    double freq = c0 * pow(ST_RATIO, midiNote);
    
    KrakenAudio::GetInstance()->TriggerNote(msg.GetNote(), msg.GetVelocity());
}

void KrakenMidi::OnMidiNoteOff(const axMidiNoteMsg& msg)
{
    
}