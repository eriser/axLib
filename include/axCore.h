/// @defgroup Core 
/// @{
#ifndef __AX_CORE__
#define __AX_CORE__


#include "axC++.h"
#include "axManager.h"

class axCore
{
public:
	axCore();
	virtual void MainLoop() = 0;
	virtual void Init(const axPoint& frame_size) = 0;

	void ResizeGLScene(const int& width, const int& height);
	axManager* GetWindowManager();
	axSize GetGlobalSize() const;
	virtual axRect GetScreenRect() = 0;
	void UpdateAll();


	int InitGL();
	int DrawGLScene();
	virtual void KillGLWindow() = 0;
	virtual bool CreateGLWindow(char* title, int width, int height, int bits) = 0;

protected:

	axManager* _windowManager;

	bool _needToDraw;
	axSize _size;
};

//extern axCore* axCORE;

#endif //__AX_CORE__

/// @}