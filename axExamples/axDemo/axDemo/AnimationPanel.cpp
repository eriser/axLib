//
//  AnimationPanel.cpp
//  axDemo
//
//  Created by Alexandre Arsenault on 2014-12-10.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//

#include "AnimationPanel.h"
#include <random>

AnimationPanel::AnimationPanel(axWindow* parent,
                               const axRect& rect):
// Parent.
axPanel(parent, rect),
_radius(20.0)
{
    std::string app_path(axApp::GetInstance()->GetAppDirectory());
    
    _nParticules = 40;
    
    _animationTimer = new axTimer();
    _animationTimer->AddConnection(0, GetOnTimerAnimation());
    
    _clickTimer = new axTimer();
    _clickTimer->AddConnection(0, GetOnClickAnimation());
    
    _releaseTimer = new axTimer();
    _releaseTimer->AddConnection(0, GetOnReleaseAnimation());
}

void AnimationPanel::OnTimerAnimation(const axTimerMsg& msg)
{
    Update();
}

void AnimationPanel::OnClickAnimation(const axTimerMsg& msg)
{
    _radius += 3.0;
    _radius = axClamp<double>(_radius, 10.0, 100.0);
    
    _nParticules += 20;
    _nParticules = axClamp<int>(_nParticules, 40, 1000);
}

void AnimationPanel::OnReleaseAnimation(const axTimerMsg& msg)
{
    _radius -= 4.0;
    _radius = axClamp<double>(_radius, 20.0, 100.0);
    
    _nParticules -= 40;
    _nParticules = axClamp<int>(_nParticules, 40, 1000);
}

void AnimationPanel::OnMouseLeftDown(const axPoint& pos)
{
    _clickTimer->StartTimer(20, 2000);
}

void AnimationPanel::OnMouseLeftUp(const axPoint& pos)
{
    _releaseTimer->StartTimer(20, 2000);
}

void AnimationPanel::OnMouseEnter()
{
    _animationTimer->StopTimer();
   _animationTimer->StartTimer(50);
}

void AnimationPanel::OnMouseLeave()
{
    _animationTimer->StopTimer();
    Update();
}

void AnimationPanel::OnMouseMotion(const axPoint& pos)
{
    _mousePosition = pos - GetAbsoluteRect().position;
}

void AnimationPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.8, 0.4, 0.4), 1.0);
    gc->DrawRectangle(rect0);
    
    //----------
    if(_animationTimer->IsRunning())
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> dis(0, _radius);
        std::uniform_real_distribution<> unif_dis(0.5, 5.0);
        std::uniform_real_distribution<> color_dis(0.0, 1.0);
        
        for(int i = 0; i < _nParticules; i++)
        {
            gc->SetColor(axColor(color_dis(gen),
                                 color_dis(gen),
                                 color_dis(gen),
                                 color_dis(gen)));
            
            axPoint pt = _mousePosition + axPoint(dis(gen), dis(gen));
            gc->DrawCircle(pt, unif_dis(gen), 50);
        }
    }

    
    gc->SetColor(axColor(0.0, 0.0, 0.0));
    gc->DrawRectangleContour(rect0);
}
