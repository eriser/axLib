#include "axFont.h"

//std::ifstream file("myfile", std::ios::binary);
//file.seekg(0, std::ios::end);
//std::streamsize size = file.tellg();
//file.seekg(0, std::ios::beg);
//
//std::vector<unsigned char> buffer(size);
//if (file.read(buffer.data(), size))
//{
//	/* worked! */
//}

axFontGlobalManager axFont::_fontManager;

axFontGlobalManager::axFontGlobalManager()
{
	if (FT_Init_FreeType(&_freeType))
	{
		cerr << "Error : Could not init freetype library." << endl;
	}
}

bool axFontGlobalManager::LoadFont(const string& path, FT_Face& face)
{
	std::map<std::string, axFontStruct>::iterator it = _fontMap.find(path);

	if (it != _fontMap.end())
	{
		//cout << "Font already loaded in memory." << endl;
		FT_New_Memory_Face(_freeType,
							(FT_Byte*)it->second._data, // First byte in memory.
							it->second._size, // Size in bytes.
							0, // Face_index.        
							&face);
		return true;
	}
	else
	{
		//cout << "Init new font." << endl;
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
			cerr << "Error new buffer." << endl;
		}

		//std::vector<unsigned char> buffer(size);

		if (file.read(buffer, size))
		{
			axFontStruct font_info(buffer, size);
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
		//if (InitImage(path, _texture, _size) == false)
		//{
		//	axImageStruct img_info(_texture, _size);
		//	_imageMap.insert(pair<std::string, axImageStruct>(path, img_info));
		//	return true;
		//}
	}

	return false;
}


axFont::axFont(const string& font)
	//_font_img(font)
{
	//if_no_error_in(InitFreeType())
	//{
	//	// Set default size.
	//	SetFontSize(12);
	//}
//	if (_fontManager.LoadFont("C:/Users/Alexandre Arsenault/Desktop/axLib/ressources/axFonts/FreeSans.ttf", _face))
//    if (_fontManager.LoadFont("/Users/alexarse/Project/axLib/ressources/axFonts/FreeSans.ttf",_face))
    if (_fontManager.LoadFont("FreeSans.ttf",_face))
	{
		SetFontSize(12);
	}
	else
	{
		cerr << "Error loading font" << endl;
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
		DSTREAM(1) << "Error : Could not load character " << letter << endl;
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
//		const char* c = font_type.c_str();

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
	// Init FreeType library.
	if (FT_Init_FreeType(&_freeType))
	{
		DSTREAM(1) << "Error : Could not init freetype library." << endl;
		return false;
	}

	//if (FT_New_Face(_freeType, "/home/alexarse/Desktop/axLib/ressources/axFonts/FreeSans.ttf", 0, &_face))
	if (FT_New_Face(_freeType, "FreeSans.ttf", 0, &_face))
	{
		DSTREAM(1) << "Init error : Could not open font." << endl;
		return false;
	}

	return true;
}

GLuint axFont::GetTexture()
{
	return _texture;
}