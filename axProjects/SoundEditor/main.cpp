#include "main.h"

SoundEditor::SoundEditor(axApp* app, axWindow* parent,
									 const axRect& rect):
								     axPanel(app, parent, rect)
{

}

void SoundEditor::OnPaint()
{
	axGC* gc = GetGC();
	axRect rect(GetRect());
	axRect rect0(axPoint(0, 0), rect.size);

	gc->SetColor(axColor(1.0, 1.0, 1.0), 1.0);
	gc->DrawRectangle(rect0);

    gc->SetColor(axColor(0.7, 0.3, 0.3), 1.0);
    gc->DrawRectangleContour(rect0.GetInteriorRect(axPoint(10, 10)));

	gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
	gc->DrawRectangleContour(rect0);
}


