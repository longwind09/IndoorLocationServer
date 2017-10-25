/************************************************************************/
/* 抽象的服务端,要注意各大组件的初始化顺序！！！切记
 *																		*/
/************************************************************************/


#ifndef __DBSERVER_H__
#define __DBSERVER_H__

#pragma once

#include "stdafx.h"
#include "log.h"
//#include "configuration.h"


class ConnectionServer;
class ConnectionManager;
class MessageForwarder;
class MessageHandler;
//class InternalMessage;




//class timer_scheduler;

class BootstrapServer;

class DataSourceManager;
class KnowledgeSystem;
class WifiSignalManager;

class ServerCommand;



#define THREADS_NUM 5


class DServer : public boost::enable_shared_from_this<DServer>
{
    public:


        DServer();

        void output_error ( std::string err_msg, const boost::system::error_code &e );

        void init ( const std::string& working_path,
                    const std::string& version,
                    const std::string& drill );

        void __DataTest();

        void start_service();




        void del_conn ( boost::shared_ptr<ConnectionServer> conn );

        boost::mutex gen_id_mutex;
        std::string gen_id();//id, from "1"

        //void handle_upload_role ( std::string endpoint, InternalMessage in_msg ); //用于掉线重连;
        //void handle_select_role ( std::string endpoint, InternalMessage in_msg ); //用于正常，或重启后选择角色;
        void handle_timeout ( std::string role_id ); //处理连接中断，这里只set NPC

        void load_scripts();
        void startTraining();

        void exitServer();
        void runCmd();

        boost::shared_ptr<boost::asio::io_service> io_service_ptr;
        boost::shared_ptr<boost::asio::io_service> bootp_service_ptr;
        //启动服务
        boost::shared_ptr<BootstrapServer> bootp_server_ptr;

        //single acceptor
        //boost::asio::ip::tcp::acceptor acceptor;

        boost::shared_ptr<ConnectionServer> client_ptr;//to accept connection
        std::list<boost::shared_ptr<ConnectionServer>> clients_list;
        boost::mutex clients_list_mutex;

        //由endpoint(id)索引到connection
        std::map<std::string, boost::weak_ptr<ConnectionServer>> endpoint_map;
        boost::mutex endpoint_map_mutex;
        //由role_id索引到connection，用于重连相关
        std::map<std::string, boost::weak_ptr<ConnectionServer>> role_id_map;
        boost::mutex role_id_map_mutex;

        boost::thread_group tg;

        boost::shared_ptr<MessageForwarder> msg_forwarder;

       // boost::shared_ptr<timer_scheduler> timer_scheduler_ptr;

        boost::shared_ptr<ConnectionManager> conn_mgr_ptr;
        DataSourceManager* ds_manager;
        //added 20150313
        boost::shared_ptr<MessageHandler>	msg_handler_ptr;

        boost::shared_ptr<KnowledgeSystem> know_sys_ptr;
        boost::shared_ptr<WifiSignalManager> wifi_mgr_ptr;

        boost::shared_ptr<ServerCommand> cmd_ptr;


        std::string version;
        //configuration config;
        Log log;
};

#endif