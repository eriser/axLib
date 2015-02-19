//
//  axGraphicInterface.h
//  axLib
//
//  Created by Alexandre Arsenault on 2015-02-17.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __AX_FRAME_BUFFER__
#define __AX_FRAME_BUFFER__

#include "axC++.h"

class axFrameBuffer
{
public:
    axFrameBuffer(const axSize& size);
    
    void Init(const axSize& size);
    
    void Resize(const axSize& size);
    
    void DrawOnFrameBuffer(const std::function<void()>& fct,
                           const axSize& size);
    
    void DrawFrameBuffer(const axSize& shownSize);
    
//    void DrawFrameBufferOnParentFrameBuffer(const axSize& shownSize);
    
private:
    GLuint _frameBuffer, _frameBufferTexture, _depthBuffer;
    
    void DrawingOnFrameBufferBlendFunction();
    void DrawingFrameBufferBlendFunction();
};

#endif // __AX_FRAME_BUFFER__
