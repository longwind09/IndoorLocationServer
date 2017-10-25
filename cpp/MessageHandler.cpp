#include "stdafx.h"
#include "MessageHandler.h"
#include "MessageDispatcher.h"
#include "Server.h"
#include "ConnectionManager.h"
#include "InternalMessage.h"


#include "WifiSignalManager.h"
#include "KnowledgeSystem.h"

MessageHandler::MessageHandler ( boost::shared_ptr<DServer> svr_ptr ) :
    server_ptr ( svr_ptr ),
    conn_mgr_ptr ( svr_ptr->conn_mgr_ptr ),
    wifisignal_mgr_ptr(new WifiSignalManager),
    know_system_ptr(svr_ptr->know_sys_ptr)

{
}


void MessageHandler::onMove ( boost::shared_ptr<InternalMessage> in_msg )
{
    cout << in_msg->message->Utf8DebugString() << endl;
    //boost::shared_ptr<vrmsg::ActorMoving> move = boost::dynamic_pointer_cast<vrmsg::ActorMoving> ( in_msg->message );
    conn_mgr_ptr->broadcast ( in_msg->message );
     //ent_mgr_ptr->onMoving ( move );
}
void MessageHandler::onPosition ( boost::shared_ptr<InternalMessage> in_msg )
{
    cout << in_msg->message->Utf8DebugString() << endl;
    //boost::shared_ptr<vrmsg::Positioning> pos = boost::dynamic_pointer_cast<vrmsg::Positioning> ( in_msg->message );
    //// ent_mgr_ptr->onPositioning ( pos );
    //conn_mgr_ptr->broadcast ( pos );
}

//discard ¶ªÆú
void MessageHandler::discardProtobufMessage ( boost::shared_ptr<InternalMessage>  in_msg )
{
    cout << "Î´×¢²áµÄÏûÏ¢: " << in_msg->message->GetTypeName() << endl;
}

void MessageHandler::onProtoMessage ( boost::shared_ptr<protobuf::Message>  proto_msg )
{
    //server_ptr->conn_mgr_ptr->broadcast( proto_msg );
}

MessageHandler::~MessageHandler ( void )
{
}

void MessageHandler::onCreateRole ( boost::shared_ptr<InternalMessage> in_msg )
{
    cout << in_msg->message->Utf8DebugString();
    conn_mgr_ptr->broadcast (in_msg->message);
}

void MessageHandler::onLogin ( boost::shared_ptr<InternalMessage> in_msg )
{
    cout << in_msg->message->Utf8DebugString();
  
    boost::shared_ptr<vrmsg::LoginResponse> message(new vrmsg::LoginResponse);
    message->set_result(true);
    

    conn_mgr_ptr->unicast_by_conn_id ( in_msg->conn_id, message );
}

void MessageHandler::onUpdateScene ( boost::shared_ptr<InternalMessage> in_msg )
{
    cout << in_msg->message->Utf8DebugString();
    //conn_mgr_ptr->unicast_by_conn_id ( in_msg->conn_id, ent_mgr_ptr->onUpdateScene ( in_msg ) );
}

void MessageHandler::onLogout ( boost::shared_ptr<InternalMessage> in_msg )
{
    cout << in_msg->message->Utf8DebugString() << endl;
    // conn_mgr_ptr->unicast_by_conn_id ( in_msg->conn_id, ent_mgr_ptr->onLogout ( in_msg ) );
}


void MessageHandler::onCreateGroup ( boost::shared_ptr<InternalMessage> in_msg )
{
    cout << in_msg->message->Utf8DebugString() << endl;
    //conn_mgr_ptr->unicast_by_conn_id ( in_msg->conn_id, ent_mgr_ptr->onCreateGroup ( in_msg ) );
}

void MessageHandler::onGetGroup ( boost::shared_ptr<InternalMessage> in_msg )
{
    cout << in_msg->message->Utf8DebugString() << endl;
    //conn_mgr_ptr->unicast_by_conn_id ( in_msg->conn_id, ent_mgr_ptr->onGetGroup ( in_msg ) );
}

void MessageHandler::onJoinGroup ( boost::shared_ptr<InternalMessage> in_msg )
{
    cout << in_msg->message->Utf8DebugString() << endl;
    // conn_mgr_ptr->unicast_by_conn_id ( in_msg->conn_id, ent_mgr_ptr->onJoinGroup ( in_msg ) );
}

void MessageHandler::onStartTraining ( boost::shared_ptr<InternalMessage> in_msg )
{
    cout << in_msg->message->Utf8DebugString() << endl;
    server_ptr.lock()->startTraining();
}

void MessageHandler::onActionCompleting ( boost::shared_ptr<InternalMessage> in_msg )
{
    // ent_mgr_ptr->onActionCompleting ( in_msg );
}

void MessageHandler::onAddWifiSignal ( boost::shared_ptr<InternalMessage> in_msg )
{
    cout << in_msg->message->Utf8DebugString() << endl;
    wifisignal_mgr_ptr->onMsgAdd(in_msg);
}

void MessageHandler::onQueryPosition ( boost::shared_ptr<InternalMessage> in_msg )
{
    //cout << in_msg->message->Utf8DebugString() << endl;
    know_system_ptr->onQueryPosition(in_msg);
}

