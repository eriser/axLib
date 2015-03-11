/*******************************************************************************
 * Copyright (c) 2015 Alexandre Arsenault.
 *
 * This file is part of axLibrary.
 *
 * axLibrary is free or commercial software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 or any later version of the
 * License or use a commercial axLibrary License.
 *
 * axLibrary is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with axLibrary. If not, see <http://www.gnu.org/licenses/>.
 *
 * To release a closed-source product which uses axLibrary, commercial
 * licenses are available, email alx.arsenault@gmail.com for more information.
 ******************************************************************************/

#ifndef __AX_FRAME_BUFFER__
#define __AX_FRAME_BUFFER__

/*******************************************************************************
 * @file    axFrameBuffer.h
 * @author  Alexandre Arsenault <alx.arsenault@gmail.com>
 * @date    18/02/2015
 ******************************************************************************/

/// @defgroup Graphic
/// @{

#include "axC++.h"
//#include "cairo/cairo.h"

class axFrameBuffer
{
public:
    axFrameBuffer(const axSize& size);
    
    void Init(const axSize& size);
    
    void Resize(const axSize& size);
    
    void DrawOnFrameBuffer(const std::function<void()>& fct,
                           const axSize& size);
    
    void DrawFrameBuffer(const axSize& shownSize);
    
private:
    GLuint _frameBuffer, _frameBufferTexture, _depthBuffer;
    
//    cairo_surface_t *surface;
//    cairo_t *context;
//    unsigned char* surf_data;
    
    void DrawingOnFrameBufferBlendFunction();
    void DrawingFrameBufferBlendFunction();
//    
//    cairo_t*  create_cairo_context(int width,
//                                   int height,
//                                   int channels,
//                                   cairo_surface_t** surf,
//                                   unsigned char** buffer);
   
};

/// @}
#endif // __AX_FRAME_BUFFER__
