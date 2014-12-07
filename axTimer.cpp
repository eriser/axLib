#include "axTimer.h"
//#include "axCocoaInterfaceMac.h"


struct axTimerPointerMsg
{
    axEventManager* event_manager;
    axTimer* timer;
    int ms;
};

axTimer::axTimer(axEventFunction fct, int ms)
{
//    AddConnection(0, fct);
    InitTimer(ms);
}

axTimer::axTimer(int ms)
{
    InitTimer(ms);
}

void axTimer::InitTimer(int ms)
{
    _ms_since_timer_start = 0;
    
    _timerThread = std::thread(timer_thread,
                               std::ref(*this),
                               ms);
//    try
    {
//        axTimerPointerMsg* msg = new axTimerPointerMsg();
//        msg->event_manager = axEventManager::GetInstance();
//        msg->timer = this;
//        msg->ms = ms;
//
//        _timerThread = std::thread(timer_thread);
//        int iret1 = pthread_create(&_timerThread,
//                                   NULL,
//                                   timer_thread,
//                                   (void*)msg);
//   
//        if(iret1)
//        {
//            std::cerr << "Error - pthread_create() return code: " << iret1 << std::endl;
////            fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
////            exit(EXIT_FAILURE);
//        }

    }
//    catch (std::error_code msg)
//    {
//        std::cout << msg.message() << std::endl;
//    }
    
}

//void* axTimer::timer_thread(void* ptr)
//{
//    axTimerPointerMsg* msg = static_cast<axTimerPointerMsg*>(ptr);
//    
//    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//    while (1)
//    {
//        std::cout << "Thread loop : " << msg->event_manager->GetEventQueueSize() << std::endl;
//        axMsg* timer_msg = new axTimerMsg(msg->timer->_ms_since_timer_start);
//        msg->timer->_ms_since_timer_start += msg->ms;
//        msg->timer->PushEvent(0, timer_msg);
//        
//        std::this_thread::sleep_for(std::chrono::milliseconds(msg->ms));
//    }
//    return nullptr;
//}

void axTimer::timer_thread(axTimer& timer, int ms)
{
    int count_ms = 5000;
    while (count_ms > 0)
    {
        axMsg* msg = new axTimerMsg(timer._ms_since_timer_start);
        timer._ms_since_timer_start += ms;
        timer.PushEvent(0, msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        count_ms -= ms;
        
        
//        CocoaGetAppDirectory();
        
    }
}

//axEventManager& evtManager,
//                           //axTimer& timer,
//                           //int ms)
//{
////    while (1)
////    {
////        axMsg* msg = new axTimerMsg(timer._ms_since_timer_start);
////        timer._ms_since_timer_start += ms;
////        timer.PushEvent(0, msg);
////        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
////    }
//}