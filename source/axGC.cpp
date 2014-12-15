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
#include "axGC.h"
#include "axWindow.h"
#include "axMath.h"

#include "axApp.h"
#include "axCore.h"

#include <cmath>

axGC::axGC(axWindow* win):
	_font("tt")
{
	_win = win;

}

//void axGC::BeginDrawing()
//{
//    axMatrix4 mview;
//    mview.Identity().Translate(_win->GetAbsoluteRect().position).Process();
//}
//
//void axGC::EndDrawing()
//{
//    axMatrix4 mview;
//    axPoint revert_pos(_win->GetAbsoluteRect().position);
//    revert_pos.x = -revert_pos.x;
//    revert_pos.y = -revert_pos.y;
//    mview.Identity().Translate(revert_pos).Process();
//}

axFloatRect axGC::RectToFloatRect(const axRect& rect)
{
	return axFloatRect(axFloat(rect.position.x), axFloat(rect.position.y),
					   axFloat(rect.size.x), axFloat(rect.size.y));
}

void axGC::SetColor(const axDouble& r, const axDouble& g, const axDouble& b)
{
	glColor4d(r, b, g, 1.0);
}

void axGC::SetColor(const axDouble& r,
                    const axDouble& g,
                    const axDouble& b,
                    const axDouble& a)
{
    glColor4d(r, g, b, a);
}

void axGC::SetColor(const axColor& color)
{
	glColor4d(color.GetRed(),
              color.GetGreen(),
              color.GetBlue(),
              color.GetAlpha());
}

void axGC::SetColor(const axColor& color, const float& alpha)
{
	glColor4d(color.GetRed(), color.GetGreen(), color.GetBlue(), alpha);
}

void axGC::DrawRectangle(const axRect& rect)
{
//    axMatrix4 mview_before(GL_MODELVIEW_MATRIX);
//    axMatrix4 mview;
    
//    mview.Identity().Translate(_win->GetAbsoluteRect().position).Process();
    
    axFloatRect frect = RectToFloatRect(rect);
    axRectFloatPoints points = frect.GetPoints(); // Order : bl, tl, tr, br.
    
    // For scroll bar.
    //	frect.position.x  -= floor(_win->GetScrollDecay().x);
    //	frect.position.y  -= _win->GetScrollDecay().y;

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_DOUBLE, 0, &points);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
    
//    mview_before.Load();
}

void axGC::DrawRoundedRectangle(const axRect& rect)
{
    /// @todo.
}

void axGC::DrawRectangleContour(const axRect& rect, float linewidth)
{
//    axMatrix4 mview_before(GL_MODELVIEW_MATRIX);
//    axMatrix4 mview;
//    mview.Identity().Translate(_win->GetAbsoluteRect().position).Process();
    
	axFloatRect frect = RectToFloatRect(rect);
//	frect.position.x  -= floor(_win->GetScrollDecay().x);
//	frect.position.y  -= _win->GetScrollDecay().y;
    
	// Note that OpenGL coordinate space has no notion of integers, 
	// everything is a float and the "centre" of an OpenGL pixel is 
	// really at the 0.5,0.5 instead of its top-left corner. 
	// Therefore, if you want a 1px wide line from 0,0 to 10,10 inclusive, 
	// you really had to draw a line from 0.5,0.5 to 10.5,10.5. 
	frect.position.x -= 0.5; // Seems to be only on linux and mac.

	glLineWidth((GLfloat)linewidth);
    
    axRectFloatPoints points = frect.GetPoints(); // Order : bl, tl, tr, br.
    GLubyte indices[] = {1,2, 2,3, 3,0, 0,1};
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_DOUBLE, 0, &points); // The value of z defaults is 0.
    glDrawElements(GL_LINE_LOOP, 8, GL_UNSIGNED_BYTE, indices);
    glDisableClientState(GL_VERTEX_ARRAY);

//    mview_before.Load();
    
    glLineWidth(1.0f);
}

