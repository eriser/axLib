#include "axCore.h"
#include <GLKit/GLKMath.h>

axCore::axCore():
	// Members.
	_needToDraw(true),
	_popupNeedToDraw(true)
{
	_windowManager = new axManager();
	 _popupManager = new axManager();

	 //_lastDrawingTime = clock();
}

axManager* axCore::GetWindowManager()
{
	return _windowManager;
}

axManager* axCore::GetPopupManager()
{
	 return _popupManager;
}

void axCore::ResizeGLScene(const int& width, const int& height)
{
	int h = height;

	// Prevent a divide by zero by.
	if (h == 0)
    {
        h = 1;
    }
    
	_size = axSize(width, h);

	// Reset the current viewport.
	glViewport(0, 0, width, h);

	// Select the projection matrix.
	glMatrixMode(GL_PROJECTION);	

	// Reset the projection matrix.
	glLoadIdentity();									

	// Calculate the aspect ratio of the window.
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)h, 0.1f, 100.0f);

	// Select the modelview matrix.
	glMatrixMode(GL_MODELVIEW);			

	// Reset the modelview matrix.
	glLoadIdentity();		
	_needToDraw = true;
	_popupNeedToDraw = true;

}

void axCore::DeletePopWindow()
{
 	KillPopGLWindow();

 	delete _popupManager;
 	_popupManager = nullptr;
}

int axCore::InitGL()										
{
	// Enable Smooth Shading.
	glShadeModel(GL_SMOOTH);				

	// Black Background
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);	

	// Depth Buffer Setup
	glClearDepth(1.0f);		

	// Enables Depth Testing
	glEnable(GL_DEPTH_TEST);		

	// The type of depth testing to do.
	glDepthFunc(GL_LEQUAL);				

	// Really nice perspective calculations.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	return true;										
}
//
//void axCore::ResizeGLScene(const int& width, const int& height)
//{
//	int h = height;
//
//	// Prevent a divide by zero by.
//	if (h == 0)	h = 1;
//	_size = axSize(width, h);
//
//	// Reset the current viewport.
//	glViewport(0, 0, width, h);
//
//	// Select the projection matrix.
//	glMatrixMode(GL_PROJECTION);
//
//	// Reset the projection matrix.
//	glLoadIdentity();
//
//	// Calculate the aspect ratio of the window.
//	gluPerspective(45.0f, (GLfloat)width / (GLfloat)h, 0.1f, 100.0f);
//
//	// Select the modelview matrix.
//	glMatrixMode(GL_MODELVIEW);
//
//	// Reset the modelview matrix.
//	glLoadIdentity();
//	_needToDraw = true;
//	_popupNeedToDraw = true;
//
//}

void axCore::ResizePopGLScene(const int& width, const int& height)
{
	int h = height;

	// Prevent a divide by zero by.
	if (h == 0)	h = 1;
		_popSize = axSize(width, h);

	// Reset the current viewport.
	glViewport(0, 0, width, h);

	// Select the projection matrix.
	glMatrixMode(GL_PROJECTION);

	// Reset the projection matrix.
	glLoadIdentity();

	// Calculate the aspect ratio of the window.
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)h, 0.1f, 100.0f);

	// Select the modelview matrix.
	glMatrixMode(GL_MODELVIEW);

	// Reset the modelview matrix.
	glLoadIdentity();
	_needToDraw = true;
	_popupNeedToDraw = true;

}

axSize axCore::GetGlobalSize() const
{
	return _size;
}

void axCore::UpdateAll()
{
	_needToDraw = true;
	_popupNeedToDraw = true;
}

int axCore::DrawGLScene()
{
    
	//glViewport(0, 0, att.width, att.height);
	// /float time = float(clock() - _lastDrawingTime) / CLOCKS_PER_SEC;
	// cout << "time  draw : " << _lastDrawingTime << "Clock per sec " << CLOCKS_PER_SEC << endl;
	//cout << time << endl;
	if (_needToDraw)// && time > 0.01)
	//if (true)
	{
		// _lastDrawingTime = clock();
		_needToDraw = false;
	
		// Clear screen and depth buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glViewport(0, 0, _size.x, _size.y);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, _size.x, _size.y, 0.0);
//        GLKMatrix4MakeOrtho (
//                             0.0,
//                             _size.x,
//                             _size.y,
//                             0.0,
//                             0.0,
//                             1.0
//                             );

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	
		axFloatRect rect(0 - 1.0, 0 - 1, _size.x * 2.0, _size.y * 2.0);

		glColor3d(0.0, 0.0, 0.0);
		GLfloat z = 0;

		glBegin(GL_QUADS);
		// Bottom left.
		glVertex3f(rect.position.x, rect.position.y, z); 

		// Bottom Right.
		glVertex3f(rect.position.x + rect.size.x, 
				   rect.position.y, z); 

		// Top Right.
		glVertex3f(rect.position.x + rect.size.x, 
				   rect.position.y + rect.size.y, z); 

		// Top Left
		glVertex3f(rect.position.x, rect.position.y + rect.size.y, z); 
		glEnd();

		_windowManager->OnPaint();
		return true;
	}
	return false;
}

int axCore::DrawGLPopScene()
{
	//glViewport(0, 0, att.width, att.height);

	//if (_needToDraw)
	if (_popupNeedToDraw)
	{
		_popupNeedToDraw = false;

		// Clear screen and depth buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glViewport(0, 0, _size.x, _size.y);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, _popSize.x, _popSize.y, 0.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		axFloatRect rect(0 - 1.0, 0 - 1, _size.x * 2.0, _size.y * 2.0);

		glColor3f(0.1, 0.1, 0.1);
		GLfloat z = 0;

		glBegin(GL_QUADS);
		// Bottom left.
		glVertex3f(rect.position.x, rect.position.y, z);

		// Bottom Right.
		glVertex3f(rect.position.x + rect.size.x,
			rect.position.y, z);

		// Top Right.
		glVertex3f(rect.position.x + rect.size.x,
			rect.position.y + rect.size.y, z);

		// Top Left
		glVertex3f(rect.position.x, rect.position.y + rect.size.y, z);
		glEnd();

		_popupManager->OnPaint();
		return true;
	}
	return false;
}
