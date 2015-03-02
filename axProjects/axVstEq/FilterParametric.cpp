//
//  FilterParametric.cpp
//  AudioApp
//
//  Created by Alexandre Arsenault on 2015-02-27.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "FilterParametric.h"
#include <axAudio/axAudioUtils.h>
#include <axAudio/axAudioFilter.h>


void NormalizeCoeff(FilterParametric::FilterCoefficients<double>& coeff)
{
    coeff.b0 /= coeff.a0;
    coeff.b1 /= coeff.a0;
    coeff.b2 /= coeff.a0;
    coeff.a1 /= coeff.a0;
    coeff.a2 /= coeff.a0;
    coeff.a0 = 1;
}

FilterParametric::FilterCoefficients<double> ComputeVariables
(const FilterParametric::FilterInfo<double>& info)
{
    FilterParametric::FilterCoefficients<double> coeff;
    
    double sr = 44100.0;

    double w0 = axAudioConstant::TwoPi * info.freq / sr;
    double c = cosf(w0);
    double alpha = sin(w0) / (2.0 * info.q);
    double A = info.amp;
    
    switch (info.type)
    {
        case axAUDIO_FILTER_LPF:
            coeff.b0 = coeff.b2 = (1 - c) / 2;
            coeff.b1 = 1 - c;
            coeff.a0 = 1 + alpha;
            coeff.a1 = -2 * c;
            coeff.a2 = 1 - alpha;
            break;
            
        case axAUDIO_FILTER_HPF:
            coeff.b0 = (1 + c) / 2;
            coeff.b1 = -(1 + c);
            coeff.b2 = coeff.b0;
            coeff.a0 = 1 + alpha;
            coeff.a1 = -2 * c;
            coeff.a2 = 1 - alpha;
            break;
            
        case axAUDIO_FILTER_BPF:
            coeff.b0 = alpha;
            coeff.b1 = 0;
            coeff.b2 = -alpha;
            coeff.a0 = 1 + alpha;
            coeff.a1 = -2 * c;
            coeff.a2 = 1 - alpha;
            break;
            
        case axAUDIO_FILTER_NOTCH:
            coeff.b0 = 1.0;
            coeff.b1 = -2.0 * cos(w0);
            coeff.b2 = 1.0;
            coeff.a0 = 1.0 + alpha;
            coeff.a1 = -2.0 * cos(w0);
            coeff.a2 = 1.0 - alpha;
            break;
            
        case axAUDIO_FILTER_PEAK:
            coeff.b0 = 1.0 + alpha * A;
            coeff.b1 = -2.0 * cos(w0);
            coeff.b2 = 1.0 - alpha * A;
            coeff.a0 = 1.0 + alpha / A;
            coeff.a1 = -2.0 * cos(w0);
            coeff.a2 = 1.0 - alpha / A;
            break;
            
        case axAUDIO_FILTER_LOW_SHELF:
            coeff.b0 = A * ((A + 1.0) - (A - 1.0) * cos(w0) + 2.0 * sqrt(A) * alpha);
            coeff.b1 = 2.0 * A *((A - 1.0) - (A + 1.0) * cos(w0));
            coeff.b2 = A * ((A + 1.0) - (A - 1.0) * cos(w0) - 2.0 * sqrt(A) * alpha);
            coeff.a0 = (A + 1.0) + (A - 1.0) * cos(w0) + 2.0 * sqrt(A) * alpha;
            coeff.a1 = -2.0 * ((A - 1.0) + (A + 1.0) * cos(w0));
            coeff.a2 = (A + 1.0) + (A - 1.0) * cos(w0) - 2.0 * sqrt(A) * alpha;
            break;
            
        case axAUDIO_FILTER_HIGH_SHELF:
            coeff.b0 = A * ((A + 1.0) + (A - 1.0) * cos(w0) + 2.0 * sqrt(A) * alpha);
            coeff.b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * cos(w0));
            coeff.b2 = A * ((A + 1.0) + (A - 1.0) * cos(w0) - 2.0 * sqrt(A) * alpha);
            coeff.a0 = (A + 1.0) - (A - 1.0) * cos(w0) + 2.0 * sqrt(A) * alpha;
            coeff.a1 = 2.0 * ((A - 1.0) - (A + 1.0) * cos(w0));
            coeff.a2 = (A + 1.0) - (A - 1.0) * cos(w0) - 2.0 * sqrt(A) * alpha;
            break;
    }
    
    NormalizeCoeff(coeff);
    
    return coeff;
}