void axGC::DrawTexture(GLuint texture, const axRect& rect, axColor color)
{
//	axPoint pos = rect.position + _win->GetAbsoluteRect().position;
    axPoint pos = rect.position;
//	pos.x  -= _win->GetScrollDecay().x;
//	pos.y  -= _win->GetScrollDecay().y;

	//axColorStruct c = color.GetColorStruct();
	//glColor4f(c.r, c.g, c.b, 1.0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, texture);

//	axREMOVE_ON_RELEASE
//	(
//		GLenum err = glGetError();
//
//		if (err != GL_NO_ERROR)
//		{
//			DSTREAM(3) << "GL TEXTURE ERROR : " << " " <<
//				gluErrorString(err) << endl;
//		}
//	)

	glDepthMask(GL_TRUE);
	axSize img_size = rect.size;

	glBegin(GL_QUADS);
	
	// Bottom left.
	glTexCoord2d(0.0, 0.0);
	glVertex2d(pos.x, pos.y);

	// Top left.
	glTexCoord2d(0.0, 1.0);
	glVertex2d(pos.x, pos.y + img_size.y);

	// Top right.
	glTexCoord2d(1.0, 1.0);
	glVertex2d(pos.x + img_size.x, pos.y + img_size.y);

	// Buttom right.
	glTexCoord2d(1.0, 0.0);
	glVertex2d(pos.x + img_size.x, pos.y);

	glEnd();

//	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

//glTexCoordPointer(2, GL_FLOAT, 0, texCoords);

struct axRectPointsOrder
{
    axRectPointsOrder(){}
    axRectPointsOrder(const axRectFloatPoints& points):
    top_left(points.top_left),
    top_right(points.top_right),
    bottom_left(points.bottom_left),
    bottom_right(points.bottom_right)
    {
    }
    axRectPointsOrder(const axFloatPoint& tl,
                      const axFloatPoint& tr,
                      const axFloatPoint& bl,
                      const axFloatPoint& br):
    top_left(tl), top_right(tr), bottom_left(bl),bottom_right(br)
    {
        
    }
    
    axFloatPoint top_left, top_right, bottom_left, bottom_right;
};
void axGC::DrawImage(axImage* img, const axPoint& position, double alpha)
{
//	axPoint pos = position + _win->GetAbsoluteRect().position;
    axPoint pos = position;
	//pos -= _win->GetScrollDecay();

	glColor4f(1.0, 1.0, 1.0, alpha);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, img->GetTexture());
	glDepthMask(GL_TRUE);
	axSize img_size = img->GetSize();

	// OpenGL stores texture upside down so glTexCoord2d must be flipped.
	glBegin(GL_QUADS);
	// Buttom left.
	glTexCoord2d(0.0, 1.0);
	glVertex2d(pos.x, pos.y);

	// Top left.
	glTexCoord2d(0.0, 0.0);
	glVertex2d(pos.x, pos.y + img_size.y);

	// Top right.
	glTexCoord2d(1.0, 0.0);
	glVertex2d(pos.x + img_size.x, pos.y + img_size.y);

	// Buttom right.
	glTexCoord2d(1.0, 1.0);
	glVertex2d(pos.x + img_size.x, pos.y);
	glEnd();
//	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void axGC::DrawImageResize(axImage* img, const axPoint& position, const axSize& size, double alpha)
{
//	axPoint pos = position + _win->GetAbsoluteRect().position;
    axPoint pos = position;
	//pos -= _win->GetScrollDecay();

	glColor4f(1.0, 1.0, 1.0, alpha);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, img->GetTexture());
	glDepthMask(GL_TRUE);
	axSize img_size = size;

	// OpenGL stores texture upside down so glTexCoord2d must be flipped.
	glBegin(GL_QUADS);
	// Buttom left.
	glTexCoord2d(0.0, 1.0);
	glVertex2d(pos.x, pos.y);

	// Top left.
	glTexCoord2d(0.0, 0.0);
	glVertex2d(pos.x, pos.y + img_size.y);

	// Top right.
	glTexCoord2d(1.0, 0.0);
	glVertex2d(pos.x + img_size.x, pos.y + img_size.y);

	// Buttom right.
	glTexCoord2d(1.0, 1.0);
	glVertex2d(pos.x + img_size.x, pos.y);
	glEnd();

//	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void axGC::DrawPartOfImage(axImage* img,
					 const axPoint& posInImage,
					 const axSize& sizeInImage,
					 const axPoint& position,
                     double alpha)
{
//	axPoint pos = position + _win->GetAbsoluteRect().position;
    
    axPoint pos = position;
	//pos -= _win->GetScrollDecay();

	axSize img_size = img->GetSize();

	double img_x = (posInImage.x + sizeInImage.x) / double(img_size.x),
		img_y = 1.0 - (posInImage.y + sizeInImage.y) / double(img_size.y);

	double x = posInImage.x / double(img_size.x);
	double y = 1.0 - posInImage.y / double(img_size.y);

	glColor4f(1.0, 1.0, 1.0, alpha);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, img->GetTexture());
	glDepthMask(GL_TRUE);
	
	// OpenGL stores texture upside down so glTexCoord2d must be flipped.
	glBegin(GL_QUADS);
	
	// Buttom left.
	glTexCoord2d(x, y);
	glVertex2d(pos.x, pos.y);

	// Top left.
	glTexCoord2d(x, img_y);
	glVertex2d(pos.x, pos.y + sizeInImage.y);

	// Top right.
	glTexCoord2d(img_x, img_y);
	glVertex2d(pos.x + sizeInImage.x, pos.y + sizeInImage.y);

	// Buttom right.
	glTexCoord2d(img_x, y);
	glVertex2d(pos.x + sizeInImage.x, pos.y);
	glEnd();
