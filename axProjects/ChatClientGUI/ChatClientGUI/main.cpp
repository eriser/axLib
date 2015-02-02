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
#include "axMain.h"

#include <sstream>
#include <chrono>
#include <ctime>


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
            stringstream str_stream;
            str_stream.write(read_msg_.body(), read_msg_.body_length());
            
            std::string txt(str_stream.str());
//            std::cout << txt << std::endl;

            if(_evtReceiveMsg)
            {
                PushEvent(0, new ChatMsg(txt));
            }
            
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
void axMain::MainEntryPoint(axApp* app)
{
    ChatGUI* chat_gui = new ChatGUI();
}


//******************************************************************************
// MyScrollPanel.
//******************************************************************************
MyScrollPanel::MyScrollPanel(axWindow* parent,
                             const axRect& rect,
                             const axSize& seenSize):
// Parent.
axPanel(parent, rect)
{
    SetBlockDrawing(true);
    SetShownRect(axRect(axPoint(0, 0), seenSize));
    
    _font = new axFont(0);
    
    axScrollBarEvents scrollEvents;
    axScrollBarInfo scroll_info;
    scroll_info.normal = axColor(0.8, 0.8, 0.8);
    scroll_info.hover = axColor(0.9, 0.9, 0.9);
    scroll_info.clicking = axColor(0.7, 0.7, 0.7);
    scroll_info.contour = axColor(0.0, 0.0, 0.0);
    scroll_info.selected = scroll_info.normal;
    
    axPoint pos(rect.position.x + rect.size.x, rect.position.y);
    axScrollBar* _scrollBar = new axScrollBar(parent,
                                              this,
                                              axRect(pos,
                                                     axSize(8, GetShownRect().size.y + 1)),
                                              scrollEvents,
                                              scroll_info);
    
    _scrollBar->SetPanelSize(rect.size);
}

void MyScrollPanel::OnBtn(const axButton::Msg& msg)
{
    std::cout << "Btn" << std::endl;
}

void MyScrollPanel::AddLine(const std::string& line)
{
    _strVector.push_back(line);
    Update();
}

void MyScrollPanel::ClearLines()
{
    _strVector.clear();
    Update();
}

void MyScrollPanel::OnMouseLeftDown(const axPoint& pos)
{
    GetParent()->OnMouseLeftDown(pos - GetAbsoluteRect().position);
}

void MyScrollPanel::OnPaintStatic()
{
    axGC* gc = GetGC();
    axRect rect(GetShownRect());
    //axRect rect0(axPoint(0, 0), rect.size);
    
    gc->SetColor(axColor(0.0, 0.0, 0.0, 1.0));
    gc->DrawRectangleContour(rect);
}

void MyScrollPanel::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetRect());
    axRect rect0(axPoint(0, 0), rect.size);

    gc->DrawRectangleColorFade(rect0,
                               axColor(1.0, 1.0, 1.0),
                               axColor(0.95, 0.95, 0.95));
    axPoint pos(5, 0);
    int i = 0;
    
    for(auto& n : _strVector)
    {
        if(i % 2 == 0)
        {
            gc->SetColor(axColor(0.8, 0.8, 0.8, 0.4));
        }
        else
        {
            gc->SetColor(axColor(0.9, 0.9, 0.9, 0.2));
        }
        
        gc->DrawRectangle(axRect(0, pos.y, rect.size.x, 15));
        
        axStringVector str_vec(GetVectorFromStringDelimiter(n, "-->"));
                               
        gc->SetColor(axColor(0.0, 0.0, 0.0, 1.0));
        gc->DrawString(*_font, str_vec[0], pos);
        gc->DrawString(*_font, str_vec[1], axPoint(pos.x + 60, pos.y));
        gc->DrawString(*_font, str_vec[2], axPoint(pos.x + 140, pos.y));
//        gc->DrawString(*_font, n, pos);
        pos += axPoint(0, 15);
        i++;
    }
}

