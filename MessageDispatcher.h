#ifndef __DISPATCHER_H__
#define __DISPATCHER_H__
#pragma once

#include "stdafx.h"

#include "message.h"



using namespace std;
using namespace google;


class InternalMessage;
class MessageHandler;

class Callback : boost::noncopyable
{
    public:
        virtual ~Callback() {};
        virtual void onMessage ( boost::shared_ptr<InternalMessage>  in_msg ) const = 0;
};


template <typename T>
class CallbackT : public Callback
{
    public:
        typedef boost::function<void ( boost::shared_ptr<InternalMessage> ) > InternalMessageCallback;

        CallbackT ( const InternalMessageCallback& callback )
            : callback_ ( callback )
        {
        }

        virtual void onMessage ( boost::shared_ptr<InternalMessage>  in_msg ) const
        {
            /*  boost::shared_ptr<T> t = boost::dynamic_pointer_cast<T>( in_msg->message );*/
            callback_ ( in_msg );
        }

    private:
        InternalMessageCallback callback_;
};



class MessageDispatcher : public boost::enable_shared_from_this<MessageDispatcher>
{
    public:

        MessageDispatcher::MessageDispatcher();

        template<typename T>
        void registerMessageCallback ( const typename CallbackT<T>::InternalMessageCallback& callback )
        {
            boost::shared_ptr<CallbackT<T> > pd ( new CallbackT<T> ( callback ) );
            callbacks_[T::descriptor()] = pd;
        }

        void registerHandler ( MessageHandler &handler );

        void onInternalMessage ( boost::shared_ptr<InternalMessage> in_msg ) const;

        void onProtoMessage ( boost::shared_ptr<protobuf::Message> proto_msg ) const;

        typedef std::map<const protobuf::Descriptor*, boost::shared_ptr<Callback> > CallbackMap;

        CallbackMap callbacks_;

        boost::function<void ( boost::shared_ptr<InternalMessage> ) > defaultCallback_;
        //未经处理,直接转发的消息回调
        boost::function<void ( boost::shared_ptr<protobuf::Message> ) > protoCallback_;
};


#endif