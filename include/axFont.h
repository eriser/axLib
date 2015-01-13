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

#ifndef __AX_FONT__
#define __AX_FONT__

/*******************************************************************************
 * @file    axFont.h
 * @author  Alexandre Arsenault <alx.arsenault@gmail.com>
 * @brief   axFont.
 * @date    13/01/2015
 ******************************************************************************/

/// @defgroup Graphic
/// @{

#include <ft2build.h>
#include FT_FREETYPE_H

#include "axC++.h"

const axByte* GetDefaultFontData(const int& index);
const long GetDefaultFontDataSize(const int& index);

class axFont
{
public:
	axFont(const string& font);
    
    axFont();
    
    axFont(const axFont&) = delete;
    axFont& operator=(const axFont&) = delete;
    
//    axFont(axFont&& font);
    
    axFont(const int& data_index);
    
    ~axFont();

	GLuint GetTexture();
    
    bool operator== (const bool& exist);
    
    bool operator!= (const bool& exist);
    
    bool IsFontReady() const;
    
    operator bool() const;
    
    // Char won't work if SetFontSize is not call.
	void SetFontSize(const int& size);
    
	void SetChar(const char& letter);
    
    axSize GetSize() const;
    
    axPoint GetDelta() const;
    
    int GetNextPosition() const;
    
	void SetFontType(const string& font_type);
    
    int GetFontSize() const;

private:
	FT_Library _freeType;
	FT_Face _face;
	GLuint _texture;
	axSize _size;
	axPoint _delta;
	int _next, _font_size;
    bool _isReady;
    
    bool LoadFont(const string& path, FT_Face& face);
};

/// @}
#endif // __AX_FONT__