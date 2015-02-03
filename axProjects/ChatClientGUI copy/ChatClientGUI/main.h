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
#include "axLib.h"
#include <thread>

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

class chat_client : public axObject
{
public:
    chat_client(boost::asio::io_service& io_service,
                tcp::resolver::iterator endpoint_iterator);
    
    void write(const chat_message& msg);
    
    void close();
    
    typedef std::function<void(boost::system::error_code ec, std::size_t)>
    CallbackFunction;
    
    void SetEvtReceiveMsg(axEventFunction fct)
    {
        _evtReceiveMsg = fct;
        
        if(_evtReceiveMsg)
        {
            AddConnection(0, _evtReceiveMsg);
        }
    }
    
private:
    void do_connect(tcp::resolver::iterator endpoint_iterator);
    
    void do_read_header();
    
    void do_read_body();
    
    void do_write();
    
    
    
//private:
    
    axEventFunction _evtReceiveMsg;
    
    
    boost::asio::io_service& io_service_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_queue write_msgs_;
};
//
//

class ChatMsg : public axMsg
{
public:
    ChatMsg(const std::string& msg)
    {
        _msg = msg;
    }

    std::string GetMsg() const
    {
        return _msg;
    }
    
    axMsg* GetCopy()
    {
        return new ChatMsg(*this);
    }
    
private:
    std::string _msg;
};

class MyScrollPanel : public axPanel
{
public:
    MyScrollPanel(axWindow* parent,
                  const axRect& rect,
                  const axSize& seenSize);
    
    void AddLine(const std::string& line);
    void ClearLines();
    
    axEVENT_ACCESSOR(axButton::Msg, OnBtn);
    
private:
    axStringVector _strVector;
    axFont* _font;
    
    virtual void OnPaint();
    virtual void OnPaintStatic();
    
    virtual void OnMouseLeftDown(const axPoint& pos);
    
    void OnBtn(const axButton::Msg& msg);
};

class ChatGUI : public axPanel
{
public:
    ChatGUI();
    
    axEVENT_ACCESSOR(ChatMsg, OnChatMsg);
    axEVENT_ACCESSOR(axTextBox::Msg, OnTextEnterClick);
    axEVENT_ACCESSOR(axButton::Msg, OnConnectButton);
    axEVENT_ACCESSOR(axTextBox::Msg, OnUsernameEnter);
    axEVENT_ACCESSOR(axTextBox::Msg, OnServerIpAdressEnter);
    axEVENT_ACCESSOR(axTextBox::Msg, OnServerPortAdressEnter);
    void CreateChat(const std::string& ip_adress, const std::string& port);
    
private:
    std::thread* _io_thread;
    chat_client* _chatClient;
    boost::asio::io_service* _io_service;
    axFont* _font;
    
    axTextBox* _txtCtrl;
    axTextBox* _usernameCtrl;
    axTextBox* _serverIpCtrl;
    axTextBox* _serverPortCtrl;
    axButton* _connectBtn;
    
    axStringVector _strVector;
    MyScrollPanel* _scrollPanel;
    
    void OnMouseLeftDown(const axPoint& pos);
    void OnChatMsg(const ChatMsg& msg);
    void OnTextEnterClick(const axTextBox::Msg& msg);
    
    void OnUsernameEnter(const axTextBox::Msg& msg);
    void OnServerIpAdressEnter(const axTextBox::Msg& msg);
    void OnServerPortAdressEnter(const axTextBox::Msg& msg);
    
    void OnConnectButton(axButton::Msg& msg);
    virtual void OnPaint();
    
    
};

#endif /* defined(__ChatClient__main__) */
