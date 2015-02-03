//
//  main.h
//  ChatClient
//
//  Created by Alexandre Arsenault on 2015-01-30.
//  Copyright (c) 2015 Alexandre Arsenault. All rights reserved.
//

#ifndef __ChatClient__main__
#define __ChatClient__main__

#include <boost/asio.hpp>
//#include "axLib.h"
#include <thread>

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client
{
public:
    chat_client(boost::asio::io_service& io_service,
                tcp::resolver::iterator endpoint_iterator);
    
    void write(const chat_message& msg);
    
    void close();
    
    typedef std::function<void(boost::system::error_code ec, std::size_t)>
    CallbackFunction;
    
private:
    void do_connect(tcp::resolver::iterator endpoint_iterator);
    
    void do_read_header();
    
    void do_read_body();
    
    void do_write();
    
private:
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};
//
//
//class ChatGUI : public axPanel
//{
//public:
//    ChatGUI();
//    
//    
//private:
//    std::thread* _io_thread;
//    chat_client* _chatClient;
//    boost::asio::io_service* _io_service;
//    
//    virtual void OnPaint()
//    {
//        axGC* gc = GetGC();
//        axRect rect(GetDrawingRect());
//        
//        gc->SetColor(axColor(1.0, 0.0, 1.0, 1.0));
//        gc->DrawRectangle(rect);
//    }
//};

#endif /* defined(__ChatClient__main__) */
