#ifndef __AX_AUDIO__
#define __AX_AUDIO__

//------------------------------------------------------------------
// On linux : if you get this error:
// bt_audio_service_open: connect() failed: Connection refused (111)
// Solution : sudo apt-get purge bluez-als
//------------------------------------------------------------------

#define _USE_MATH_DEFINES
#include <cmath>

#include "portaudio.h"

//-----------------------------------------------------------------------------
// Audio.
//-----------------------------------------------------------------------------

namespace axAudio
{
    class Core
    {
    public:
        Core();
        
        ~Core();
        
        int InitAudio();
        void StartAudio();
        void StopAudio();
        
        virtual int CallbackAudio(const float *input,
                                  float *output,
                                  unsigned long frameCount)
        {
            float *out = output;
            
            for(int i = 0; i < frameCount; i++)
            {
                *out++ = 0.0;
                *out++ = 0.0;
            }
            
            return 0;
        }
        
        static int myPaCallback(const void *in,
                                void *out,
                                unsigned long frameCount,
                                const PaStreamCallbackTimeInfo* timeInfo,
                                PaStreamCallbackFlags statusFlags,
                                void* userData)
        {
            Core* audio = static_cast<Core*>(userData);
            return audio->CallbackAudio((const float*)in,
                                        (float*)out,
                                        frameCount);
        }
        
        // private:
        PaStreamParameters outputParameters;
        PaStream *stream;
        PaError err;
    };
}

#endif // __AX_AUDIO__