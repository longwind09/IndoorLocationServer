/************************************************************************/
/* 服务器端通信主体
 * 封装了ConnectionBase的收发消息功能                                    */
/************************************************************************/

#ifndef __CONNECTION_SERVER_H__
#define __CONNECTION_SERVER_H__
#pragma once

#include "Stdafx.h"
#include "Utility.h"
#include "ConnectionBase.h"
//#include "message.h"

using namespace google;

class DServer;
class protobuf::Message;
class MessageDispatcher;
class MessageForwarder;
class InternalMessage;

class ConnectionServer : public ConnectionBase, public boost::enable_shared_from_this<ConnectionServer>
{
    public:

        ConnectionServer ( const boost::shared_ptr<DServer> svr_ptr );

        void output_error ( std::string err_msg, const boost::system::error_code &e );

        void start();
        void stop();
        void start_echo();

        void push_message ( boost::shared_ptr<protobuf::Message> msg );
        void push_messages ( const std::vector<boost::shared_ptr<protobuf::Message>> msgs );
        void send_message();
        void send_message_handler ( const boost::system::error_code& e );

        void recv_message();
        void recv_message_handler ( const boost::system::error_code& e );
        void proccess_message();

        void handle_timeout ( const boost::system::error_code& ec, std::string ss );

        //void disconnect();

        //make sure all completion handler of async operations have been called
        void on_client_disconnect();


        boost::weak_ptr<DServer> server_ptr;

        // boost::shared_ptr<DServer> server_ptr;


        std::string endpoint;//连接ID，非endpoint，accept是赋予
        std::string role_id;//连接对应的role_id，select_role成功时赋予，cancel时reset，连接断开时不变
        std::string conn_id;
        std::string user_name;
        std::string ip;

        bool connected;
        bool sending;

        std::queue<boost::shared_ptr<protobuf::Message>> messages_buffer_send;
        boost::recursive_mutex messages_buffer_send_mutex;

        //收到的消息
        boost::mutex temp_msg_mutex;
        boost::shared_ptr<protobuf::Message> message_buffer_recv;
        //消息队列
        std::queue< boost::shared_ptr<protobuf::Message>> messages_queue_recv;
        std::queue<boost::shared_ptr<protobuf::Message>> messages_queue_op;
        //消息队列锁
        boost::recursive_mutex messages_queue_recv_mutex;
        boost::recursive_mutex messages_queue_op_mutex;


        //收消息定时器
        boost::shared_ptr<boost::asio::deadline_timer> echo_timer_ptr;
        boost::shared_ptr<boost::asio::deadline_timer> timeout_timer_ptr;


};

#endif