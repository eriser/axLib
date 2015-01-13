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
#include "axApp.h"

axFontGlobalManager axFont::_fontManager;

axFontGlobalManager::axFontGlobalManager()
{
	if (FT_Init_FreeType(&_freeType))
	{
        std::cerr << "Error : Could not init freetype library." << std::endl;
	}
}

bool axFontGlobalManager::LoadFont(const string& path, FT_Face& face)
{
	std::map<std::string, axFontStruct>::iterator it = _fontMap.find(path);

	if (it != _fontMap.end())
	{
        FT_New_Memory_Face(_freeType,
                           (FT_Byte*)it->second._data, // First byte in memory.
                           it->second._size, // Size in bytes.
                           0, // Face_index.
                           &face);
		return true;
	}
	else
	{
		std::ifstream file(path, std::ios::binary);
		file.seekg(0, std::ios::end);
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		
		char* buffer = nullptr;
		try
		{
			buffer = new char[size];
		}
		catch (std::bad_alloc)
		{
            std::cerr << "Error LoadFont new buffer." << std::endl;
		}

		if (file.read(buffer, size))
		{
			axFontStruct font_info(buffer, (unsigned int)size);
			_fontMap.insert(pair<std::string, axFontStruct>(path, font_info));

			FT_New_Memory_Face(_freeType,
							   (FT_Byte*)buffer, // First byte in memory.
							   size, // Size in bytes.
							   0, // Face_index.        
				               &face);

			file.close();

			return true;
		}
		else
		{
			delete buffer;
		}
	}

	return false;
}


axFont::axFont(const string& font)
{
	if (_fontManager.LoadFont(axApp::GetInstance()->GetAppDirectory() + 
		std::string("FreeSans.ttf"), _face))
    //if (_fontManager.LoadFont(std::string("/Users/alexarse/Project/axLib/ressources/axFonts/") +
    //                          std::string("FreeSans.ttf"), _face))
	{
		SetFontSize(12);
	}
	else
	{
        std::cerr << "Error loading font" << std::endl;
	}
	

	glGenTextures(1, &_texture);
}

void axFont::SetFontSize(const int& size)
{
	_font_size = size;
	FT_Set_Pixel_Sizes(_face, 0, size);
}

void axFont::SetChar(const char& letter)
{
	if_error_in(FT_Load_Char(_face, letter, FT_LOAD_RENDER))
	{
        std::cerr << "Error : Could not load character " << letter << std::endl;
	}
	else
	{
		FT_GlyphSlot g = _face->glyph;
		_size = axSize(g->bitmap.width, g->bitmap.rows);

		_delta = axPoint(_face->glyph->bitmap_left, _face->glyph->bitmap_top);
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
        
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void axFont::SetFontType(const string& font_type)
{
    if (_fontManager.LoadFont(font_type, _face))
    {
        SetFontSize(12);
    }
}

bool axFont::InitFreeType()
{
	// Init FreeType library.
	if (FT_Init_FreeType(&_freeType))
	{
        std::cerr << "Error : Could not init freetype library." << std::endl;
		return false;
	}

	if (FT_New_Face(_freeType, "FreeSans.ttf", 0, &_face))
	{
        std::cerr << "Init error : Could not open font." << std::endl;
		return false;
	}

	return true;
}

GLuint axFont::GetTexture()
{
	return _texture;
}