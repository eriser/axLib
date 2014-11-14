#include "main.h"

Desktop::Desktop(axApp* app, axWindow* parent, const axRect& rect):
			axPanel(app, parent, rect)
{
	axMenu* menu = new axMenu(app, this, axRect(10, 50, 151, 300));
	axMenuNode* root = new axMenuNode(app, menu, axRect(0, 0, 151, 20));

	menu->AddRootNode(root);
	 
	axMenuNode* n1 = new axMenuNode(root, "Header Files", "nodes/folder.png");
	root->AddSubNode( n1 );

	n1->AddSubNode( new axMenuNode(n1, "Sub Node 1", "nodes/file.png"));
	n1->AddSubNode( new axMenuNode(n1, "Sub Node 2", "nodes/file.png"));
	n1->AddSubNode( new axMenuNode(n1, "Sub Node 3", "nodes/file.png"));

	axMenuNode* n2 = new axMenuNode(root, "Source Files", "nodes/folder.png");
	root->AddSubNode(n2);
}

void Desktop::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(1.0, 1.0, 1.0), 1.0);
	gc->DrawRectangle(rect0);

	gc->SetColor(axColor(1.0, 0.0, 0.0), 1.0);
	gc->DrawLine(axPoint(10, 40), axPoint(10 + 150, 40));
}

int main()
{
	axApp app(axSize(500, 500));
	Desktop* desktop = new Desktop(&app, nullptr, 
								   axRect(0, 0, 500, 500));

	//axMenu* menu = new axMenu();
	app.MainLoop();

	return 0;
}
