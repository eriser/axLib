//
//  main.cpp
//  axManager
//
//  Created by Alexandre Arsenault on 2014-12-05.
//  Copyright (c) 2014 Alexandre Arsenault. All rights reserved.
//
#include <iostream>
#include "main.h"
#include "axEvent.h"
#include "axObject.h"
#include "axEventManager.h"
#include "axTimer.h"

class MyClassMsg : public axMsg
{
public:
    MyClassMsg(double msg)
    {
        message = msg;
    }
    
    double message;
    
    virtual axMsg* GetCopy()
    {
        return new MyClassMsg(*this);
    }
};

class MyClass : public axObject
{
public:
    MyClass(double g)
    {
    }
    
    axEventId MyFctId{0};
    axEventId MyFct2Id{1};
    
    enum : axEventId { EVT_FCT_1 = 0, EVT_FCT_2 = 1};
    
    axEVENT(MyClassMsg, MyFct);
    axEVENT(MyClassMsg, MyFct2);
    axEVENT(axTimerMsg, MyTimerFct);
    
private:
    void MyFct(const MyClassMsg& msg)
    {
        std::cout << "MyFct : " << msg.message << std::endl;
    }
    
    void MyFct2(const MyClassMsg& msg)
    {
        std::cout << "MyFct2" << std::endl;
    }
    
    void MyTimerFct(const axTimerMsg& msg)
    {
        std::cout << "Timer : " << msg.GetTime() << std::endl;
    }
};


int main()
{
    std::cout << "Begin." << std::endl;
    axEventManager* evtManager = axEventManager::GetInstance();
    
    MyClass a(2.2), b(3.3), c(4.4);
    
    std::cout << "Connection." << std::endl;
    a.AddConnection(MyClass::EVT_FCT_1, a.GetMyFct());
    a.AddConnection(MyClass::EVT_FCT_2, a.GetMyFct2());
    
    std::cout << "Push Event." << std::endl;
    evtManager->PushEvent(a.GetId(),
                          MyClass::EVT_FCT_1,
                          new MyClassMsg(999));
    
    a.PushEvent(MyClass::EVT_FCT_1, new MyClassMsg(28.78));
    a.PushEvent(MyClass::EVT_FCT_2, new MyClassMsg(24.78));
    
    axTimer timer1(200);
    timer1.AddConnection(0, a.GetMyTimerFct());
    
    std::cout << "Event Manager Call Next." << std::endl;
    
    while(1)
    {
        evtManager->CallNext();
    }
    
    return 0;
}
