/** @defgroup Graphic
 *  @{
 */
#ifndef __AX_FONT__
#define __AX_FONT__

#include <ft2build.h>
#include FT_FREETYPE_H

#include "axC++.h"
#include "axImage.h"
#include <map>

class axFont
{
public:
	axFont(const string& font);

	GLuint GetTexture();
	void SetFontSize(const int& size);
	void SetChar(const char& letter);
	axSize GetSize() const { return _size; }
	axPoint GetDelta() const { return _delta; }
	int GetNextPosition() const { return _next; }
	void SetFontType(const string& font_type);
	int GetFontSize() const
	{
		return _font_size;
	}

private:
	FT_Library _freeType;
	FT_Face _face;
	GLuint _texture;
	axSize _size;
	axPoint _delta;
	int _next;
	int _font_size;

	bool InitFreeType();
};

#endif // __AX_FONT__

/** @} */ // Graphic group.