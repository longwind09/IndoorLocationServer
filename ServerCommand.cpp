#include "stdafx.h"
#include "ServerCommand.h"

#include "server.h"
#include "KnowledgeSystem.h"


using namespace std;







ServerCommand::ServerCommand ( boost::shared_ptr<DServer> svr_ptr ) :
    p_server ( svr_ptr )
{

}


ServerCommand::~ServerCommand ( void )
{
}

void ServerCommand::run()
{
    init();
    // showUI();
}

void ServerCommand::showUI()
{
    cout << "-------------server cmd------------------" << endl;
    
    for ( CallbackMap::iterator it = callbacks_.begin(); it != callbacks_.end(); ++it )
    {
        cout << "\t" << it->first << endl;
    }
    
    cout << endl << endl;
    cout << "server>";
    waitOrder();
    // showUI();
}

void ServerCommand::regCallback ( std::string function, const  CommandCallback::MyCallback& callback )
{
    boost::shared_ptr<CommandCallback > cb ( new CommandCallback ( callback ) );
    callbacks_[function] = cb;
}

void ServerCommand::waitOrder()
{
    std::string cmd;
    cin >> cmd;
    
    if ( callbacks_.find ( cmd ) != callbacks_.end() )
    {
        callbacks_[cmd]->onCommand();
    }
    
    cout << endl;
}

void ServerCommand::init()
{
    regCallback ( "process", boost::bind ( &KnowledgeSystem::ProcessRawData, & ( * ( p_server.lock()->know_sys_ptr ) ) ) );
    regCallback ( "showdata", boost::bind ( &KnowledgeSystem::showWifiData, & ( * ( p_server.lock()->know_sys_ptr ) ) ) );
    regCallback ( "exit", boost::bind ( &DServer::exitServer, & ( * ( p_server.lock() ) ) ) );
    callbacks_["process"]->onCommand();
    callbacks_["showdata"]->onCommand();
}
