/************************************************************************/
/* ConnectionBase.h
 * boost asio socket 通信基类，实现了异步收发消息
/************************************************************************/


#ifndef __CONNECTION_BASE_H__
#define __CONNECTION_BASE_H__

#pragma once

#include "stdafx.h"
#include "codec.h"


class ConnectionBase
{
    public:

        ConnectionBase ( boost::shared_ptr<boost::asio::io_service> io_service_ptr );

        //基类虚析构函数
        virtual ~ConnectionBase();

        boost::asio::ip::tcp::socket& socket();

        //写，t不能变
        template <typename T, typename Handler>
        void async_write ( const T &t, Handler handler );

        //读，变的就是t
        template <typename T, typename Handler>
        void async_read ( T &t, Handler handler );


        template <typename T, typename Handler>
        void handle_read_header ( const boost::system::error_code& e,
                                  T &t, boost::tuple<Handler> handler );


        template <typename T, typename Handler>
        void handle_read_data ( const boost::system::error_code& e,
                                T &t, boost::tuple<Handler> handler );


        //可以用#define
        enum { header_length = 4, type_length = 4 };

        //socket_通信用
        boost::asio::ip::tcp::socket socket_;
        //strand_ wrap用？？？
        boost::asio::strand strand_;

        std::string outbound_header_;
        std::vector<Byte> outbound_data_;

        //数据包头部定长，数据部变长
        std::vector<Byte> inbound_header_;
        // Byte inbound_header_[header_length];
        char inbound_type_[type_length];
        std::vector<Byte> inbound_data_;

};



template<typename T, typename Handler>
void ConnectionBase::async_write ( const T &t, Handler handler )
{
    outbound_data_ = EncodeAsByteArray ( t );
    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back ( boost::asio::buffer ( outbound_data_ ) );
    boost::asio::async_write ( socket_, buffers, handler );
}


/************************************************************************/
/* asyn_read consist of read_header and read_data               */
/************************************************************************/

template <typename T, typename Handler>
void ConnectionBase::async_read ( T &t, Handler handler )
{
    //函数指针 ConnectionBase::*f 的ConnectionBase::可以不要吧？这里可以再f的地方用handle_read_header
    //tuple   很多的时候我们经常需要为我们的函数返回多个值 std::pair其实就是boost::tuple的2个参数的特例，对boost::tuple你可以绑定更多的参数，或者你可以迭代实现无限多参数的情况
    void ( ConnectionBase::*f ) ( const boost::system::error_code&, T&, boost::tuple<Handler> ) = &ConnectionBase::handle_read_header<T, Handler>;
    inbound_header_.clear();
    inbound_header_.resize ( header_length );
    boost::asio::async_read ( socket_,
                              boost::asio::buffer ( inbound_header_ ),
                              boost::bind ( f,
                                            this,
                                            boost::asio::placeholders::error,
                                            boost::ref ( t ),
                                            boost::make_tuple ( handler ) ) );
}

template <typename T, typename Handler>
void ConnectionBase::handle_read_header ( const boost::system::error_code& e,
        T &t, boost::tuple<Handler> handler )
{
    if ( e )
    {
        boost::get<0> ( handler ) ( e );
    }
    else
    {
        std::istringstream is ( std::string ( inbound_header_.begin(), inbound_header_.end() ) );
        std::size_t inbound_data_size = 0;
        //消息头内容最小为20
        if ( ! ( is >> std::hex >>  inbound_data_size ) )
        {
            boost::system::error_code error ( boost::asio::error::invalid_argument );
            boost::get<0> ( handler ) ( error );
            return;
        }
        if ( inbound_data_size < 20 )
        {

            return;
        }


        inbound_data_.resize ( inbound_data_size );
        void ( ConnectionBase::*f ) ( const boost::system::error_code&, T&, boost::tuple<Handler> ) = &ConnectionBase::handle_read_data<T, Handler>;
        boost::asio::async_read ( socket_,
                                  boost::asio::buffer ( inbound_data_ ),
                                  boost::bind ( f,
                                                this,
                                                boost::asio::placeholders::error,
                                                boost::ref ( t ),
                                                handler ) );
    }
}

template <typename T, typename Handler>
void ConnectionBase::handle_read_data ( const boost::system::error_code& e,
                                        T &t, boost::tuple<Handler> handler )
{
    if ( e )
    {
        boost::get<0> ( handler ) ( e );
    }
    else
    {
        try
        {
            t = decode ( inbound_data_ );
        }
        catch ( std::exception& e )
        {
            boost::system::error_code error ( boost::asio::error::invalid_argument );
            boost::get<0> ( handler ) ( error );
            return;
        }

        boost::get<0> ( handler ) ( e );
    }
}

#endif