#include "stdafx.h"
#include "ConnectionServer.h"

#include "utility.h"
#include "Server.h"
#include "MessageDispatcher.h"
#include "MessageForwarder.h"
#include "InternalMessage.h"
#include "ConnectionManager.h"

using namespace std;

ConnectionServer::ConnectionServer ( const boost::shared_ptr<DServer> svr_ptr ) :
    server_ptr ( svr_ptr ),
    ConnectionBase ( svr_ptr->io_service_ptr )
{
    connected = sending = false;
    echo_timer_ptr.reset ( new boost::asio::deadline_timer ( *svr_ptr->io_service_ptr ) );
    timeout_timer_ptr.reset ( new boost::asio::deadline_timer ( *svr_ptr->io_service_ptr ) );
    conn_id = Utility::gen_id();
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void ConnectionServer::output_error ( std::string err_msg, const boost::system::error_code &e )
{
    cerr << err_msg << ", error Message: " << e.message() << endl;
}

/************************************************************************/
/*    连接成功后开始通信
 *                                                                     */
/************************************************************************/
void ConnectionServer::start()
{
    recv_message();

    if ( connected && !sending ) send_message();

    // start_echo();
}

void ConnectionServer::stop()
{
    boost::system::error_code ignored_ec;
    socket_.shutdown ( boost::asio::socket_base::shutdown_both );
    socket_.close ( ignored_ec );
}

void ConnectionServer::push_message ( boost::shared_ptr<protobuf::Message> msg )
{
    boost::recursive_mutex::scoped_lock lock ( messages_buffer_send_mutex );

    messages_buffer_send.push ( msg );

    if ( connected && !sending ) send_message();
}

void ConnectionServer::push_messages ( const std::vector<boost::shared_ptr<protobuf::Message>> msgs )
{
    boost::recursive_mutex::scoped_lock lock ( messages_buffer_send_mutex );

    for ( int i = 0; i < msgs.size(); ++i )
    {
        push_message ( msgs[i] );
    }

    if ( connected && !sending ) send_message();
}

void ConnectionServer::send_message()
{
    boost::recursive_mutex::scoped_lock lock ( messages_buffer_send_mutex );

    if ( messages_buffer_send.empty() )
    {
        sending = false;
        return;
    }

    sending = true;

    boost::shared_ptr<protobuf::Message> msg;
    msg = messages_buffer_send.front();

    // std::cout << "send Message, msg->GetTypeName=" << msg->GetTypeName() << endl;

    async_write ( msg, strand_.wrap ( boost::bind ( &ConnectionServer::send_message_handler, shared_from_this(), boost::asio::placeholders::error ) ) );
}

void ConnectionServer::send_message_handler ( const boost::system::error_code& e )
{
    if ( e )
    {
        output_error ( "send error", e );
        sending = false;
        boost::system::error_code no_error;

        if ( e != boost::asio::error::operation_aborted ) handle_timeout ( no_error, "in send_handler" );

        return;
    }

    else
    {
        boost::recursive_mutex::scoped_lock lock ( messages_buffer_send_mutex );

        messages_buffer_send.pop();
    }

    send_message();
}

//该函数只能单线程进来
void ConnectionServer::recv_message()
{
    // timeout_timer_ptr->expires_from_now ( boost::posix_time::seconds ( 60 ) );
    timeout_timer_ptr->async_wait ( boost::bind ( &ConnectionServer::handle_timeout, shared_from_this(), boost::asio::placeholders::error, "in recv" ) );
    // Use of strands allows execution of code in a multithreaded program
    // without the need for explicit locking
    async_read (
        message_buffer_recv,
        strand_.wrap (
            boost::bind ( &ConnectionServer::recv_message_handler,
                          shared_from_this(),
                          boost::asio::placeholders::error ) ) );
}

void ConnectionServer::recv_message_handler ( const boost::system::error_code& e )
{
    if ( e )
    {
        output_error ( "recv error", e );
        //on_client_disconnect();
        boost::system::error_code no_error;

        if ( e != boost::asio::error::operation_aborted ) handle_timeout ( no_error, "in recv_handler" );

        return;
    }

    else
    {
        timeout_timer_ptr->cancel();

        //  boost::recursive_mutex::scoped_lock lock( messages_queue_recv_mutex );
        // messages_queue_recv.push( message_buffer_recv );
        proccess_message();
        // cout << "process  " << message_buffer_recv->GetTypeName() << endl;
        recv_message();

    }


}


void ConnectionServer::proccess_message()
{


    boost::shared_ptr<InternalMessage> in_msg ( new InternalMessage ( message_buffer_recv, conn_id ) );

    //push_message( message_buffer_recv );

    //messages_queue_recv.pop();

    //
    //转发消息，收到消息后考虑把消息放在队列里面，让别的线程去处理

    // boost::shared_ptr<InternalMessage> in_msg( new InternalMessage( message_buffer_recv, conn_id ) );
    server_ptr.lock()->io_service_ptr->post ( boost::bind ( &MessageForwarder::forward_internal_message, server_ptr.lock()->msg_forwarder,  in_msg ) );


}



void ConnectionServer::start_echo()
{

    boost::shared_ptr<vrmsg::Vector3> msg ( new vrmsg::Vector3() );
    msg->set_x ( 100 );
    msg->set_y ( 100 );
    msg->set_z ( 100 );
    push_message ( msg );



    echo_timer_ptr->expires_from_now ( boost::posix_time::seconds ( 5 ) );
    echo_timer_ptr->async_wait ( boost::bind ( &ConnectionServer::start_echo, shared_from_this() ) );
}

/************************************************************************/
/* 处理超时                                                                     */
/************************************************************************/
void ConnectionServer::handle_timeout ( const boost::system::error_code& ec, std::string ss )
{
    if ( ec ) return;

    using namespace std;
    cout << ss << " in handle_timeout, socket shutdown" << endl;

    timeout_timer_ptr->cancel();
    //echo_timer_ptr->cancel();

    stop();
    connected = false;
    sending = false;

    server_ptr.lock()->conn_mgr_ptr->handle_timeout ( conn_id );
}



void ConnectionServer::on_client_disconnect()
{
    //protobuf::Message msg;
    //msg.set_str( endpoint );

    //   server_ptr->msg_forwarder->io_s_ptr->post( boost::bind( &MessageForwarder::forward_message, server_ptr->msg_forwarder, msg ) );

    server_ptr.lock()->del_conn ( shared_from_this() ); //call at last
}
