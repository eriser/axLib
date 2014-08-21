#ifndef __AUDIO_WAVEFORM__
#define __AUDIO_WAVEFORM__

#include "axLib.h"
#include <vector>
#include <string>
#include <cmath>

#include "axAudio.h"

//#include <sys/types.h>
//#include <unistd.h>

struct axWaveformEvents
{
	axWaveformEvents()
	{

	}

	int f;
};

struct axWaveformInfo
{
    axColor waveform, background, lines, contour;

    axWaveformInfo( const axColor& wave_color,
                    const axColor& bg_color,
                    const axColor& lines_color,
                    const axColor& contour_color):
                    // Members
                    waveform( wave_color ),
                    background( bg_color ),
                    lines( lines_color ),
                    contour( contour_color ){}
};


class axWaveform: public axPanel
{
public:
    axWaveform( axApp* app,
                axWindow* parent,
                const axRect& rect,
                const axWaveformEvents& events,
                const axWaveformInfo& waveInfo,
				const string& snd_path);

    void SetBufferPtr( axAudioBuffer* buffer )
    {
        m_buffer = buffer;
        Update();
    }

	void SetBackgroundAlpha(const float& alpha)
	{
		_bgAlpha = alpha;
	}

    // void SetTimeLineParms( axTimeLineParams* lines )
    // {
    //     m_lineParams = lines;
    //     Update();
    // }

private:
    axWaveformInfo m_info;
    axWaveformEvents m_eventID;
    axAudioBuffer* m_buffer;

	axImage* _envRoundImg;

	vector<axFloatPoint> _pointsRealValue;
	vector<axPoint> _envPoints;
    // axTimeLineParams* m_lineParams;

	float _bgAlpha;

    void FormatLines(const axSize& size);

    void OnPaint();
};

//-----------------------------------------------------------------------------
// MultipleSlider.
//-----------------------------------------------------------------------------
// class AudioWaveform: public axPanel
// {
// public:
// 	AudioWaveform(axApp* app, 
// 				   axWindow* parent, 
// 				   const axRect& rect);

// private:
// 	// Events.
// 	virtual void OnPaint();
// };


#endif // __AUDIO_WAVEFORM__