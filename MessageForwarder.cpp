#include "stdafx.h"
#include "MessageForwarder.h"

#include "server.h"
#include "InternalMessage.h"
#include "MessageDispatcher.h"
#include "MessageHandler.h"

MessageForwarder::MessageForwarder ( boost::shared_ptr<DServer> svr_ptr )
    : server_ptr ( svr_ptr ),
      dispatcher_ptr ( new MessageDispatcher() ),
      io_s_ptr ( server_ptr.lock()->io_service_ptr )
{

}



void MessageForwarder::forward_internal_message ( boost::shared_ptr<InternalMessage> in_msg )
{
    dispatcher_ptr->onInternalMessage ( in_msg );
}

void MessageForwarder::forward_proto_message ( boost::shared_ptr<protobuf::Message> proto_msg )
{
    dispatcher_ptr->onProtoMessage ( proto_msg );
}

void MessageForwarder::registerHandler( MessageHandler &handler)
{
    dispatcher_ptr->registerHandler ( handler);
}
