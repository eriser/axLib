/*******************************************************************************
 * Copyright (c) 2013 Alexandre Arsenault.
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
#include "axFont.h"

axFont::axFont():
_isReady(false)
{
    
}

axFont::axFont(const int& data_index):
_isReady(false)
{
    if (FT_Init_FreeType(&_freeType))
    {
        axError("Could not init freetype library.");
        FT_Done_FreeType(_freeType);
    }
    else
    {
        
        if(data_index > 1)
        {
            axError("Only two default font buffer.");

            FT_Done_FreeType(_freeType);
        }
        else
        {
            bool err = FT_New_Memory_Face(_freeType,
                                          GetDefaultFontData(data_index),
                                          GetDefaultFontDataSize(data_index),
                                          0,
                                          &_face);
            
            if(err)
            {
                axError("Could not open font.");
                FT_Done_FreeType(_freeType);
            }
            else
            {
                _isReady = true;
                SetFontSize(12);
                glGenTextures(1, &_texture);
            }
        }
    }
}

axFont::axFont(const std::string& path):
_isReady(false)
{
    if (FT_Init_FreeType(&_freeType))
    {
        axError("Could not init freetype library.");
        FT_Done_FreeType(_freeType);
    }
    else
    {
        bool err = LoadFont(path, _face);
        
        if(err)
        {
            axError("Could not open font.");
            FT_Done_FreeType(_freeType);
        }
        else
        {
            _isReady = true;
            SetFontSize(12);
            glGenTextures(1, &_texture);
        }
    }
}

axFont::~axFont()
{
    if(_isReady)
    {
        FT_Done_Face(_face);
        FT_Done_FreeType(_freeType);
        glDeleteTextures(1, &_texture);
    }
}

bool axFont::operator== (const bool& exist)
{
    return _isReady == exist;
}

bool axFont::operator!= (const bool& exist)
{
    return _isReady != exist;
}

bool axFont::IsFontReady() const
{
    return _isReady;
}

axFont::operator bool() const
{
    return _isReady;
}

int axFont::GetFontSize() const
{
    return _font_size;
}

axSize axFont::GetSize() const
{
    return _size;
}

axPoint axFont::GetDelta() const
{
    return _delta;
}

int axFont::GetNextPosition() const
{
    return _next;
}

bool axFont::LoadFont(const string& path, FT_Face& face)
{
    // Zero mean succes.
    if_error_in(FT_New_Face(_freeType, path.c_str(), 0, &_face))
    {
        return true;
    }
    
    return false;
}

void axFont::SetFontSize(const int& size)
{
    if(_isReady)
    {
        _font_size = size;
        FT_Set_Pixel_Sizes(_face, 0, size);
    }
}

void axFont::SetChar(const char& letter)
{
    if(_isReady)
    {
        // Zero mean succes.
        if_error_in(FT_Load_Char(_face, letter, FT_LOAD_RENDER))
        {
            axError("Could not load character ", letter);
        }
        else
        {
            FT_GlyphSlot g = _face->glyph;
            _size = axSize(g->bitmap.width, g->bitmap.rows);
            
            _delta = axPoint(_face->glyph->bitmap_left,
                             _face->glyph->bitmap_top);
            
            _next = g->advance.x / 64.0;
            
            glBindTexture(GL_TEXTURE_2D, _texture);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            
            glTexImage2D(GL_TEXTURE_2D,
                         0,
                         GL_ALPHA,
                         g->bitmap.width,
                         g->bitmap.rows,
                         0,
                         GL_ALPHA,
                         GL_UNSIGNED_BYTE,
                         g->bitmap.buffer);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }
    }
}

void axFont::SetFontType(const string& font_type)
{
    axError("SetFontType not implemented yet.");
}

GLuint axFont::GetTexture()
{
	return _texture;
}