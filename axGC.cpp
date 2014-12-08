#include "axGC.h"
#include "axWindow.h"
#include "axMath.h"

axGC::axGC(axWindow* win):
	_font("tt")
{
	_win = win;

}

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
    axMatrix4 mview_before(GL_MODELVIEW_MATRIX);
    axMatrix4 mview;
    
    mview.Identity().Translate(_win->GetAbsoluteRect().position).Process();
    
    axFloatRect frect = RectToFloatRect(rect);
    axRectFloatPoints points = frect.GetPoints(); // Order : bl, tl, tr, br.
    
    // For scroll bar.
    //	frect.position.x  -= floor(_win->GetScrollDecay().x);
    //	frect.position.y  -= _win->GetScrollDecay().y;
    
	//GLubyte indices[] = { 0, 1, 2, 2, 3 }; // GL_TRIANGLE_FAN.
    //glEnableClientState(GL_VERTEX_ARRAY);
    //glVertexPointer(2, GL_DOUBLE, 0, &points);
    //glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_BYTE, indices);
    //glDisableClientState(GL_VERTEX_ARRAY);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_DOUBLE, 0, &points);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
    
    mview_before.Load();
}

void axGC::DrawRoundedRectangle(const axRect& rect)
{
    /// @todo.
}

void axGC::DrawRectangleContour(const axRect& rect, float linewidth)
{
    axMatrix4 mview_before(GL_MODELVIEW_MATRIX);
    axMatrix4 mview;
    mview.Identity().Translate(_win->GetAbsoluteRect().position).Process();
    
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

    mview_before.Load();
    
    glLineWidth(1.0f);
}

void axGC::DrawTexture(GLuint texture, const axRect& rect, axColor color)
{
	axPoint pos = rect.position + _win->GetAbsoluteRect().position;
	pos.x  -= _win->GetScrollDecay().x;
	pos.y  -= _win->GetScrollDecay().y;

	//axColorStruct c = color.GetColorStruct();
	//glColor4f(c.r, c.g, c.b, 1.0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, texture);

	axREMOVE_ON_RELEASE
	(
		GLenum err = glGetError();

		if (err != GL_NO_ERROR)
		{
			DSTREAM(3) << "GL TEXTURE ERROR : " << " " <<
				gluErrorString(err) << endl;
		}
	)

	//if (err != GL_NO_ERROR)
	//{
	//	cout << "GL TEXTURE ERROR : " << hex << err << " " << 
	//			 gluErrorString(err) << endl;
	//}

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
	axPoint pos = position + _win->GetAbsoluteRect().position;
	pos -= _win->GetScrollDecay();

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
	axPoint pos = position + _win->GetAbsoluteRect().position;
	pos -= _win->GetScrollDecay();

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
					 const axPoint& position)
{
	axPoint pos = position + _win->GetAbsoluteRect().position;
	pos -= _win->GetScrollDecay();

	axSize img_size = img->GetSize();

	double img_x = (posInImage.x + sizeInImage.x) / double(img_size.x),
		img_y = 1.0 - (posInImage.y + sizeInImage.y) / double(img_size.y);

	double x = posInImage.x / double(img_size.x);
	double y = 1.0 - posInImage.y / double(img_size.y);

	glColor4f(1.0, 1.0, 1.0, 1.0);

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
	axFloatRect rect = RectToFloatRect(rectangle + _win->GetAbsoluteRect().position);
	rect.position.x  -= _win->GetScrollDecay().x;
	rect.position.y  -= _win->GetScrollDecay().y;

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
    axFloatRect rect = RectToFloatRect(rectangle + _win->GetAbsoluteRect().position);
//    rect.position.x  -= _win->GetScrollDecay().x;
//    rect.position.y  -= _win->GetScrollDecay().y;
    
    glBegin(GL_QUADS);
    SetColor(c1);
    glVertex3f(rect.position.x, rect.position.y, 0); // Bottom left.
    
    //SetColor(c1);
    glVertex3f(rect.position.x + rect.size.x,
               rect.position.y, 0); // Bottom Right.
    
    SetColor(c2);
    glVertex3f(rect.position.x + rect.size.x,
               rect.position.y + rect.size.y, 0); // Top Right.
    
    //SetColor(c2);
    glVertex3f(rect.position.x,
               rect.position.y + rect.size.y, 0); // Top Left
    glEnd();
}

void axGC::DrawLines(const vector<axPoint>& pts, float width)
{
	axPoint real_pos = _win->GetAbsoluteRect().position;
	real_pos.x -= _win->GetScrollDecay().x;
	real_pos.y -= _win->GetScrollDecay().y;

    glPushMatrix();
    glTranslated(real_pos.x, real_pos.y, 0.0);

	glLineWidth(width);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, 0, pts.data());
    glDrawArrays(GL_LINES, 0, int(pts.size()));
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPopMatrix();
}

void axGC::DrawLine(const axPoint& pt1, const axPoint& pt2, float width)
{
	axPoint real_pos = _win->GetAbsoluteRect().position;
	real_pos.x  -= _win->GetScrollDecay().x;
	real_pos.y  -= _win->GetScrollDecay().y;

	axPoint p1 = pt1 + real_pos;
	axPoint p2 = pt2 + real_pos;

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

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

void axGC::DrawCircle(const axPoint& pos, float r, int num_segments) 
{ 
	axPoint real_pos = _win->GetAbsoluteRect().position;
	real_pos -= _win->GetScrollDecay();

	real_pos += pos;

	glBegin(GL_LINE_LOOP); 
	for(int i = 0; i < num_segments; i++) 
	{ 
		float theta = 2.0f * 3.1415926f * double(i) / double(num_segments);//get the current angle 

		float x = r * cosf(theta);//calculate the x component 
		float y = r * sinf(theta);//calculate the y component 

		glVertex2f(x + real_pos.x, y + real_pos.y);//output vertex 

	} 
	glEnd(); 
}