//
//  axWaveform.cpp
//  MidiSequencer
//
//  Created by Alexandre Arsenault on 2014-11-14.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//
#include <limits>
#include "axWaveform.h"
#include <axAudio/axAudioBuffer.h>

/*******************************************************************************
 * axWaveform::Info.
 ******************************************************************************/
axWaveform::Info::Info() :
axInfo()
{
    
}

axWaveform::Info::Info(const axColor& bgColor,
                       const axColor& waveformColor,
                       const axColor& contourColor,
                       const axColor& gridMainLineColor,
                       const axColor& gridSecondLineColor,
                       const axColor& middleLineColor,
                       const axColor& envColor) :
// Heritage.
axInfo(),
// Members.
bg_color(bgColor),
waveform_color(waveformColor),
contour_color(contourColor),
grid_main_line_color(gridMainLineColor),
grid_second_line_color(gridSecondLineColor),
middle_line_color(middleLineColor),
env_color(envColor)
{
    
}

axWaveform::Info::Info(const std::string& path):
// Heritage.
axInfo(path)
{
    axWidgetLoader loader;
    axVectorPairString att = loader.GetAttributes(path);
    
    SetAttributes(att);
}

axWaveform::Info::Info(const axVectorPairString& attributes)
{
    SetAttributes(attributes);
}

axStringVector axWaveform::Info::GetParamNameList() const
{
    return axStringVector{"bg_color",
                          "waveform_color",
                          "contour_color",
                          "grid_main_line_color",
                          "grid_second_line_color",
                          "middle_line_color",
                          "env_color"};
}

std::string axWaveform::Info::GetAttributeValue(const std::string& name)
{
    if(name == "bg_color")
    {
        return bg_color.ToString();
    }
    else if(name == "waveform_color")
    {
        return waveform_color.ToString();
    }
    else if(name == "contour_color")
    {
        return contour_color.ToString();
    }
    else if(name == "grid_main_line_color")
    {
        return grid_main_line_color.ToString();
    }
    else if(name == "grid_second_line_color")
    {
        return grid_second_line_color.ToString();
    }
    else if(name == "middle_line_color")
    {
        return middle_line_color.ToString();
    }
    else if(name == "env_color")
    {
        return env_color.ToString();
    }
    
    return "";
}

void axWaveform::Info::SetAttribute(const axStringPair& attribute)
{
    if(attribute.first == "bg_color")
    {
        bg_color.LoadFromString(attribute.second);
    }
    else if(attribute.first == "waveform_color")
    {
        waveform_color.LoadFromString(attribute.second);
    }
    else if(attribute.first == "contour_color")
    {
        contour_color.LoadFromString(attribute.second);
    }
    else if(attribute.first == "grid_main_line_color")
    {
        grid_main_line_color.LoadFromString(attribute.second);
    }
    else if(attribute.first == "grid_second_line_color")
    {
        grid_second_line_color.LoadFromString(attribute.second);
    }
    else if(attribute.first == "middle_line_color")
    {
        middle_line_color.LoadFromString(attribute.second);
    }
    else if(attribute.first == "env_color")
    {
        env_color.LoadFromString(attribute.second);
    }
}

/*******************************************************************************
 * axButton::Builder.
 ******************************************************************************/
axWaveform::Builder::Builder(axWindow* parent):
axWidgetBuilder(parent)
{
    
}

axWidget* axWaveform::Builder::Create(const axVectorPairString& attributes)
{
    axWindow* parent = GetParent();
    std::string name;
    axPoint pos;
    
    for(auto& s : attributes)
    {
        if(s.first == "name")
        {
            name = s.second;
        }
        else if(s.first == "rect")
        {
            axStringVector strVec;
            strVec = GetVectorFromStringDelimiter(s.second, ",");
            
            pos = axPoint(stoi(strVec[0]),
                          stoi(strVec[1]));
            
            _size = axSize(stoi(strVec[2]),
                           stoi(strVec[3]));
        }
        else if(s.first == "info")
        {
            _info = axWaveform::Info(s.second);
        }
    }
    
    axWaveform* wave = new axWaveform(parent, axRect(pos, _size), _info);
    
    parent->GetResourceManager()->Add(name, wave);
    return wave;
}

/*******************************************************************************
 * axWaveform::axWaveform.
 ******************************************************************************/
axWaveform::axWaveform(axWindow* parent,
                       const axRect& rect,
                       const axWaveform::Info& info) :
