//
//  PaintPanel.cpp
//  Demo
//
//  Created by Alexandre Arsenault on 2014-11-03.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "PaintPanel.h"

PaintPanel::PaintPanel(axApp* app,
                     axWindow* parent,
                     const axRect& rect):
// Parent.
axPanel(app, parent, rect)
{
    //std::string app_path("/Users/alexarse/Project/axLib/axExamples/Demo/");
	std::string app_path("C:/Users/Alexandre Arsenault/Desktop/axLib/axExamples/Demo/VisualStudio2013/axGL/");
    dog_cat_img = new axImage(app_path + std::string("whiskers-wags-dog-cat.png"));
}

void PaintPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.4, 0.4, 0.4), 1.0);
    gc->DrawRectangle(rect0);
    
    gc->SetColor(axColor(0.9, 0.4, 0.4), 1.0);
    gc->DrawRectangle(axRect(40, 40, 50, 50));
    
    
    gc->SetColor(axColor(0.9, 0.4, 0.9), 1.0);
    gc->DrawRectangle(axRect(100, 40, 50, 50));
    gc->SetColor(axColor(0.0, 1.0, 0.0), 1.0);
    gc->DrawRectangleContour(axRect(100, 40, 50, 50));
    

    gc->SetColor(axColor(0.9, 0.9, 0.4), 1.0);
    gc->DrawRectangleContour(axRect(160, 40, 50, 50));
    
    
    gc->DrawRectangleColorFade(axRect(220, 40, 50, 50),
                               axColor(0.3, 0.8, 0.2),
                               1.0,
                               axColor(0.3, 0.8, 0.8),
                               1.0);
    
    gc->SetLineWidth(0.5);
    gc->SetColor(axColor(0.0, 0.0, 1.0), 1.0);
    gc->DrawCircle(axPoint(305, 65), 25.0, 3);
    
    gc->SetLineWidth(1.0);
    gc->SetColor(axColor(1.0, 0.0, 1.0), 1.0);
    gc->DrawCircle(axPoint(365, 65), 25.0, 4);
    
    gc->SetLineWidth(2.0);
    gc->SetColor(axColor(0.0, 1.0, 1.0), 1.0);
    gc->DrawCircle(axPoint(425, 65), 25.0, 5);

    gc->SetLineWidth(3.0);
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawCircle(axPoint(485, 65), 25.0, 40);
    
    gc->DrawImage(dog_cat_img, axPoint(40, 100));
    gc->DrawImageResize(dog_cat_img,
                        axPoint(250, 100),
                        axSize(90, 70),
                        1.0);
    
    gc->DrawImageResize(dog_cat_img,
                        axPoint(340, 100),
                        axSize(90, 70),
                        0.35);
    
    gc->DrawPartOfImage(dog_cat_img,
                        axPoint(0, 0),
                        axSize(60, 45),
                        axPoint(430, 100));
    
    gc->SetLineWidth(3.0);
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    
    int y1 = 350, y2 = 250;
    double w = 1.0;
    for(int i = 0; i < 6; i++, y1 += 10, y2 += 10, w += 1.0)
    {
        gc->DrawLine(axPoint(40, y1), axPoint(300, y2), w);
    }
    
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->SetFontSize(12);
    gc->DrawString("Default 12", axPoint(350, 190));
    
    gc->SetFontSize(14);
    gc->DrawString("Default 14", axPoint(350, 205));
    
    gc->SetColor(axColor(0.0, 1.0, 0.0), 1.0);
    gc->SetFontSize(20);
    gc->DrawString("Default 20", axPoint(350, 220));

    
    gc->SeDefaultLine();
    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    
    gc->DrawRoundedRectangle(axRect(350, 350, 50, 20));
    gc->DrawRectangleContour(rect0);
    
    
    
    

}