//******************************************************************************
// ChatGUI.
//******************************************************************************
ChatGUI::ChatGUI():
axPanel(nullptr, axRect(1, 1, 500, 500))
{
    std::string server_ip_adress = "localhost";
    std::string server_port = "80";
    
    CreateChat(server_ip_adress, server_port);
    
    _font = new axFont(0);
    
    // Scroll panel.
    axSize size(498, 475);
    // height of top bar = 25, height on scrollPanel = 450.
    _scrollPanel = new MyScrollPanel(this, axRect(0, 40, 491, 1000),
                                     axSize(491, 435));
    axTextBox::Events txtEvents;
    txtEvents.enter_click = GetOnTextEnterClick();
    
    axTextBox::Info txtInfo;
    txtInfo.normal = axColor(0.9, 0.9, 0.9);
    txtInfo.hover = axColor(0.9, 0.9, 0.9);
    txtInfo.selected = axColor(1.0, 1.0, 1.0);
    txtInfo.highlight = axColor(0.4, 0.4, 0.6, 0.4);
    txtInfo.contour = axColor(0.0, 0.0, 0.0);
    txtInfo.cursor = axColor(1.0, 0.0, 0.0);
    txtInfo.selected_shadow = axColor(0.8, 0.8, 0.8, 0.3);
    
    _txtCtrl = new axTextBox(this,
                             axRect(0, GetRect().size.y - 25,
                                    GetRect().size.x - 1, 24),
                             txtEvents,
                             txtInfo,
                             "",
                             "",
                             axTextBox::Flags::FLASHING_CURSOR);
    
    axTextBox::Info usernameInfo;
    usernameInfo.normal = axColor(0.9, 0.9, 0.9, 0.0);
    usernameInfo.hover = axColor(0.9, 0.9, 0.9, 0.0);
    usernameInfo.selected = axColor(1.0, 1.0, 1.0, 0.3);
    usernameInfo.highlight = axColor(0.4, 0.4, 0.6, 0.4);
    usernameInfo.contour = axColor(0.0, 0.0, 0.0, 0.0);
    usernameInfo.cursor = axColor(0.3, 0.3, 0.3);
    usernameInfo.selected_shadow = axColor(0.8, 0.8, 0.8, 0.3);
    
    axTextBox::Events usernameEvents;
    usernameEvents.enter_click = GetOnUsernameEnter();
    _usernameCtrl = new axTextBox(this,
                                  axRect(70, 0,
                                         80, 24),
                                  usernameEvents,
                                  usernameInfo,
                                  "",
                                  "user0",
                                  axTextBox::Flags::FLASHING_CURSOR);
    
    _usernameCtrl->SetMaximumChar(10);
    
    axTextBox::Events serverIpEvents;
    serverIpEvents.enter_click = GetOnServerIpAdressEnter();
    _serverIpCtrl = new axTextBox(this,
                                  axRect(210, 0,
                                         120, 24),
                                  serverIpEvents,
                                  usernameInfo,
                                  "",
                                  "localhost",
                                  axTextBox::Flags::FLASHING_CURSOR);
    
    axTextBox::Events serverPortEvents;
    serverPortEvents.enter_click = GetOnServerPortAdressEnter();
    _serverPortCtrl = new axTextBox(this,
                                  axRect(360, 0,
                                         25, 24),
                                  serverPortEvents,
                                  usernameInfo,
                                  "",
                                  "12",
                                  axTextBox::Flags::FLASHING_CURSOR);
    
    axButton::Info btn1_info;
    btn1_info.normal = axColor(0.8, 0.8, 0.8, 0.2);
    btn1_info.hover = axColor(0.9, 0.9, 0.9, 0.2);
    btn1_info.clicking = axColor(0.7, 0.7, 0.7, 0.2);
    btn1_info.contour = axColor(0.0, 0.0, 0.0, 0.2);
    btn1_info.selected = btn1_info.normal;
    
    axButton::Events btn1_evts;
    btn1_evts.button_click = GetOnConnectButton();
    
    _connectBtn = new axButton(this,
                               axRect(435, 2, 55, 21),
                               btn1_evts,
                               btn1_info,
                               "", "Connect");

}

