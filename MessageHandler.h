#ifndef __MESSAGE_HANDLER_H__
#define __MESSAGE_HANDLER_H__

#include "stdafx.h"
#include "message.h"




#pragma once

using namespace google;


class DServer;
class MessageDispatcher;
class InternalMessage;
class ConnectionManager;


class MessageDispatcher;
class KnowledgeSystem;
class WifiSignalManager;

class MessageHandler : public boost::enable_shared_from_this<MessageHandler>
{
    public:
        MessageHandler ( boost::shared_ptr<DServer> svr_ptr );
        virtual ~MessageHandler( );


        void onMove ( boost::shared_ptr<InternalMessage> in_msg );
        void onPosition ( boost::shared_ptr<InternalMessage> in_msg );
        void discardProtobufMessage ( boost::shared_ptr<InternalMessage> in_msg );
        void onProtoMessage ( boost::shared_ptr<protobuf::Message> proto_msg );
        void onCreateRole ( boost::shared_ptr<InternalMessage> in_msg );
        void onCreateGroup ( boost::shared_ptr<InternalMessage> in_msg );
        void onGetGroup ( boost::shared_ptr<InternalMessage> in_msg );
        void onJoinGroup ( boost::shared_ptr<InternalMessage> in_msg );


        void onLogin ( boost::shared_ptr<InternalMessage> in_msg );
        void onLogout ( boost::shared_ptr<InternalMessage> in_msg );
        void onUpdateScene ( boost::shared_ptr<InternalMessage> in_msg );


        void onStartTraining ( boost::shared_ptr<InternalMessage> in_msg );
        void onActionCompleting ( boost::shared_ptr<InternalMessage> in_msg );

        void onAddWifiSignal( boost::shared_ptr<InternalMessage> in_msg );
        void onQueryPosition(boost::shared_ptr<InternalMessage> in_msg);

    private:
        friend MessageDispatcher;
        boost::weak_ptr<DServer> server_ptr;
        boost::shared_ptr<ConnectionManager> conn_mgr_ptr;

        boost::shared_ptr<KnowledgeSystem> know_system_ptr;
        boost::shared_ptr<WifiSignalManager> wifisignal_mgr_ptr;
        //boost::shared_ptr<entity_manager> ent_mgr_ptr;


};


#endif