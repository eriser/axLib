//
//  axDrawingBuffer.cpp
//  axLib
//
//  Created by Alexandre Arsenault on 2015-02-20.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#include "axDrawingBuffer.h"

axDrawingBuffer::axDrawingBuffer(const axSize& size):
_size(size)
{
    // Create cairo-surface/context to act as OpenGL-texture source
    context = contexteate_cairo_context(size.x, size.y, 4,
                                        &surface, &surf_data);
    
    glGenTextures (1, &_texture);
    glBindTexture (GL_TEXTURE_RECTANGLE_ARB, _texture);
    glTexImage2D (GL_TEXTURE_RECTANGLE_ARB,
                  0,
                  GL_RGBA,
                  size.x,
                  size.y,
                  0,
                  GL_BGRA,
                  GL_UNSIGNED_BYTE,
                  NULL);
    glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
}

cairo_t* axDrawingBuffer::contexteate_cairo_context(int width,
                                                    int height,
                                                    int channels,
                                                    cairo_surface_t** surf,
                                                    unsigned char** buffer)

{
    cairo_t* cr;
    
    // create cairo-surface/context to act as OpenGL-texture source
    *buffer = (unsigned char*)calloc(channels * width * height, sizeof (unsigned char));
    if (!*buffer)
    {
        printf ("create_cairo_context() - Couldn't allocate buffer\n");
        return NULL;
    }
    
    *surf = cairo_image_surface_create_for_data(*buffer,
                                                CAIRO_FORMAT_ARGB32,
                                                width,
                                                height,
                                                channels * width);
    if (cairo_surface_status (*surf) != CAIRO_STATUS_SUCCESS)
    {
        free (*buffer);
        printf ("create_cairo_context() - Couldn't create surface\n");
        return NULL;
    }
    
    cr = cairo_create (*surf);
    if (cairo_status (cr) != CAIRO_STATUS_SUCCESS)
    {
        free (*buffer);
        printf ("create_cairo_context() - Couldn't create context\n");
        return NULL;
    }
    
    return cr;
}