axWidget(parent, rect, new Info(info)),
_audioBuffer(nullptr)
{
    _zoom = 1.0;
    _leftPos = 0.0;
    _showEnv = false;
}

void axWaveform::SetAudioBuffer(axAudioBuffer* buffer)
{
    _audioBuffer = buffer;
    _zoom = 1.0;
    _leftPos = 0.0;
    Update();
}

void axWaveform::ShowEnv(const bool& show)
{
    _showEnv = show;
    Update();
}

void axWaveform::SetZoom(const double& zoom)
{
    if(_audioBuffer != nullptr)
    {
        axBufferInfo b_info = _audioBuffer->GetBufferInfo();
        double nSamplesToProcess = b_info.frames * zoom;
        
        if(nSamplesToProcess < 5.0)
        {
            _zoom = 5.0 / double(b_info.frames);
        }
        else
        {
            _zoom = zoom;
        }
        
        if(_leftPos * b_info.frames + nSamplesToProcess > b_info.frames)
        {
            _leftPos = double(b_info.frames - nSamplesToProcess) / double(b_info.frames);
        }
        
        axPanel::Update();
    }
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
    
    axPanel::Update();
}

axFloatRange axWaveform::GetBorders() const
{
    if(_audioBuffer != nullptr)
    {
        axBufferInfo b_info = _audioBuffer->GetBufferInfo();
        double nSamplesToProcess = double(b_info.frames) * _zoom;
        double left_pos = b_info.frames * _leftPos;
        double right_pos = left_pos + nSamplesToProcess;
        double r = 1.0 / double(b_info.frames);
        return axFloatRange(left_pos * r, right_pos * r);
    }
    return axFloatRange(0.0, 1.0);
}

//static int nDraw = 0;
void axWaveform::OnPaint()
{
//    nDraw++;
//    std::cout << "nDraw : " << nDraw << std::endl;

    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(GetDrawingRect());
    axWaveform::Info* info = static_cast<Info*>(_info);
    
    // Draw background.
    gc->SetColor(info->bg_color);
    gc->DrawRectangle(rect0);
    
    if(_audioBuffer != nullptr)
    {
        axBufferInfo b_info = _audioBuffer->GetBufferInfo();
        float* buffer = _audioBuffer->GetBuffer();
        int middle_y = rect.size.y * 0.5;
        
        // Waveform color.
        gc->SetColor(info->waveform_color);

        double nSamplesToProcess = double(b_info.frames) * _zoom;
        
        // Not a 100% sure about 1 + ..
        int index = 1 + b_info.frames * _leftPos;
        
        int changing_pixel = 0; // Just for debugging.
        int nSamplePerPixel = 0;
        
        double min_value_pixel = 1000.0;
        double max_value_pixel = -1000.0;
        
        // Ratio of number of pixel over number of sample to draw.
        double r = double(rect.size.x-2) / nSamplesToProcess;
        
        for(int i = 1; i < floor(nSamplesToProcess); i++, index++)
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
                    // Pixel value.
                    int y_pixel_left = middle_y - l_value * 0.9 * middle_y;
                    int y_pixel_right = middle_y - r_value * 0.9 * middle_y;
                    
                    gc->DrawLine(axPoint(x_pos_left, y_pixel_left),
                                 axPoint(x_pos_right, y_pixel_right));
                }
                
                changing_pixel++;
                nSamplePerPixel = 0;
                min_value_pixel = 1000.0;
                max_value_pixel = -1000.0;
            }
        }
        
        // Draw middle line.
        gc->SetColor(info->middle_line_color);
        gc->DrawLine(axPoint(1, middle_y), axPoint(rect.size.x - 2, middle_y));
    }
    
    //--------------------------------------------------------------------------
