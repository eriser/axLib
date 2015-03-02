//
//  FilterParametric.h
//  AudioApp
//
//  Created by Alexandre Arsenault on 2015-02-27.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __AudioApp__FilterParametric__
#define __AudioApp__FilterParametric__

#include "axLib.h"
#include <array>
#include <axAudio/axAudioFilter.h>


class FilterParametric : public axPanel
{
public:
    FilterParametric(axWindow* parent, const axRect& rect);
    
    
    template<typename T>
    struct FilterCoefficients
    {
        FilterCoefficients(){}
        FilterCoefficients(const T& B0, const T& B1, const T& B2,
                           const T& A0, const T& A1, const T& A2):
        b0(B0), b1(B1), b2(B2), a0(A0), a1(A1), a2(A2)
        {
        }
        
        T b0, b1, b2, a0, a1, a2;
    };
    
    template<typename T>
    struct FilterInfo
    {
        FilterInfo(){}
        FilterInfo(const T& freq_, const T& q_, const T& amp_,
                   const axAudioFilterType& type_, const bool& active_):
                   freq(freq_), q(q_), amp(amp_), type(type_), active(active_)
        {
        }
        
        T freq, q, amp;
        axAudioFilterType type;
        bool active;
    };
    
    template<typename T>
    struct Filter
    {
        Filter(){}
        
        Filter(const FilterInfo<T>& info_, const FilterCoefficients<T>& coeff_):
        info(info_), coeff(coeff_)
        {
        }
        
        FilterInfo<T> info;
        FilterCoefficients<T> coeff;
    };
    
    void SetFilterCoefficients(const FilterCoefficients<double>& coeff);
    
    void SetFrequency(const int& index, const double& freq);
    void SetQ(const int& index, const double& q);
    void SetAmp(const int& index, const double& amp);
    void SetActive(const int& index, const bool& active);
    
private:
    axFont _font;
//    std::vector<FilterCoefficients<double>> _filterCoeffs;
    std::vector<Filter<double>> _filters;
    
    std::array<double, 300> _freqs;
    std::array<double, 45> _linesFreq;
    
    
    virtual void OnPaint();
};

#endif /* defined(__AudioApp__FilterParametric__) */
