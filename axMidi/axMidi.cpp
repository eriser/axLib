#include "axMidi.h"

axMidi::axMidi()
{
    _midiInHandle = new RtMidiIn();
    
    // Check available ports.
    unsigned int nPorts = _midiInHandle->getPortCount();
    
    if ( nPorts == 0 )
    {
        std::cout << "No ports available!\n";
        
        /// @todo Trhow exception.
    }
    
    _midiInHandle->openPort( 0 );
    
    // Set our callback function.  This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue.
    _midiInHandle->setCallback(&MidiCallBack, (void*) this);
    
    // Don't ignore sysex, timing, or active sensing messages.
    _midiInHandle->ignoreTypes(false, false, false);

//cleanup:
//    delete midiin;
}

axMidi::~axMidi()
{

}

void axMidi::OnMidiNoteOn(const axMidiNoteMsg& msg)
{
    
}

void axMidi::OnMidiNoteOff(const axMidiNoteMsg& msg)
{
    
}

void axMidi::MidiCallBack(double deltatime,
                          std::vector<unsigned char>* message,
                          void* userData)
{
    unsigned int nBytes = message->size();
    
    if(nBytes >= 3)
    {
        axMidiNoteMsg midiMsg(message->at(1), message->at(2));
        axMidi* midi = static_cast<axMidi*>(userData);
        
        if(message->at(0) == 144)
        {
            
            midi->OnMidiNoteOn(midiMsg);
//            std::cout << "Note on : " << (int)message->at(1) << " velocity : " << (int)message->at(2) << std::endl;
        }
        else if(message->at(0))
        {
            midi->OnMidiNoteOff(midiMsg);
//            std::cout << "Note off : " << (int)message->at(1) << " velocity : " << (int)message->at(2) << std::endl;
        }
    }
}