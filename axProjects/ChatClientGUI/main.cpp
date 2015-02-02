//
// chat_client.cpp
// ~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2014 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <functional>

#include "axChatMessage.h"
#include "main.h"
//#include "axMain.h"

chat_client::chat_client(boost::asio::io_service& io_service,
            tcp::resolver::iterator endpoint_iterator)
: io_service_(io_service),
socket_(io_service)
{
    do_connect(endpoint_iterator);
}

void chat_client::write(const chat_message& msg)
{
    io_service_.post(
                     [this, msg]()
                     {
                         bool write_in_progress = !write_msgs_.empty();
                         write_msgs_.push_back(msg);
                         if (!write_in_progress)
                         {
                             do_write();
                         }
                     });
}

void chat_client::close()
{
    io_service_.post([this]() { socket_.close(); });
}

void chat_client::do_connect(tcp::resolver::iterator endpoint_iterator)
{
    boost::asio::async_connect(socket_, endpoint_iterator,
                               [this](boost::system::error_code ec,
                                      tcp::resolver::iterator)
                               {
                                   if (!ec)
                                   {
                                       do_read_header();
                                   }
                               });
}

void chat_client::do_read_header()
{
    // Callback function.
    CallbackFunction fct = [this](boost::system::error_code ec, std::size_t)
    {
        if (!ec && read_msg_.decode_header())
        {
            do_read_body();
        }
        else
        {
            socket_.close();
        }
    };
    
    boost::asio::async_read(socket_,
                            boost::asio::buffer(read_msg_.data(),
                                                chat_message::header_length),
                            fct);
}

void chat_client::do_read_body()
{
    // Callback function.
    CallbackFunction fct = [this](boost::system::error_code ec, std::size_t)
    {
        if (!ec)
        {
            std::cout << "Read body." << std::endl;
            std::cout.write(read_msg_.body(), read_msg_.body_length());
            std::cout << "\n";
            do_read_header();
        }
        else
        {
            socket_.close();
        }
    };
    
    // Read message.
    boost::asio::async_read(socket_,
                            boost::asio::buffer(read_msg_.body(),
                                                read_msg_.body_length()),
                            fct);
}

void chat_client::do_write()
{
    // Callback function.
    CallbackFunction fct = [this](boost::system::error_code ec, std::size_t)
    {
        if (!ec)
        {
            write_msgs_.pop_front();
            if (!write_msgs_.empty())
            {
                do_write();
            }
        }
        else
        {
            socket_.close();
        }
    };
    
    
    boost::asio::async_write(socket_,
                             boost::asio::buffer(write_msgs_.front().data(),
                                                 write_msgs_.front().length()),
                             fct);
}
//
//void axMain::MainEntryPoint(axApp* app)
//{
//    ChatGUI* chat_gui = new ChatGUI();
//}
//
//
//ChatGUI::ChatGUI():
//axPanel(nullptr, axRect(0, 0, 300, 300))
//{
//    std::string server_ip_adress = "localhost";
//    std::string server_port = "82";
//    
//    try
//    {
//        _io_service = new boost::asio::io_service();
////        boost::asio::io_service io_service;
//        //
//        tcp::resolver resolver(*_io_service);
//        auto endpoint_iterator = resolver.resolve({server_ip_adress.c_str(),
//                                                  server_port.c_str()});
//        
//        _chatClient = new chat_client(*_io_service, endpoint_iterator);
//    
//        _io_thread = new std::thread([this]()
//        {
//            std::cout << "Run" << std::endl;
//            _io_service->run();
//        });
//        
//
//        std::cout << "Try sending msg." << std::endl;
//        std::string my_msg = "Test msg.";
//        chat_message msg;
//        msg.body_length(my_msg.size());
//        std::memcpy(msg.body(), my_msg.c_str(), msg.body_length());
//        msg.encode_header();
//        _chatClient->write(msg);
////        _chatClient->write(msg);
//        
////        std::thread t([&io_service](){ io_service.run(); });
//    
////                char line[chat_message::max_body_length + 1];
////    
////                while (std::cin.getline(line, chat_message::max_body_length + 1))
////                {
////                    chat_message msg;
////                    msg.body_length(std::strlen(line));
////                    std::memcpy(msg.body(), line, msg.body_length());
////                    msg.encode_header();
////                    _chatClient->write(msg);
////                }
//        //
//        //        chatClient.close();
//        //        t.join();
//    }
//    catch (std::exception& e)
//    {
//        std::cerr << "Exception: " << e.what() << "\n";
//    }
//}
//


//int main(int argc, char* argv[])
//{
//    try
//    {
//        if (argc != 3)
//        {
//            std::cerr << "Usage: chat_client <host> <port>\n";
//            return 1;
//        }
//        
//        boost::asio::io_service io_service;
//        
//        tcp::resolver resolver(io_service);
//        auto endpoint_iterator = resolver.resolve({argv[1], argv[2]});
//        chat_client c(io_service, endpoint_iterator);
//        
//        std::thread t([&io_service](){ io_service.run(); });
//        
//        char line[chat_message::max_body_length + 1];
//        
//        while (std::cin.getline(line, chat_message::max_body_length + 1))
//        {
//            chat_message msg;
//            msg.body_length(std::strlen(line));
//            std::memcpy(msg.body(), line, msg.body_length());
//            msg.encode_header();
//            c.write(msg);
//        }
//        
//        c.close();
//        t.join();
//    }
//    catch (std::exception& e)
//    {
//        std::cerr << "Exception: " << e.what() << "\n";
//    }
//    
//    return 0;
//}