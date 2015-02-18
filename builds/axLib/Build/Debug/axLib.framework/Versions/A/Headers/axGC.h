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
#ifndef __AX_GC__
#define __AX_GC__

/// @defgroup Graphic
/// @{

#include "axImage.h"
#include "axColor.h"
#include "axFont.h"

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

    void DrawRoundedRectangle(const axRect& rect);
    
	void DrawImage(axImage* img, const axPoint& pos, double alpha = 1.0);
    
	void DrawImageResize(axImage* img,
                         const axPoint& position,
                         const axSize& size,
                         double alpha = 1.0);

	void DrawPartOfImage(axImage* img,
                         const axPoint& posInImage,
                         const axSize& sizeInImage,
                         const axPoint& pos,
                         double alpha = 1.0);
    
    void DrawPartOfImageResize(axImage* img,
                               const axPoint& posInImage,
                               const axSize& sizeInImage,
                               const axRect& rect,
                               double alpha = 1.0);

    axPoint DrawChar(axFont& font,
                     const char& key,
                     const axPoint& pos);
    
	void DrawString(axFont& font,
                    const string& text,
                    const axPoint& pos);

	void DrawStringAlignedCenter(axFont& font,
                                 const string& text,
                                 const axRect& rect);

	void DrawRectangleColorFade(const axRect& rect,
                                const axColor& c1,
                                const float& alpha1,
                                const axColor& c2,
                                const float& alpha2);
    
    void DrawRectangleColorFade(const axRect& rect,
                                const axColor& c1,
                                const axColor& c2);

	void DrawLine(const axPoint& pt1, const axPoint& pt2, float width = 1.0);
    void DrawSmouthLine(const axPoint& pt1, const axPoint& pt2);
	void DrawLines(const vector<axPoint>& pts, float width = 1.0);
    
    void DrawPoint(const axPoint& pt, const int& size);
    
    /// @todo This function is not finish. (Add some param)
    void DrawLineCubic(const axPoint& pt1, const axPoint& pt2);

    void SetLineWidth(const double& width);
    void SeDefaultLine();
    
	void DrawCircle(const axPoint& pos,
                    const double& radius,
                    const int& nSegments);
	
	void DrawTexture(GLuint texture,
                     const axRect& rect,
                     axColor color = axColor(0.0, 0.0, 0.0));
    
//    void DrawWindowBuffer();
    
    // Just blocking x axis for now.
    void BlockDrawing(const axRect& rect);
    void UnBlockDrawing();

private:
	axWindow* _win;
//	axFont _font;
    
	axFloatRect RectToFloatRect(const axRect& rect);
};

/// @}
#endif //__AX_GC__

