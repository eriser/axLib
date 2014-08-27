#define T_FLOAT float
//-----------------------------------------------------------------------------
//  Audio_Filter
//-----------------------------------------------------------------------------
class axAudioFilter
{
public:
    axAudioFilter();
    T_FLOAT Process(T_FLOAT in);
    t_out ProcessStereo(t_out in);
    void SetFreq(T_FLOAT f);
    void SetQ(T_FLOAT f);
    void SetGain(T_FLOAT f);

    T_FLOAT* lfo[2];
    T_FLOAT* lfoAmnt[2];
    T_FLOAT* env[2];
    T_FLOAT* envAmnt[2];

//private:
    T_FLOAT freq, q, gain;
    void Compute_Variables(T_FLOAT freq, T_FLOAT q);
    void Biquad_Coeff(int fType);
    T_FLOAT b0, b1, b2, a0, a1, a2;
    T_FLOAT x1, x2, y1, y2;
    T_FLOAT rx1, rx2, ry1, ry2;
    T_FLOAT c, w0, alpha;
    int filterType;
    bool init;
    long sr;
};
