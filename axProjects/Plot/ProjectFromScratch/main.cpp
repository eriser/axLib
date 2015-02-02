#include "main.h"

/*******************************************************************************
 * MyProject.
 ******************************************************************************/
MyProject::MyProject(axWindow* parent, const axRect& rect):
axPanel(parent, rect)
{
    axTextBox::Events txtEvents;
    txtEvents.enter_click = GetOnEnterDown();
    
    axTextBox::Info txtInfo;
    txtInfo.normal = axColor(0.9, 0.9, 0.9);
    txtInfo.hover = axColor(0.9, 0.9, 0.9);
    txtInfo.selected = axColor(1.0, 1.0, 1.0);
    txtInfo.highlight = axColor(0.4, 0.4, 0.6, 0.4);
    txtInfo.contour = axColor(0.0, 0.0, 0.0);
    txtInfo.cursor = axColor(1.0, 0.0, 0.0);
    txtInfo.selected_shadow = axColor(0.8, 0.8, 0.8, 0.3);
    
    _textBox = new axTextBox(this, axRect(10, 10, 200, 25),
                             txtEvents, txtInfo, "",
                             "sin(x)");
    
    axButton* gen = new axButton(this, axRect(_textBox->GetNextPosRight(5),
                                              axSize(60, 25)),
                                 axButton::Events(GetOnGenerate()),
                                 axSTANDARD_BUTTON, "",
                                 "Generate");
    
    
    axPlotEvents plotEvents;
    
    axPlotInfo plotInfo;
    plotInfo.background_color = axColor(1.0, 1.0, 1.0);
    plotInfo.grid_dark_color = axColor(0.0, 0.0, 0.0);
    plotInfo.grid_color = axColor(0.8, 0.8, 0.8);
    plotInfo.contour_color = axColor(0.0, 0.0, 0.0);
    plotInfo.function_color = axColor(1.0, 0.0, 0.0);
    
    std::vector<double> xGrid{-6, -5, -4, -3, -2, -1, 1, 2, 3, 4, 5, 6};
    std::vector<double> xGridDark{0};
    
    std::vector<double> yGrid{-0.8, -0.6, -0.4, -0.2, 0.2, 0.4, 0.6, 0.8};
    std::vector<double> yGridDark{0};
    
    _plot = new axPlot(this, axRect(10, 45, rect.size.x - 20, rect.size.y - 55),
                              plotEvents, plotInfo,
                              std::string("sin(x)"),
                              axFloatRange(-2.0 * M_PI, 2.0 * M_PI),
                              axFloatRange(-1.0, 1.0),
                              xGrid, xGridDark, yGrid, yGridDark);
    
    std::vector<axFloatPoint> points = { axFloatPoint(0.1, 0.1),
                                         axFloatPoint(0.2, 0.2),
                                         axFloatPoint(0.3, 0.3),
                                         axFloatPoint(0.4, 0.4),
                                         axFloatPoint(0.5, 0.5),
                                         axFloatPoint(0.6, 0.6),
                                         axFloatPoint(0.7, 0.7),
                                         axFloatPoint(0.8, 0.8),
                                         axFloatPoint(0.9, 0.9)};
    _plot->SetPoints(points);
}

void MyProject::OnGenerate(const axButton::Msg& msg)
{
    unsigned char* data = nullptr;
    axRect imgRect = _plot->GetWindowPixelData(data);

    axImage img(data, imgRect.size);
    std::string img_path("/Users/alexarse/Desktop/test.png");

    if(!img.SaveImage(img_path))
    {
        std::cout << "GenerateImage didn't work." << std::endl;
    }
    
    delete[] data;
}

void MyProject::OnEnterDown(const axTextBox::Msg& msg)
{
    _plot->SetExpression(msg.GetMsg());
}

void MyProject::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect0(axPoint(0, 0), GetRect().size);
    
    gc->SetColor(axColor(0.4, 0.4, 0.4), 1.0);
    gc->DrawRectangle(rect0);
    

    gc->SetColor(axColor(0.0, 0.0, 0.0), 1.0);
    gc->DrawRectangleContour(rect0);

}

void axMain::MainEntryPoint(axApp* app)
{
    MyProject* myProject = new MyProject(nullptr, axRect(0, 0, 500, 500));
}


