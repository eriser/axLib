#ifndef __AX_CORE_WX_WIDGETS__
#define __AX_CORE_WX_WIDGETS__

#include "axConfig.h"

#if _axWxWidgetsCore_ == 1
#include "axCore.h"
//glew32.lib; 
//libpng16.lib; 
//zlib.lib; freetype253_D.lib; 
//libvlc.lib; irrlicht.lib; 
//wxbase30ud.lib; 
//wxbase30ud_net.lib; 
//wxbase30ud_xml.lib; 
//wxexpatd.lib; 
//wxjpegd.lib; 
//wxmsw30ud_adv.lib; 
//wxmsw30ud_aui.lib; 
//wxmsw30ud_core.lib; 
//wxmsw30ud_gl.lib; 
//wxmsw30ud_html.lib; 
//wxmsw30ud_media.lib;
//wxmsw30ud_propgrid.lib; 
//wxmsw30ud_qa.lib; 
//wxmsw30ud_ribbon.lib; 
//wxmsw30ud_richtext.lib;
//wxmsw30ud_stc.lib; 
//wxmsw30ud_webview.lib;
//wxmsw30ud_xrc.lib; 
//wxpngd.lib; 
//wxregexud.lib;
//wxscintillad.lib; 
//wxtiffd.lib; 
//wxzlibd.lib; 


#pragma comment( lib, "wxbase30ud.lib" )
#pragma comment( lib, "wxmsw30ud_core.lib" )
#pragma comment( lib, "wxmsw30ud_gl.lib" )

class wxGLCanvas;

class axCoreWxWidgets : public axCore
{
public:
	axCoreWxWidgets();

	virtual void Init(const axSize& frame_size);

	virtual bool CreatePopupWindow(const char* title, int width, int height)
	{
		(title);
		(width);
		(height);

		return false;
	}

	void SetWxGlCanvas(wxGLCanvas* canvas)
	{
		_wxGLCanvas = canvas;
	}

	virtual string OpenFileDialog()
	{
		return "";
	}

	virtual void PushEventOnSystemQueue();

	axPoint GetScreenRelativePosition()
	{

	}

	virtual void ResizeFrame(const axSize& size)
	{
		(size);
	}
	
	virtual void UpdateAll();

	virtual axSize GetScreenSize()
	{
		return axSize(1000, 1000);
	}

	virtual axRect GetScreenRect()
	{
		return axRect(0, 0, 1000, 1000);
	}

	virtual int DrawGLPopScene()
	{
		return 0;
	}

	//virtual int DrawGLScene()
	//{
	//	return 0;
	//}

	std::string GetAppDirectory()
	{
		return "";
	}
	void DeletePopWindow()
	{

	}

protected:
	virtual void KillGLWindow()
	{

	}
	virtual void KillPopGLWindow()
	{

	}
	virtual bool CreateGLWindow(const char* title, int width, int height, int bits = 16)
	{
		(title);
		(width);
		(height);
		(bits);
		return false;
	}
	virtual void MainLoop()
	{

	}

	wxGLCanvas* _wxGLCanvas;

};

#endif //_axWxWidgetsCore_.
#endif //__AX_CORE_WX_WIDGETS__