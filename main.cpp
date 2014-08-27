#include "main.h"

MyTest::MyTest(axApp* app, axWindow* parent, const axRect& rect) :
axPanel(0, app, parent, rect)
{
	//axButton* btn = new axButton(app, this, axRect(20, 200, 50, 20),
	//							 axButtonEvents(GetOnClick()),
	//							 axSTANDARD_BUTTON, "", "Test");

	axMenu* menu = new axMenu(app, this, axRect(0, 0, 150, 300));
	axMenuNode* root = new axMenuNode(app, menu, axRect(0, 0, 150, 20));

	menu->AddRootNode(root);

	axMenuNode* n1 = new axMenuNode(root, "Header Files", 
									"C:/Users/alexarse/Desktop/axLib/ressources/axImages/nodes/folder.png");
	root->AddSubNode(n1);

	n1->AddSubNode(new axMenuNode(n1, "Sub Node 1", "C:/Users/alexarse/Desktop/axLib/ressources/axImages/nodes//file.png"));
	n1->AddSubNode(new axMenuNode(n1, "Sub Node 2", "C:/Users/alexarse/Desktop/axLib/ressources/axImages/nodes//file.png"));
	n1->AddSubNode(new axMenuNode(n1, "Sub Node 3", "C:/Users/alexarse/Desktop/axLib/ressources/axImages/nodes//file.png"));

	axMenuNode* n2 = new axMenuNode(root, "Source Files", "C:/Users/alexarse/Desktop/axLib/ressources/axImages/nodes//folder.png");
	root->AddSubNode(n2);
}

void MyTest::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.4, 0.4, 0.4), 1.0);
	gc->DrawRectangle(rect0);

	gc->SetColor(axColor(0.1, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(axRect(0, 1, rect0.size.x -1 , rect0.size.y - 1));
}

Desktop::Desktop(axApp* app, axWindow* parent, const axRect& rect):
		 axPanel(app, parent, rect)
{

	//axImage* img = new axImage("C:/Users/alexarse/Desktop/axLib/ressources/axImages/nodes/folder.png");
	//img = new axImage("C:/Users/alexarse/Desktop/axLib/ressources/axImages/nodes/file.png");
	openfile = new axButton(app, this, 
							axButtonEvents(GetOnOpenFile()),
							"axButtonFileOpen.axobj");

	openfile = new axButton(app, this, 
							axButtonEvents(GetOnOpenWindow()), 
							"axButtonWindowOpen.axobj");

	axButton* btn = new axButton(app, this,
								 axButtonEvents(GetOnShowAppDirectory()),
								 "axBtn.axobj");
}

void Desktop::OnClosePopupWindow(const int& msg)
{
	/*HWND win = static_cast<axCoreWin32*>(GetApp()->GetCore())->GetPopupWindowHandle();
	SendMessage(win, WM_QUIT, 0, 0);*/
}

void Desktop::OnOpenWindow(const axButtonMsg& msg)
{
	//GetApp()->CreatePopupWindow("test", 150, 300);

	//MyTest* test = new MyTest(GetApp(), nullptr, axRect(0, 0, 150, 300));
	//test->SetCloseCallback(GetOnClosePopupWindow());



	//axPoint pos = static_cast<axCoreWin32*>(GetApp()->GetCore())->GetScreenRelativePosition();
	//cout << "POsition : " << pos.x << " " << pos.y << endl;
	//pos.x += msg.GetSender()->GetAbsoluteRect().position.x;
	//pos.y += msg.GetSender()->GetAbsoluteRect().position.y;
	//pos.y += msg.GetSender()->GetSize().y;
	//cout << "POsition : " << pos.x << " " << pos.y << endl;
	////cout << msg.GetSender()->GetAbsoluteRect().position.x << endl;
	//MoveWindow(static_cast<axCoreWin32*>(GetApp()->GetCore())->GetPopupWindowHandle(),
	//			pos.x, //msg.GetSender()->GetAbsoluteRect().position.x,
	//			pos.y + 30, //msg.GetSender()->GetAbsoluteRect().position.y + msg.GetSender()->GetAbsoluteRect().size.y,
	//			150, //msg.GetSender()->GetAbsoluteRect().size.x,
	//			300, //msg.GetSender()->GetAbsoluteRect().size.y,*/
	//			true
	//			);


}

void Desktop::OnShowAppDirectory(const axButtonMsg& msg)
{
	//cout << GetApp()->GetAppDirectory() << endl;

	//HWND win = static_cast<axCoreWin32*>(GetApp()->GetCore())->GetPopupWindowHandle();
	//SendMessage(win, WM_QUIT, 0, 0);
	//GetApp()->GetCore()->KillPopGLWindow();
}

void Desktop::OnOpenFile(const axButtonMsg& msg)
{
	//cout << "Click" << endl;
	//string filename(GetApp()->OpenFileDialog());
	//if (!filename.empty())
	//{
	//	cout << filename << endl;
	//}
}

void Desktop::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(0.4, 0.4, 0.4), 1.0);
	gc->DrawRectangle(rect0);
}

int main()
{
	axApp app(axSize(500, 80));
	Desktop* dekstop = new Desktop(&app, nullptr, axRect(0, 0, 500, 80));

	app.MainLoop();

	system("pause");
	return 0;
}