void ChatGUI::CreateChat(const std::string& ip_adress, const std::string& port)
{
    try
    {
        _io_service = new boost::asio::io_service();
        
        tcp::resolver resolver(*_io_service);
        auto endpoint_iterator = resolver.resolve({ip_adress.c_str(),
                                                   port.c_str()});
        
        _chatClient = new chat_client(*_io_service, endpoint_iterator);
        _chatClient->SetEvtReceiveMsg(GetOnChatMsg());
        
        _io_thread = new std::thread([this]()
                                     {
                                         std::cout << "Run" << std::endl;
                                         _io_service->run();
                                     });
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}

void ChatGUI::OnConnectButton(axButton::Msg& msg)
{
    if(_io_service != nullptr && _io_thread != nullptr)
    {
        std::cout << "Stop thread." << std::endl;
        
        _io_service->stop();
        _io_thread->join();
        _chatClient->close();
        
        delete _io_service;
        delete _io_thread;
        delete _chatClient;
        
        _io_service = nullptr;
        _io_thread = nullptr;
        _chatClient = nullptr;
        
        std::cout << "Thread done." << std::endl;
        
        _scrollPanel->ClearLines();
    }
    
    CreateChat(_serverIpCtrl->GetLabel(),
               _serverPortCtrl->GetLabel());
 
}

void ChatGUI::OnServerIpAdressEnter(const axTextBox::Msg& msg)
{
    _serverIpCtrl->UnGrabKey();
}

void ChatGUI::OnServerPortAdressEnter(const axTextBox::Msg& msg)
{
    _serverPortCtrl->UnGrabKey();
}

void ChatGUI::OnUsernameEnter(const axTextBox::Msg& msg)
{
    _usernameCtrl->UnGrabKey();
}

void ChatGUI::OnTextEnterClick(const axTextBox::Msg& msg)
{
    // Get current time.
    time_t t = time(0);   // get time now
    tm * now = localtime(&t);
    
    std::string hour = to_string(now->tm_hour);
    std::string min = to_string(now->tm_min);
    std::string sec = to_string(now->tm_sec);
    
    if(now->tm_hour < 10) hour = "0" + hour;
    if(now->tm_min < 10) min = "0" + min;
    if(now->tm_sec < 10) sec = "0" + sec;

    std::string timestamp = hour + ":" + min + ":" + sec;
    
    
    // Add timestamp and username to message.
    std::string username = timestamp + "-->" + _usernameCtrl->GetLabel() + "-->";
    std::string line(msg.GetMsg());
    line = username + line;
    
    chat_message c_msg;
    c_msg.body_length(line.size());
    std::memcpy(c_msg.body(), line.c_str(), c_msg.body_length());
    c_msg.encode_header();
    _chatClient->write(c_msg);
    
    _txtCtrl->SetLabel("");
}

void ChatGUI::OnMouseLeftDown(const axPoint& pos)
{
        _txtCtrl->UnGrabKey();
}

void ChatGUI::OnChatMsg(const ChatMsg& msg)
{
    _scrollPanel->AddLine(msg.GetMsg());
    std::cout << "evt : " << msg.GetMsg() << std::endl;
}

void ChatGUI::OnPaint()
{
    axGC* gc = GetGC();
    axRect rect(GetDrawingRect());
    
    gc->SetColor(axColor(1.0, 1.0, 1.0, 1.0));
    gc->DrawRectangle(rect);
    
    
    gc->DrawRectangleColorFade(axRect(0, 0, GetRect().size.x, 25),
                               axColor(0.9, 0.9, 0.9, 1.0),
                               axColor(0.8, 0.8, 0.8, 1.0));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0));
    gc->DrawLine(axPoint(0, 25), axPoint(GetRect().size.x, 25));
    
    gc->SetColor(axColor(0.4, 0.4, 0.4));
    gc->DrawRectangleColorFade(axRect(0, 25, GetRect().size.x, 15),
                               axColor(0.6, 0.6, 0.6, 1.0),
                               axColor(0.55, 0.55, 0.55, 1.0));
    
    gc->SetColor(axColor(0.9, 0.9, 0.9));
    gc->DrawString(*_font, "Time", axPoint(5, 25));
    gc->DrawString(*_font, "Username", axPoint(65, 25));
    gc->DrawString(*_font, "Message", axPoint(145, 25));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0));
    gc->DrawString(*_font, "Username : ", axPoint(5, 5));
    gc->DrawString(*_font, "Server IP : ", axPoint(150, 5));
    gc->DrawString(*_font, "Port : ", axPoint(330, 5));
    
    gc->SetColor(axColor(0.0, 0.0, 0.0, 1.0));
    gc->DrawRectangleContour(rect);
}
