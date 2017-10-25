// excerpts from http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (giantchen at gmail dot com)

#ifndef PROTOBUF_CODEC_H
#define PROTOBUF_CODEC_H

#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>


//#include <string>

/*#include <arpa/inet.h>  // htonl, ntohl*/

//#include <windows.h>
//#pragma comment(lib, "wsock32.lib")
//#include <stdint.h>

#include "stdafx.h"
#include "Utility.h"
using namespace google;


// struct ProtobufTransportFormat __attribute__ ((__packed__))
// {
//   4(hex)  len;
//   2(hex)  nameLen;
//   char     typeName[nameLen];
//   char     protobufData[len-nameLen-6];
// }

//该消息大小最大范围
const int kHeaderLen = 4;
const int kMessageNameLen = 2;



//////////////////////////////////////////////////////////////////////////
///////////////////////////for test//////////////////////////////

inline void printArray( const std::vector<Byte> &buf )
{

    for( vector<Byte>::const_iterator it = buf.begin(); it != buf.end(); ++it )
    {
        cout << ( int )*it << " " ;

    }
    cout << endl;
}



///
/// Encode protobuf Message to transport format defined above
/// returns a std::string.
///
/// returns a empty string if message.AppendToString() fails.
///
/************************************************************************/
/* 把消息类型名的长度、消息类型名写在消息前
 * */
/************************************************************************/
//inline  std::string encode(  boost::shared_ptr<protobuf::Message> message )
//{
//    std::string result;
//
//    //resize函数用法
//    result.resize( kHeaderLen );
//
//    string typeName = message->GetTypeName();
//    int nameLen = typeName.length();
//    //result后面加类型名长度和类型名
//    result.append( Utility::toHexString( nameLen, kMessageNameLen ) );
//    result.append( typeName );
//    //后面跟消息体
//    bool succeed = message->AppendToString( &result );
//    //成功后加校验值
//    if ( succeed )
//    {
//        int body_len = result.length() - kHeaderLen;
//        string str_body_length = Utility::toHexString( body_len, kHeaderLen );
//
//        std::copy( str_body_length.begin(), str_body_length.end(), result.begin() );
//
//    }
//    else
//    {
//        result.clear();
//    }
//
//    return result;
//
//}



inline std::vector<Byte> EncodeAsByteArray(  boost::shared_ptr<protobuf::Message> message )
{
    std::string type_header = "";
    std::vector<Byte> data;
    int message_byte_size = message->ByteSize();
    
    string typeName = message->GetTypeName();
    int nameLen = typeName.length();
    //result后面加类型名长度和类型名
    type_header.append( Utility::toHexString( nameLen, kMessageNameLen ) );
    type_header.append( typeName );
    data.resize( type_header.length() + message_byte_size + kHeaderLen );
    std::copy( type_header.begin(), type_header.end(), data.begin() + kHeaderLen );
    //后面跟消息体
    bool succeed = message->SerializeToArray( data.data() + kHeaderLen + kMessageNameLen + nameLen, message_byte_size );
    //成功后加校验值
    if ( succeed )
    {
        int body_len = kMessageNameLen + nameLen + message_byte_size;
        string str_body_length = Utility::toHexString( body_len, kHeaderLen );
        std::copy( str_body_length.begin(), str_body_length.end(), data.begin() );
    }
    else
    {
        type_header.clear();
        data.clear();
    }

    // cout << "bytes send:" << endl;
    //printArray( data );
    return data;

}


/************************************************************************/
/*     从消息类型名创建消息，利用了反射
 *     返回消息的智能指针
 *  */
/************************************************************************/
inline boost::shared_ptr<protobuf::Message> createMessage( const std::string & type_name )
{
    boost::shared_ptr<protobuf::Message> message;
    const google::protobuf::Descriptor* descriptor =
        google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName( type_name );
    if ( descriptor )
    {
        const protobuf::Message* prototype =
            protobuf::MessageFactory::generated_factory()->GetPrototype( descriptor );
        if ( prototype )
        {
            message.reset( prototype->New() ) ;
        }
    }

    return message;
}

/************************************************************************/
/*  　解码方法，返回消息的智能指针
	  buf:去掉消息头的部分
 * */
/************************************************************************/

//inline boost::shared_ptr<protobuf::Message> decode( const std::string & buf )
//{
//    boost::shared_ptr<protobuf::Message> result;
//
//    int len = buf.length();
//    if ( len >= 10 )
//    {
//        string str_name_len = buf.substr( 0, kMessageNameLen );
//        int nameLen = Utility::hexStringToInt( str_name_len );
//        if ( nameLen >= 2 && nameLen <= 255 )
//        {
//            std::string typeName( buf.begin() + kMessageNameLen, buf.begin() + kMessageNameLen + nameLen );
//            boost::shared_ptr<protobuf::Message> message = createMessage( typeName );
//            if ( message != NULL )
//            {
//                const char* data = buf.c_str() + kMessageNameLen + nameLen;
//                int32_t dataLen = len - nameLen - kMessageNameLen;
//                if ( message->ParseFromArray( data, dataLen ) )
//                {
//                    result = message;
//
//                }
//                else
//                {
//                    // parse error
//                    throw std::exception( "parse error" );
//                }
//            }
//            else
//            {
//                // unknown message type
//            }
//        }
//        else
//        {
//            // invalid name len
//        }
//    }
//
//    return result;
//}


inline boost::shared_ptr<protobuf::Message> decode( const std::vector<Byte> &buf )
{
    // cout << "bytes receive:" << endl;
    /*printArray( buf );*/
    boost::shared_ptr<protobuf::Message> result;

    int len = buf.size();
    if ( len >= 10 )
    {
        std::string str_name_len = string( buf.begin(), buf.begin() + kMessageNameLen );

        int nameLen = Utility::hexStringToInt( str_name_len );
        if ( nameLen >= 2 && nameLen <= 255 )
        {
            std::string typeName( buf.begin() + kMessageNameLen, buf.begin() + kMessageNameLen + nameLen );
            //cout<<typeName<<endl;
            boost::shared_ptr<protobuf::Message> message = createMessage( typeName );
            if ( message != NULL )
            {
                const Byte* data = buf.data() + kMessageNameLen + nameLen;
                int32_t dataLen = len - nameLen - kMessageNameLen;
                if ( message->ParseFromArray( data, dataLen ) )
                {
                    result = message;


                }
                else
                {
                    // parse error

                }
            }
            else
            {
                // unknown message type
            }
        }
        else
        {
            // invalid name len
        }
    }

    return result;
}




#endif  // PROTOBUF_CODEC_H