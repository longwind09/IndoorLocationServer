#ifndef __MESSAGE_FORWARDER_H__
#define __MESSAGE_FORWARDER_H__

#pragma once

#include "stdafx.h"
#include "message.h"


class DServer;
class InternalMessage;
class MessageDispatcher;
class MessageHandler;
using namespace google;

/************************************************************************/
/*      MessageForwarder is utilized to forward message from ConnectionServer
 *      to dispatcher with another thread
 *                                                                      */
/************************************************************************/
class MessageForwarder : public boost::enable_shared_from_this<MessageForwarder>
{
    public:

        MessageForwarder ( boost::shared_ptr<DServer> svr_ptr );
        void forward_internal_message ( boost::shared_ptr<InternalMessage> in_msg );
        void forward_proto_message ( boost::shared_ptr<protobuf::Message> proto_msg );
        void registerHandler( MessageHandler &handler);

        boost::weak_ptr<DServer> server_ptr;
        boost::shared_ptr<boost::asio::io_service> io_s_ptr;
        boost::shared_ptr<MessageDispatcher> dispatcher_ptr;
};

#endif