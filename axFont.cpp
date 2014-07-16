#include "axFont.h"

axFont::axFont(const string& font)
	//_font_img(font)
{
	if_no_error_in(InitFreeType())
	{
		// Set default size.
		SetFontSize(18);
	}

	glGenTextures(1, &_texture);
}

//axPoint axFont::GetLetterPosition(const char& letter)
//{
//	return _letters.find(letter)->second;
//}

void axFont::SetFontSize(const int& size)
{
	_font_size = size;
	FT_Set_Pixel_Sizes(_face, 0, size);
}

void axFont::SetChar(const char& letter)
{
	if_error_in(FT_Load_Char(_face, letter, FT_LOAD_RENDER))
	{
		DSTREAM << "Error : Could not load character " << letter << endl;
	}
	else
	{
		FT_GlyphSlot g = _face->glyph;
		//DSTREAM << letter << " : Font size ( " << g->bitmap.width << ", " << g->bitmap.rows << ")" <<  endl;

		_size = axSize(g->bitmap.width, g->bitmap.rows);

		_delta = axPoint(_face->glyph->bitmap_left, _face->glyph->bitmap_top);
		//DSTREAM << letter << " : Font delta ( " << _delta.x << ", " << _delta.y << ")" << endl;
		//if (letter == '1')
		//	_next = _size.x + 1;
		//else
		_next = g->advance.x / 64.0;
		//DSTREAM << "NEXT : " << _next / 64.0  << endl;

		//DSTREAM << "TOP VALUE: " <<_face->glyph->bitmap_top << endl;

		//glGenTextures(1, &_texture);
		glBindTexture(GL_TEXTURE_2D, _texture);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_ALPHA,
			g->bitmap.width,
			g->bitmap.rows,
			0,
			GL_ALPHA,
			GL_UNSIGNED_BYTE,
			g->bitmap.buffer
			);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
		//			 g->bitmap.width, 
		//			 g->bitmap.rows,
		//			 0, GL_RGBA, 
		//			 GL_UNSIGNED_BYTE, 
		//			 g->bitmap.buffer);
		
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}
void axFont::SetFontType(const string& font_type)
{
	// FreeType error code. 0 means success.
	if(FT_Done_Face(_face) == 0)
	{
		const char* c = font_type.c_str();

		// FreeType error code. 0 means success.
		if (FT_New_Face(_freeType, font_type.c_str(), 0, &_face) == 0)
		{
			SetFontSize(_font_size);
		}
		else
		{
			cerr << "Error : Could not open font." << endl;
		}
	}
	else
	{
		cerr << "FT_Done_Face Error !" << endl;
	}	
}

bool axFont::InitFreeType()
{
	if (FT_Init_FreeType(&_freeType))
	{
		DSTREAM << "Error : Could not init freetype library." << endl;
		return false;
	}

	if (FT_New_Face(_freeType, "/home/alexarse/Desktop/axLib/ressources/axFonts/FreeSans.ttf", 0, &_face))
	{
		DSTREAM << "Init error : Could not open font." << endl;
		return false;
	}

	return true;
}

GLuint axFont::GetTexture()
{
	return _texture;
}