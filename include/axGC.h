/// @defgroup Graphic 
/// @{

#ifndef __AX_GC__
#define __AX_GC__

#include "axImage.h"
#include "axColor.h"
#include "axFont.h"

#include <cmath>

class axWindow;

class axGC
{
public:
	axGC(axWindow* win);

	void SetColor(const axColor& color);
    
    void SetColor(const axColor& color, const float& alpha);

	void SetColor(const axDouble& r,
                  const axDouble& g,
                  const axDouble& b);
    
    void SetColor(const axDouble& r,
                  const axDouble& g,
                  const axDouble& b,
                  const axDouble& a);

	void DrawRectangle(const axRect& rect);

	void DrawRectangleContour(const axRect& rect, float linewidth = 1.0);

	void DrawImage(axImage* img, const axPoint& pos, double alpha = 1.0);
    
	void DrawImageResize(axImage* img,
                         const axPoint& position,
                         const axSize& size,
                         double alpha = 1.0);

	void DrawPartOfImage(axImage* img,
                         const axPoint& posInImage,
                         const axSize& sizeInImage,
                         const axPoint& pos);

	void DrawString(const string& text, const axPoint& pos);

	void DrawStringAlignedCenter(const string& text, 
								 //const axPoint& pos, 
								 const axRect& rect);

	void SetFontType(const string& font_type){ _font.SetFontType(font_type); }

	void SetFontSize(const int& size) { _font.SetFontSize(size); }



	void DrawRectangleColorFade(const axRect& rect,
                                const axColor& c1,
                                const float& alpha1,
                                const axColor& c2,
                                const float& alpha2);

	void DrawLine(const axPoint& pt1, const axPoint& pt2, float width = 1.0);
	void DrawLines(const vector<axPoint>& pts, float width = 1.0);

    void SetLineWidth(const double& width);
    void SeDefaultLine();
    
	void DrawCircle(const axPoint& pos, float r, int num_segments);
	
	void DrawTexture(GLuint texture,
                     const axRect& rect,
                     axColor color = axColor(0.0, 0.0, 0.0));

private:
	axWindow* _win;
	axFont _font;
	//axRect _win_abs_position;

	axFloatRect RectToFloatRect(const axRect& rect);
	
};

// axFloatRect RectToFloatRect(const axRect& rect);

#endif //__AX_GC__

/// @}