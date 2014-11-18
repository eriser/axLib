//
//  SoundEditorAudio.h
//  SoundEditorAudio
//
//  Created by Alexandre Arsenault on 2014-11-14.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#ifndef __SoundEditorAudio__
#define __SoundEditorAudio__

#include "axAudio.h"
#include "axLib.h"

class axAudioBuffer;
class axAudioBufferPlayer;

class axAudioPlayerMsg
{
public:
    axAudioPlayerMsg(const double& pos, const double& vol)
    {
        position = pos;
        volume = vol;
    }
    
    double GetPosition() const
    {
        return position;
    }
    
    double GetVolume() const
    {
        return volume;
    }
    
private:
    double position;
    double volume;
};


class SoundEditorAudio: public axAudio
{
public:
    static SoundEditorAudio* GetInstance();
    
    
    axEVENT(int, OnPlay);
    axEVENT(std::string, OnChangeFilePath);
    
    void SetPlayingPositionEvent(axEvtFunction(axAudioPlayerMsg) fct);
    void SetSoundFilePath(const std::string& path);
    
    axAudioBuffer* GetSoundBuffer();
    
private:
    SoundEditorAudio();
    static SoundEditorAudio* _instance;
    
    axEvtFunction(axAudioPlayerMsg) _playingPositionEvt;
    double _secToSendPlayingEvt;
    static const double PLAYING_POSITION_TIME_INTERVAL;
    
    axAudioBuffer* _sndBuffer;
    axAudioBufferPlayer* _bufferPlayer;
    
    // Events.
    void OnPlay(const int& msg);
    void OnChangeFilePath(const std::string& msg);
    
    
    virtual int CallbackAudio(const float* input,
                              float* output,
                              unsigned long frameCount);
};

#endif // __SoundEditorAudio__