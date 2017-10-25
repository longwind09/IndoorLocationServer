#ifndef __INTERNAL_MESSAGE_H__
#define __INTERNAL_MESSAGE_H__

#pragma once



#include "stdafx.h"
#include "message.h"



using namespace google;
using namespace std;



/************************************************************************/
/* 内部消息
 * 用于服务器内部各组件间传递
 * */
/************************************************************************/
class InternalMessage
{

    public :

        InternalMessage::InternalMessage ( boost::shared_ptr<protobuf::Message> msg, string con_id )
            : message ( msg ),
              conn_id ( con_id ),
              delay_time ( 0 )
        {

        }
        InternalMessage::InternalMessage ( boost::shared_ptr<protobuf::Message> msg )
            : message ( msg ),
              broadcast_option ( 0 ),
              delay_time ( 0 )
        {

        }

        //外部消息
        boost::shared_ptr<protobuf::Message> message;
        //连接id
        std::string conn_id;
        std::string endpoint;

        int delay_time;//-1, 0, positive
        int broadcast_option;//-1, 0(broadcast)
};













#endif