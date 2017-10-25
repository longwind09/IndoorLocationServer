#include "stdafx.h"

#include "BootstrapServer.h"

BootstrapServer::BootstrapServer ( boost::asio::io_service& io_service )
    : socket_ ( io_service, ip::udp::endpoint ( ip::udp::v4(), 8888 ) )
{
    //start_service();
}

void BootstrapServer::start_service()
{
    socket_.async_receive_from (
        boost::asio::buffer ( recv_buffer_ ), remote_endpoint_,
        boost::bind ( &BootstrapServer::handle_receive, this,
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred ) );
}

void BootstrapServer::stop_service()
{
    socket_.shutdown ( boost::asio::ip::tcp::socket::shutdown_both );
    // 关闭socket之前最好先调用shutdown
    socket_.close();
}

void BootstrapServer::handle_receive (
    const boost::system::error_code& error, std::size_t bytes_transferred )
{
    //std::cout << error.message() << std::endl;
    if ( !error || error == boost::asio::error::message_size )
    {
        std::cout << "client found:" << remote_endpoint_ << std::endl;
        boost::shared_ptr<std::string> message ( new std::string ( ip::host_name() ) );
        socket_.async_send_to (
            boost::asio::buffer ( *message ), remote_endpoint_,
            boost::bind ( &BootstrapServer::handle_send, this, message,
                          boost::asio::placeholders::error,
                          boost::asio::placeholders::bytes_transferred ) );
        start_service();
    }
    
    else
    {
        std::cout << "接收失败： " << error.message() << std::endl;
    }
}

void BootstrapServer::handle_send (
    boost::shared_ptr<std::string>, const boost::system::error_code& error, std::size_t )
{
    if ( error && error != boost::asio::error::message_size )
    {
        std::cout << "发送失败：" << error.message() << std::endl;
    }
}