FilterParametric::FilterParametric(axWindow* parent, const axRect& rect):
axPanel(parent, rect),
_font(0)
{
    _font.SetFontSize(9);
    
    // 1.
    auto info_filter = FilterInfo<double>(100.0, 0.707, 1.0, axAUDIO_FILTER_HPF, true);
    auto coeff_filter = ComputeVariables(info_filter);
    _filters.push_back(Filter<double>(info_filter, coeff_filter));
    
    // 2.
    info_filter = FilterInfo<double>(500.0, 0.707, 1.0, axAUDIO_FILTER_PEAK, true);
    coeff_filter = ComputeVariables(info_filter);
    _filters.push_back(Filter<double>(info_filter, coeff_filter));
    
    // 3.
    info_filter = FilterInfo<double>(800.0, 0.707, 1.0, axAUDIO_FILTER_PEAK, true);
    coeff_filter = ComputeVariables(info_filter);
    _filters.push_back(Filter<double>(info_filter, coeff_filter));
    
    // 4.
    info_filter = FilterInfo<double>(3000.0, 0.707, 1.0, axAUDIO_FILTER_PEAK, true);
    coeff_filter = ComputeVariables(info_filter);
    _filters.push_back(Filter<double>(info_filter, coeff_filter));
    
    // 5.
    info_filter = FilterInfo<double>(10000.0, 0.707, 1.0, axAUDIO_FILTER_PEAK, true);
    coeff_filter = ComputeVariables(info_filter);
    _filters.push_back(Filter<double>(info_filter, coeff_filter));
    
    // 6.
    info_filter = FilterInfo<double>(12000.0, 0.707, 1.0, axAUDIO_FILTER_PEAK, true);
    coeff_filter = ComputeVariables(info_filter);
    _filters.push_back(Filter<double>(info_filter, coeff_filter));
    
    // 7.
    info_filter = FilterInfo<double>(15000.0, 0.707, 1.0, axAUDIO_FILTER_PEAK, true);
    coeff_filter = ComputeVariables(info_filter);
    _filters.push_back(Filter<double>(info_filter, coeff_filter));
    
    // 8.
    info_filter = FilterInfo<double>(20000.0, 0.707, 1.0, axAUDIO_FILTER_LPF, true);
    coeff_filter = ComputeVariables(info_filter);
    _filters.push_back(Filter<double>(info_filter, coeff_filter));

    
    for(int i = 0; i < 300; i++)
    {
        _freqs[i] = axAudio::LogarithmicInterpole<double>
        (15.0, 22000.0, i / double(300.0));
    }

    _linesFreq[0] = 20.0;
    _linesFreq[1] = 30.0;
    _linesFreq[2] = 40.0;
    _linesFreq[3] = 50.0;
    _linesFreq[4] = 60.0;
    _linesFreq[5] = 70.0;
    _linesFreq[6] = 80.0;
    _linesFreq[7] = 90.0;
    _linesFreq[8] = 100.0;
    
    _linesFreq[9] = 200.0;
    _linesFreq[10] = 300.0;
    _linesFreq[11] = 400.0;
    _linesFreq[12] = 500.0;
    _linesFreq[13] = 600.0;
    _linesFreq[14] = 700.0;
    _linesFreq[15] = 800.0;
    _linesFreq[16] = 900.0;
    _linesFreq[17] = 1000.0;

    _linesFreq[18] = 2000.0;
    _linesFreq[19] = 3000.0;
    _linesFreq[20] = 4000.0;
    _linesFreq[21] = 5000.0;
    _linesFreq[22] = 6000.0;
    _linesFreq[23] = 7000.0;
    _linesFreq[24] = 8000.0;
    _linesFreq[25] = 9000.0;
    _linesFreq[26] = 10000.0;

    _linesFreq[27] = 11000.0;
    _linesFreq[28] = 12000.0;
    _linesFreq[29] = 13000.0;
    _linesFreq[30] = 14000.0;
    _linesFreq[31] = 15000.0;
    _linesFreq[32] = 16000.0;
    _linesFreq[33] = 17000.0;
    _linesFreq[34] = 18000.0;
    _linesFreq[35] = 19000.0;
    _linesFreq[36] = 20000.0;
}

void FilterParametric::SetFrequency(const int& index, const double& freq)
{
    _filters[index].info.freq = freq;
    _filters[index].coeff = ComputeVariables(_filters[index].info);
    Update();
}

void FilterParametric::SetQ(const int& index, const double& q)
{
    _filters[index].info.q = q;
    _filters[index].coeff = ComputeVariables(_filters[index].info);
    Update();
}

void FilterParametric::SetAmp(const int& index, const double& amp)
{
    _filters[index].info.amp = amp;
    _filters[index].coeff = ComputeVariables(_filters[index].info);
    Update();
}

void FilterParametric::SetActive(const int& index, const bool& active)
{
    _filters[index].info.active = active;
    _filters[index].coeff = ComputeVariables(_filters[index].info);
    Update();
}


