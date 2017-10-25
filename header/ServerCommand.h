/************************************************************************/
/*
       程序运行期间接受用户输入执行不同功能的命令行
*/
/************************************************************************/
#ifndef __SERVER_COMMAND_H__
#define __SERVER_COMMAND_H__

#pragma once
#include "stdafx.h"

class CommandCallback :  boost::noncopyable
{
    public:
        typedef boost::function<void () > MyCallback;
        
        CommandCallback ( const MyCallback& callback )
            : callback_ ( callback )
        {
        }
        
        void onCommand () const
        {
            callback_ ();
        }
        
    private:
        MyCallback callback_;
};


class DServer;


class ServerCommand
{
    public:
        ServerCommand (  boost::shared_ptr<DServer> svr_ptr );
        ~ServerCommand ( void );
        
        void run();
        void showUI();
        void init();
        void waitOrder();
        
        void regCallback ( std::string function, const  CommandCallback::MyCallback& callback );
        
    private:
        typedef std::map<const std::string, boost::shared_ptr<CommandCallback> > CallbackMap;
        CallbackMap callbacks_;
        boost::weak_ptr<DServer> p_server;
        
        
        
        
        
        
};

#endif