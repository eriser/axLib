#include "axCore.h"

//axCore* axCORE = nullptr;

axCore::axCore():
	// Members.
	_needToDraw(true)
{
	_windowManager = new axManager();
}

axManager* axCore::GetWindowManager()
{
	return _windowManager;
}

void axCore::ResizeGLScene(const int& width, const int& height)
{
	int h = height;

	// Prevent a divide by zero by.
	if (h == 0)	h = 1;
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

axSize axCore::GetGlobalSize() const
{
	return _size;
}

void axCore::UpdateAll()
{
	_needToDraw = true;
}

int axCore::DrawGLScene()
{
	//glViewport(0, 0, att.width, att.height);
	
	if (_needToDraw)
	//if (true)
	{
		_needToDraw = false;
	
		// Clear screen and depth buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glViewport(0, 0, _size.x, _size.y);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, _size.x, _size.y, 0.0);
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

		_windowManager->OnPaint();
		return true;
	}
	return false;
}