//    if(_envBuffer != nullptr)
//    {
//        if((*_envBuffer).size() && _showEnv)
//        {
//            //        axBufferInfo b_info = _audioBuffer->GetBufferInfo();
//            //        float* buffer = _audioBuffer->GetBuffer();
//            int middle_y = rect.size.y * 0.5;
//            
//            gc->SetColor(info->env_color);
//            
//            double nSamplesToProcess = double((*_envBuffer).size()) * _zoom;
//            
//            // Not a 100% sure about 1 + ..
//            int index = 1 + (*_envBuffer).size() * _leftPos;
//            
//            int changing_pixel = 0; // Just for debugging.
//            int nSamplePerPixel = 0;
//            
//            double min_value_pixel = 1000.0;
//            double max_value_pixel = -1000.0;
//            
//            // Ratio of number of pixel over number of sample to draw.
//            double r = double(rect.size.x-2) / nSamplesToProcess;
//            
//            for(int i = 1; i < floor(nSamplesToProcess); i++, index++)
//            {
//                // Pixel position.
//                double x_pos_left = double(i-1) * r;
//                double x_pos_right = double(i) * r;
//                
//                double l_value = (*_envBuffer)[index - 1];
//                double r_value = (*_envBuffer)[index];
//                
//                if(l_value < min_value_pixel) min_value_pixel = l_value;
//                if(l_value > max_value_pixel) max_value_pixel = l_value;
//                if(r_value < min_value_pixel) min_value_pixel = r_value;
//                if(r_value > max_value_pixel) max_value_pixel = r_value;
//                
//                // Increment sample at each iteration.
//                nSamplePerPixel++;
//                
//                // If pixel has change.
//                if(int(x_pos_left) != int(x_pos_right))
//                {
//                    if(nSamplePerPixel > 1)
//                    {
//                        // Pixel value.
//                        int y_pixel_left = middle_y - min_value_pixel * 0.9 * middle_y;
//                        int y_pixel_right = middle_y - max_value_pixel * 0.9 * middle_y;
//                        
//                        // Draw min to max line on the left pixel.
//                        gc->DrawLine(axPoint(x_pos_left, y_pixel_left),
//                                     axPoint(x_pos_left, y_pixel_right));
//                        
//                        // Draw last value of left pixel and first value of right
//                        // pixel. This is to make sure that horizontal lines will be
//                        // drawn if necessary. A liason between each vertical lines.
//                        y_pixel_left = middle_y - l_value * 0.9 * middle_y;
//                        y_pixel_right = middle_y - r_value * 0.9 * middle_y;
//                        
//                        gc->DrawLine(axPoint(x_pos_left, y_pixel_left),
//                                     axPoint(x_pos_right, y_pixel_right));
//                    }
//                    // One sample or less per pixel.
//                    else
//                    {
//                        // Pixel value.
//                        int y_pixel_left = middle_y - l_value * 0.9 * middle_y;
//                        int y_pixel_right = middle_y - r_value * 0.9 * middle_y;
//                        
//                        gc->DrawLine(axPoint(x_pos_left, y_pixel_left),
//                                     axPoint(x_pos_right, y_pixel_right));
//                    }
//                    
//                    changing_pixel++;
//                    nSamplePerPixel = 0;
//                    min_value_pixel = 1000.0;
//                    max_value_pixel = -1000.0;
//                }
//            }
//            
//        }
//    }
    
//    if(_envPoints != nullptr)
//    {
//        if((*_envPoints).size() && _showEnv)
//        {
//            int middle_y = rect.size.y * 0.5;
//            
////            gc->SetColor(axColor(0.0, 1.0, 0.0), 1.0);
////            
////            for(auto& n : *_envPoints)
////            {
////                double x_pos = n.x * GetRect().size.x;
////                gc->DrawLine(axPoint(x_pos, middle_y),
////                             axPoint(x_pos, middle_y - n.y * 0.9 * middle_y));
////                
//////                std::cout << n.x << " " << n.y << std::endl;
////            }
//            
//            gc->SetColor(axColor(0.0, 0.0, 1.0), 1.0);
//            
//            for(int i = 1; i < (*_envPoints).size(); i++)
//            {
//                double x_pos_left = (*_envPoints)[i-1].x * GetRect().size.x;
//                double x_pos_right = (*_envPoints)[i].x * GetRect().size.x;
//                int y_pixel_left = middle_y - (*_envPoints)[i-1].y * 0.9 * middle_y;
//                int y_pixel_right = middle_y - (*_envPoints)[i].y * 0.9 * middle_y;
//                
//                gc->DrawLine(axPoint(x_pos_left, y_pixel_left),
//                             axPoint(x_pos_right, y_pixel_right));
////                gc->DrawLine(axPoint(x_pos, middle_y),
////                             axPoint(x_pos, middle_y - n.y * 0.9 * middle_y));
//                
//                //                std::cout << n.x << " " << n.y << std::endl;
//            }
//        }
//    }
    

    // Draw contour.
    gc->SetColor(info->contour_color);
    gc->DrawRectangleContour(rect0);
}