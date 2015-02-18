//
//  axGraphicInterface.h
//  axLib
//
//  Created by Alexandre Arsenault on 2015-02-17.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __axLib__axGraphicInterface__
#define __axLib__axGraphicInterface__

#include "axC++.h"

class axWindow;

namespace axGraphicInterface
{
    void Resize(const axSize& size);
    
    void Init();
    
    void Draw(const axSize& size);
}


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
};

#endif /* defined(__axLib__axGraphicInterface__) */
