/// @defgroup Core 
/// @{
#ifndef __AX_CORE__
#define __AX_CORE__


#include "axC++.h"
#include "axManager.h"
#include <time.h>

class axCore
{
public:
	axCore();
	virtual void MainLoop() = 0;
	virtual void Init(const axPoint& frame_size) = 0;

	void ResizeGLScene(const int& width, const int& height, double y = 0);
	axManager* GetWindowManager();
	axManager* GetPopupManager();
	virtual string GetAppDirectory() = 0;

    
	virtual bool CreatePopupWindow(char* title, int width, int height) = 0;

	axSize GetGlobalSize() const;
    
    void SetGlobalSize(const axSize& size);
	virtual axRect GetScreenRect() = 0;
	virtual void UpdateAll();

	virtual string OpenFileDialog() = 0;

	void DeletePopWindow();
    
    virtual void PushEventOnSystemQueue() = 0;


	int InitGL();
	void ResizePopGLScene(const int& width, const int& height);
	virtual int DrawGLScene();
	virtual int DrawGLPopScene();
	virtual void KillGLWindow() = 0;
	virtual void KillPopGLWindow() = 0;
	virtual bool CreateGLWindow(char* title, int width, int height, int bits) = 0;
    
    double GetYTestValue()
    {
        return _y_test;
    }

protected:

	axManager* _windowManager;
	axManager* _popupManager;

	bool _needToDraw, _popupNeedToDraw;
	axSize _size, _popSize;
	//clock_t _lastDrawingTime;
    
    double _y_test;
};

//extern axCore* axCORE;

#endif //__AX_CORE__

/// @}