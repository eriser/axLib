//
//  axWaveform.cpp
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-14.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//
#include <limits>
#include "axWaveform.h"
#include "axAudioBuffer.h"

axWaveform::axWaveform(axApp* app, axWindow* parent,
                         const axRect& rect):
axPanel(app, parent, rect),
_audioBuffer(nullptr)
{
    _zoom = 1.0;
    _leftPos = 0.0;
}

void axWaveform::SetAudioBuffer(axAudioBuffer* buffer)
{
    _audioBuffer = buffer;
}

void axWaveform::SetZoom(const double& zoom)
{
    axBufferInfo b_info = _audioBuffer->GetBufferInfo();
    double nSamplesToProcess = b_info.frames * zoom;
    
    if(nSamplesToProcess < 11)
    {
        _zoom = 11.0 / b_info.frames;
    }
    else
    {
        _zoom = zoom;
    }
    
    if(_leftPos * b_info.frames + nSamplesToProcess > b_info.frames)
    {
//        std::cout << "Error" << std::endl;
        _leftPos = double(b_info.frames - nSamplesToProcess) / double(b_info.frames);
    }
    
    
    Update();
}

void axWaveform::SetLeftPosition(const double& pos)
{
    axBufferInfo b_info = _audioBuffer->GetBufferInfo();
    double nSamplesToProcess = double(b_info.frames) * _zoom;
//    double left_pos = b_info.frames * _leftPos;
//    double right_pos = left_pos + nSamplesToProcess;
    
    if(pos < 0.0)
    {
        _leftPos = 0.0;
    }
    else if(b_info.frames * pos + nSamplesToProcess > b_info.frames)
    {
        // nothing.
        _leftPos = (b_info.frames - nSamplesToProcess) / b_info.frames;
    }
//    else if(pos > 1.0)
//    {
//        _leftPos = 1.0;
//    }
    else
    {
        _leftPos = pos;
    }
    
    Update();
}

axFloatRange axWaveform::GetBorders() const
{
    axBufferInfo b_info = _audioBuffer->GetBufferInfo();
    double nSamplesToProcess = double(b_info.frames) * _zoom;
    double left_pos = b_info.frames * _leftPos;
    double right_pos = left_pos + nSamplesToProcess;
    double r = 1.0 / double(b_info.frames);
    return axFloatRange(left_pos * r, right_pos * r);
}

//static int nDraw = 0;
void axWaveform::OnPaint()
{
//    nDraw++;
//    std::cout << "nDraw : " << nDraw << std::endl;

    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.5, 0.5, 0.5), 1.0);
    gc->DrawRectangle(rect0);
    
    if(_audioBuffer != nullptr)
    {
        axBufferInfo b_info = _audioBuffer->GetBufferInfo();
        float* buffer = _audioBuffer->GetBuffer();
        int middle_y = rect.size.y * 0.5;
        
        gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);

        double nSamplesToProcess = double(b_info.frames) * _zoom;
        
        // Not a 100% sure about 1 + ..
        int index = 1 + b_info.frames * _leftPos;
        
        int changing_pixel = 0; // Just for debugging.
        int nSamplePerPixel = 0;
        
        double min_value_pixel = 1000.0;
        double max_value_pixel = -1000.0;
        
        // Ratio of number of pixel over number of sample to draw.
        double r = double(rect.size.x-2) / nSamplesToProcess;
        
        for(int i = 1; i < ceil(nSamplesToProcess); i++, index++)
        {
            // Pixel position.
            double x_pos_left = double(i-1) * r;
            double x_pos_right = double(i) * r;
            
            double l_value = buffer[index - 1];
            double r_value = buffer[index];
            
            if(l_value < min_value_pixel) min_value_pixel = l_value;
            if(l_value > max_value_pixel) max_value_pixel = l_value;
            if(r_value < min_value_pixel) min_value_pixel = r_value;
            if(r_value > max_value_pixel) max_value_pixel = r_value;
            
            // Increment sample at each iteration.
            nSamplePerPixel++;
            
            // If pixel has change.
            if(int(x_pos_left) != int(x_pos_right))
            {
                if(nSamplePerPixel > 1)
                {
                    // Pixel value.
                    int y_pixel_left = middle_y - min_value_pixel * 0.9 * middle_y;
                    int y_pixel_right = middle_y - max_value_pixel * 0.9 * middle_y;
                    
                    // Draw min to max line on the left pixel.
                    gc->DrawLine(axPoint(x_pos_left, y_pixel_left),
                                 axPoint(x_pos_left, y_pixel_right));
                    
                    // Draw last value of left pixel and first value of right
                    // pixel. This is to make sure that horizontal lines will be
                    // drawn if necessary. A liason between each vertical lines.
                    y_pixel_left = middle_y - l_value * 0.9 * middle_y;
                    y_pixel_right = middle_y - r_value * 0.9 * middle_y;
                    
                    gc->DrawLine(axPoint(x_pos_left, y_pixel_left),
                                 axPoint(x_pos_right, y_pixel_right));
                }
                // One sample or less per pixel.
                else
                {
                    std::cout << "One sample or less per pixel." << std::endl;
                    // Pixel value.
                    int y_pixel_left = middle_y - l_value * 0.9 * middle_y;
                    int y_pixel_right = middle_y - r_value * 0.9 * middle_y;
                    
                    gc->DrawLine(axPoint(x_pos_left, y_pixel_left),
                                 axPoint(x_pos_right, y_pixel_right));
                }
                
//                std::cout << "Pixel : " << changing_pixel << std::endl;
//                std::cout << "Samples : " << nSamplePerPixel << std::endl;
                changing_pixel++;
                nSamplePerPixel = 0;
                min_value_pixel = 1000.0;
                max_value_pixel = -1000.0;
            }
            
//            // Pixel value.
//            int y_pixel_left = middle_y - l_value * 0.9 * middle_y;
//            int y_pixel_right = middle_y - r_value * 0.9 * middle_y;
//            
//            gc->DrawLine(axPoint(x_pos_left, y_pixel_left),
//                         axPoint(x_pos_right, y_pixel_right));
        }
        
        gc->SetColor(axColor(0.7, 0.7, 0.7), 1.0);
        gc->DrawLine(axPoint(1, middle_y), axPoint(rect.size.x - 2, middle_y));
    }

    
    
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);
}