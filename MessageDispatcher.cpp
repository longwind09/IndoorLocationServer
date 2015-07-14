#include "stdafx.h"
#include "message.h"
#include "MessageDispatcher.h"
#include "InternalMessage.h"
#include "MessageHandler.h"

MessageDispatcher::MessageDispatcher()
{
}
void MessageDispatcher::registerHandler ( MessageHandler &handler )
{
    registerMessageCallback<vrmsg::ActorMoving> ( boost::bind ( &MessageHandler::onMove, &handler, _1 ) );
    //registerMessageCallback<vrmsg::Positioning> ( boost::bind ( &MessageHandler::onPosition, &handler, _1 ) );
    registerMessageCallback<vrmsg::CreateRole> ( boost::bind ( &MessageHandler::onCreateRole, &handler, _1 ) );
    registerMessageCallback<vrmsg::LoginRequest> ( boost::bind ( &MessageHandler::onLogin, &handler, _1 ) );
    //registerMessageCallback<vrmsg::UpdateSceneRequest> ( boost::bind ( &MessageHandler::onUpdateScene, &handler, _1 ) );
    //registerMessageCallback<vrmsg::CreateGroupRequest> ( boost::bind ( &MessageHandler::onCreateGroup, &handler, _1 ) );
    //registerMessageCallback<vrmsg::GetGroupRequest> ( boost::bind ( &MessageHandler::onGetGroup, &handler, _1 ) );
    //registerMessageCallback<vrmsg::JoinGroupRequest> ( boost::bind ( &MessageHandler::onJoinGroup, &handler, _1 ) );
    //registerMessageCallback<vrmsg::StartTrainingRequest> ( boost::bind ( &MessageHandler::onStartTraining, &handler, _1 ) );
    //registerMessageCallback<vrmsg::ActionCompleting> ( boost::bind ( &MessageHandler::onActionCompleting, &handler, _1 ) );
    registerMessageCallback<vrmsg::QueryPostionRequest> ( boost::bind ( &MessageHandler::onQueryPosition, &handler, _1 ) );
    registerMessageCallback<vrmsg::AddWifiSignalRequest> ( boost::bind ( &MessageHandler::onAddWifiSignal, &handler, _1 ) );
    defaultCallback_ = ( boost::bind ( &MessageHandler::discardProtobufMessage, &handler, _1 ) );
    protoCallback_  = ( boost::bind ( &MessageHandler::onProtoMessage, &handler, _1 ) );
}


void MessageDispatcher::onInternalMessage ( boost::shared_ptr<InternalMessage>  in_msg ) const
{
    CallbackMap::const_iterator it = callbacks_.find ( in_msg->message->GetDescriptor() );

    if ( it != callbacks_.end() )
    {
        it->second->onMessage ( in_msg );
    }
    else
    {
        defaultCallback_ ( in_msg );
    }
}

void MessageDispatcher::onProtoMessage ( boost::shared_ptr<protobuf::Message>  proto_msg ) const
{
    protoCallback_ ( proto_msg );
}