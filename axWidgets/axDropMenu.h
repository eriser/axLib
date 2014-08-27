///// @defgroup Widgets 
///// @{
//
//#ifndef __AX_DROP_MENU__
//#define __AX_DROP_MENU__
//
//#include "axEvent.h"
//#include "axPanel.h"
//#include "axColor.h"
//#include "axGC.h"
//#include "axImage.h"
//#include <fstream>
//
///**************************************************************************//**
// * axButtonFlags.
//******************************************************************************/
//#define axBUTTON_SINGLE_IMG	axFLAG_1
//#define axBUTTON_IMG_RESIZE	axFLAG_2
//
//class axDropMenuMsg;
//
//class axDropMenuMsg
//{
//public:
//	axDropMenuMsg()
//	{
//		_sender = nullptr;
//	}
//
//	axDropMenuMsg(axDropMenu* sender, const string& msg)
//	{
//		_sender = sender;
//		_msg = msg;
//	}
//
//	axDropMenu* GetSender() const
//	{
//		return _sender;
//	}
//
//	string GetMsg() const
//	{
//		return _msg;
//	}
//
//private:
//	axDropMenu* _sender;
//	string _msg;
//};
////
//struct axDropMenuEvents
//{
//	//axEvtFunction(axButtonMsg) button_click;
//	axEvtFunction(axDropMenuMsg) menu_click;
//	
//	axDropMenuEvents(){}
//	axDropMenuEvents(axEvtFunction(axDropMenuMsg) fct)
//	{ 
//		menu_click = fct; 
//	}
//};
////
//struct axDropMenuInfo
//{
//	axColor background;
//	axColor normal;
//	axColor hover;
//	axColor clicking;
//	axColor selected;
//	axColor contour;
//	axColor font_color;
//
//	axDropMenuInfo(){}
//	axDropMenuInfo(
//		const axColor& normal_color,
//		const axColor& background_color,
//		const axColor& hover_color,
//		const axColor& clicked_color,
//		const axColor& selected_color,
//		const axColor& contour_color,
//		const axColor& font_color_) :
//		normal(normal_color),
//		background(background_color),
//		hover(hover_color),
//		clicking(clicked_color),
//		selected(selected_color),
//		contour(contour_color),
//		font_color(font_color_){}
//
//};
//
//#define axSTANDARD_DROP_MENU 	axDropMenuInfo( \
//								axColor(0.5, 0.5, 0.5), \
//								axColor(0.2, 0.2, 0.2), \
//								axColor(0.6, 0.6, 0.6), \
//								axColor(0.4, 0.4, 0.4), \
//								axColor(0.5, 0.5, 0.5), \
//								axColor(0.0, 0.0, 0.0), \
//								axColor(0.0, 0.0, 0.0))
////
////
//class axDropMenu : public axPanel
//{
//public:
//	axDropMenu(axApp* app,
//		axWindow* parent,
//		const axRect& rect,
//		const axDropMenuEvents& events,
//		const axDropMenuInfo& info);
//
//	//void SetBackgroundAlpha(const float& alpha);
//
//	//void SetMsg(const string& msg);
//	//void SetSelected(const bool& selected);
//
//private:
//	axDropMenuEvents _events;
//	axDropMenuInfo _info;
//};
//
//#endif //__AX_DROP_MENU__
//
///// @}
