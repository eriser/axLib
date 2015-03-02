//
//  axDrawingBuffer.h
//  axLib
//
//  contexteated by Alexandre Arsenault on 2015-02-20.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __axLib__axDrawingBuffer__
#define __axLib__axDrawingBuffer__

#include "axC++.h"
#include "cairo/cairo.h"

class axDrawingBuffer
{
public:
    axDrawingBuffer(const axSize& size);
    
    axSize GetSize() const
    {
        return _size;
    }
    
    unsigned char* GetData()
    {
        return surf_data;
    }
    
    GLuint GetTexture()
    {
        return _texture;
    }
    
    void DrawRectangle()
    {
        cairo_set_line_width (context, 40.96);
        cairo_move_to (context, 76.8, 84.48);
        cairo_rel_line_to (context, 51.2, 51.2);
        cairo_rel_line_to (context, 51.2, 200);
        cairo_set_line_join (context, CAIRO_LINE_JOIN_MITER); /* default */
        cairo_stroke (context);
        
//        cairo_move_to (context, 76.8, 161.28);
//        cairo_rel_line_to (context, 51.2, -51.2);
//        cairo_rel_line_to (context, 51.2, 51.2);
//        cairo_set_line_join (context, CAIRO_LINE_JOIN_BEVEL);
//        cairo_stroke (context);
//        
//        cairo_move_to (context, 76.8, 238.08);
//        cairo_rel_line_to (context, 51.2, -51.2);
//        cairo_rel_line_to (context, 51.2, 51.2);
//        cairo_set_line_join (context, CAIRO_LINE_JOIN_ROUND);
//        cairo_stroke (context);
        
//        cairo_restore (context);
    }
    
private:
    GLuint _texture;
    axSize _size;
    cairo_surface_t *surface;
    cairo_t *context;
    unsigned char* surf_data;
    
    cairo_t*  contexteate_cairo_context(int width,
                                   int height,
                                   int channels,
                                   cairo_surface_t** surf,
                                   unsigned char** buffer);
    
};

#endif /* defined(__axLib__axDrawingBuffer__) */