//	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void axGC::SetFontSize(const int& size)
{
    _font.SetFontSize(size);
}

void axGC::DrawString(const string& text, const axPoint& pos)
{
	// axPoint pos = position + _win->GetAbsoluteRect().position;
	// pos -= _win->GetScrollDecay();

	int x = pos.x;

	for (int i = 0; i < text.size(); i++)
	{
		_font.SetChar(text[i]);
		axPoint delta = _font.GetDelta();

		DrawTexture(_font.GetTexture(), 
			axRect(axPoint(x + delta.x, pos.y - delta.y + _font.GetFontSize()),
					_font.GetSize()));

		x += _font.GetNextPosition();
	}
}

axPoint axGC::DrawChar(const char& key, const axPoint& pos)
{
    // axPoint pos = position + _win->GetAbsoluteRect().position;
    // pos -= _win->GetScrollDecay();
    
    int x = pos.x;
    
        _font.SetChar(key);
        axPoint delta = _font.GetDelta();
    
    //std::cout << "Delta : " << delta.x << std::endl;
    
        DrawTexture(_font.GetTexture(),
                    axRect(axPoint(x + delta.x, pos.y - delta.y + _font.GetFontSize()),
                           _font.GetSize()));
        
        x += _font.GetNextPosition();
    
    return axPoint(x, pos.y);
}


// Just blocking x axis for now.
void axGC::BlockDrawing(const axRect& rect)
{
    axMatrix4 before_proj(GL_MODELVIEW);
    axMatrix4 proj;
    proj.Identity().Load();
    
//    axRect r = rect;
    
    axPoint abso(_win->GetAbsoluteRect().position);
    
    glScissor(abso.x + rect.position.x,
              axApp::GetInstance()->GetCore()->GetGlobalSize().y -
              (abso.y + rect.position.y + rect.size.y),
              rect.size.x,
              rect.size.y);
    
    glEnable(GL_SCISSOR_TEST);
    before_proj.Load();
}

void axGC::UnBlockDrawing()
{
    glDisable(GL_SCISSOR_TEST);
}

void axGC::DrawStringAlignedCenter(const string& text,
								   //const axPoint& pos,
								   const axRect& rect)
{
	int length = 0;
	int height = 0;
	for (int i = 0; i < text.size(); i++)
	{
		_font.SetChar(text[i]);
		length += _font.GetSize().x;// +_font.GetNextPosition();
		
		if (_font.GetSize().y > height)
			height = _font.GetSize().y;
	}

	axPoint pos(rect.position.x  + ceil((rect.size.x - length) * 0.5),
		rect.position.y + ceil((rect.size.y - height) * 0.5));

	int x = pos.x;
	for (int i = 0; i < text.size(); i++)
	{
		_font.SetChar(text[i]);
		axPoint delta = _font.GetDelta();

		DrawTexture(_font.GetTexture(),
			axRect(axPoint(x + delta.x, pos.y - delta.y + height/*_font.GetFontSize()*/),
			_font.GetSize()));

		x += _font.GetNextPosition();
	}
}

