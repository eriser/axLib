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
    
    void InitGLWindowBackBuffer(GLuint& frameBuffer,
                                GLuint& frameBufferTexture,
                                const axSize& size);
    
    void DrawWindow(axWindow* win, bool& need_update, GLuint& frameBuffer);
}

#endif /* defined(__axLib__axGraphicInterface__) */