template <size_t size>
void FillPoints(const std::array<double, size>& freqs,
                const FilterParametric::Filter<double>& filter,
                std::array<double, size>& points,
                const int& sr)
{
    using axAudio::Pow2;
    
    FilterParametric::FilterCoefficients<double> c = filter.coeff;
    
    // Formula  : Magnitude Response of biquad filter.
    // Num      : b0^2 + b1^2 + b2^2 + 2(b0 b1 + b1 b2) cos(w) + 2 b0 b2 cos(2w)
    // Denum    : 1 + a1^2 + a2^2 + 2(a1 + a1a2)cos(w) + 2 a2 cos(w)
    // Magnitude: sqrt(num / denum)
    
    if(filter.info.active)
    {
        const double num_sum = Pow2(c.b0) + Pow2(c.b1) + Pow2(c.b2);
        const double denum_sum = 1.0 + Pow2(c.a1) + Pow2(c.a2);
        
        for (int i = 0; i < freqs.size(); ++ i)
        {
            const double w = 2.0 * axAudioConstant::Pi * freqs[i] / double(sr);
            const double cosWT2 = 2.0 * cos(w);
            const double cos2WT2 = 2.0 * cos(2.0 * w);
            
            double numerator = num_sum + (c.b0 * c.b1 + c.b1 * c.b2) * cosWT2 +
            c.b0 * c.b2 * cos2WT2;
            
            double denominator = denum_sum + (c.a1 + c.a1 * c.a2) * cosWT2 +
            c.a2 * cos2WT2;
            // Magnitude.
            points[i] *= sqrt(numerator / denominator);
        }
    }
}

void FilterParametric::OnPaint()
{
    axGC gc(this);
    axRect rect(GetDrawingRect());
    
    gc.SetColor(axColor(0.3));
    gc.DrawRoundedRectangle(rect, 10);
    
    const int sr = 44100;
    const double log15 = log(15.0);
    const double mult_const = rect.size.x / double(log(22000.0) - log15);
    
    using axAudio::Pow2;
    
    std::array<double, 300> points;
    points.fill(1.0);
    
    // Fill frequency response magnitudes.
    for(auto& n : _filters)
    {
        FillPoints<300>(_freqs, n, points, sr);
    }

    // Draw freq lines.
    gc.SetColor(axColor(0.2));
    for(int i = 0; i < _linesFreq.size(); i++)
    {
        const double x_pos = (log(_linesFreq[i]) - log15) * mult_const;
        gc.DrawLine(axPoint(x_pos, 1.0), axPoint(x_pos, rect.size.y));
    }
    
    // Draw horizontale lines.
    gc.SetColor(axColor(0.2));
    for(int i = 0; i < 15; i++)
    {
        const double y_pos = i / double(15) * rect.size.y;
        gc.DrawLine(axPoint(1.0, y_pos), axPoint(rect.size.x, y_pos));
    }
    
    // Draw frequency response.
    gc.SetSmoothLine(2.0);
    gc.SetColor(axColor(0.8, 0.4, 0.0));
    
    for(int i = 1; i < _freqs.size(); i++)
    {
        const double x_left = (log(_freqs[i-1]) - log15) * mult_const;
        const double x_right = (log(_freqs[i]) - log15) * mult_const;

        const axPoint lpt(x_left, rect.size.y * (1.0 - points[i - 1] * 0.5));
        const axPoint rpt(x_right, rect.size.y * (1.0 - points[i] * 0.5));
        
        gc.DrawLine(lpt, rpt, 2.0);
    }
    
    gc.SetDefaultLine();
    
    

    // Draw filter freq points.
    std::array<double, 8> f;
    for(int i = 0; i < f.size(); i++)
    {
        f[i] = _filters[i].info.freq;
    }
    
    std::array<double, 8> pt;
    pt.fill(1.0);
    
    for(int i = 0; i < 8; i++)
    {
        FillPoints<8>(f, _filters[i], pt, sr);
    }
    
    for(int i = 0; i < 8; i++)
    {
        if(_filters[i].info.active)
        {
            gc.SetColor(axColor(0.9, 0.4, 0.0));
            const double x = (log(f[i]) - log15) * mult_const;
            
            axPoint pt_pos(x, rect.size.y * (1.0 - pt[i] * 0.5));
            gc.DrawPoint(pt_pos, 11);
            
            gc.SetColor(axColor(1.0));
            
            gc.DrawString(_font, to_string(i + 1),
                          pt_pos - axPoint(3, 6));
        }
    }
    
    gc.SetColor(axColor::axBlackColor);
    gc.DrawRoundedRectangleContour(rect, 10);
}