void axGC::DrawRectangleColorFade(const axRect& rectangle,
								  const axColor& c1, const float& alpha1,
								  const axColor& c2, const float& alpha2)
{
//	axFloatRect rect = RectToFloatRect(rectangle + _win->GetAbsoluteRect().position);
    axFloatRect rect = RectToFloatRect(rectangle);
	//rect.position.x  -= _win->GetScrollDecay().x;
	//rect.position.y  -= _win->GetScrollDecay().y;

	glBegin(GL_QUADS);
	SetColor(c1, alpha1);
	glVertex3f(rect.position.x, rect.position.y, 0); // Bottom left.

	//SetColor(c1);
	glVertex3f(rect.position.x + rect.size.x,
		rect.position.y, 0); // Bottom Right.

	SetColor(c2, alpha2);
	glVertex3f(rect.position.x + rect.size.x,
		rect.position.y + rect.size.y, 0); // Top Right.

	//SetColor(c2);
	glVertex3f(rect.position.x,
		rect.position.y + rect.size.y, 0); // Top Left
	glEnd();
}

void axGC::DrawRectangleColorFade(const axRect& rectangle,
                                  const axColor& c1,
                                  const axColor& c2)
{
//    axFloatRect rect = RectToFloatRect(rectangle + _win->GetAbsoluteRect().position);
    axFloatRect rect = RectToFloatRect(rectangle);
    
    glBegin(GL_QUADS);
    SetColor(c1);
    glVertex3f(rect.position.x, rect.position.y, 0); // Bottom left.
    
    glVertex3f(rect.position.x + rect.size.x,
               rect.position.y, 0); // Bottom Right.
    
    SetColor(c2);
    glVertex3f(rect.position.x + rect.size.x,
               rect.position.y + rect.size.y, 0); // Top Right.
    
    glVertex3f(rect.position.x,
               rect.position.y + rect.size.y, 0); // Top Left
    glEnd();
}

void axGC::DrawLines(const vector<axPoint>& pts, float width)
{
//	axPoint real_pos = _win->GetAbsoluteRect().position;
//    axPoint real_pos = _win->GetRect().position;
//	real_pos.x -= _win->GetScrollDecay().x;
//	real_pos.y -= _win->GetScrollDecay().y;
//
//    glPushMatrix();
//    glTranslated(real_pos.x, real_pos.y, 0.0);

	glLineWidth(width);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, 0, pts.data());
    glDrawArrays(GL_LINES, 0, int(pts.size()));
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPopMatrix();
}

void axGC::DrawLine(const axPoint& pt1, const axPoint& pt2, float width)
{
//	axPoint real_pos = _win->GetAbsoluteRect().position;
//    axPoint real_pos = _win->GetRect().position;
//	real_pos.x  -= _win->GetScrollDecay().x;
//	real_pos.y  -= _win->GetScrollDecay().y;

    axPoint p1 = pt1;// - _win->GetScrollDecay().x;;// + real_pos;
    axPoint p2 = pt2;// - _win->GetScrollDecay().y;// + real_pos;

//	glEnable(GL_LINE_SMOOTH);
//	glEnable(GL_POLYGON_SMOOTH);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glLineWidth(width);

	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
}

void axGC::SetLineWidth(const double& width)
{
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    
    glLineWidth(width);

}

void axGC::SeDefaultLine()
{
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glLineWidth(1.0);
}

void axGC::DrawCircle(const axPoint& pos,
                      const double& radius,
                      const int& nSegments)
{ 
//	axPoint real_pos = _win->GetAbsoluteRect().position;
    axPoint real_pos = pos;//_win->GetRect().position;

	//real_pos -= _win->GetScrollDecay();

//	real_pos += pos;

	glBegin(GL_LINE_LOOP); 
	for(int i = 0; i < nSegments; i++)
	{
        // Get the current angle.
		double theta = 2.0f * M_PI * double(i) / double(nSegments);

		double x = radius * cosf(theta);
		double y = radius * sinf(theta);

		glVertex2d(x + real_pos.x, y + real_pos.y);
	} 
	glEnd(); 
}