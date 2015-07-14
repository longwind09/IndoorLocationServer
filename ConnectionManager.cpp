#include "stdafx.h"
#include "ConnectionManager.h"

#include "utility.h"
#include "server.h"
#include "ConnectionServer.h"

using namespace std;

ConnectionManager::ConnectionManager ( boost::shared_ptr<DServer> svr_ptr )
    : ui_conn_id_ ( "-1" ),
      server_ptr ( svr_ptr ),
      acceptor_ptr (
          new boost::asio::ip::tcp::acceptor (
              * ( svr_ptr->io_service_ptr ),
              boost::asio::ip::tcp::endpoint ( boost::asio::ip::tcp::v4(), 9999 ) ) )
{

}


//////////////////////////////////////////////////////////////////////////
//见handle_verify中关于del_conn的部分
//////////////////////////////////////////////////////////////////////////
void ConnectionManager::del_conn ( boost::shared_ptr<ConnectionServer> conn )
{
    //boost::mutex::scoped_lock lock( endpoint_map_mutex );
    //boost::mutex::scoped_lock lock2( clients_list_mutex );

    //endpoint_map.erase( conn->endpoint );

    //auto iter = std::find( begin( clients_list ), end( clients_list ), conn );
    //if ( iter != clients_list.end() ) clients_list.erase( iter );
}

void ConnectionManager::del_conn ( std::string conn_id )
{
    //boost::mutex::scoped_lock lock( endpoint_map_mutex );
    //boost::mutex::scoped_lock lock2( clients_list_mutex );

    //endpoint_map.erase( conn->endpoint );

    //auto iter = std::find( begin( clients_list ), end( clients_list ), conn );
    //if ( iter != clients_list.end() ) clients_list.erase( iter );
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void ConnectionManager::start_accept()
{
    //新建一个temp client，用作服务器和客户端通信的本地client
    temp_conn.reset ( new ConnectionServer ( server_ptr.lock() ) );
    acceptor_ptr->async_accept ( temp_conn->socket_,
                                 boost::bind ( &ConnectionManager::handle_accept, this, boost::asio::placeholders::error, temp_conn ) );
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
void ConnectionManager::handle_accept ( const boost::system::error_code& error,
                                        boost::shared_ptr<ConnectionServer> conn )
{
    if ( error )
    {
        Utility::global_output ( "accept error:" + error.message() );
        return;
    }

    boost::mutex::scoped_lock lock ( connections_mutex );
    connections_.push_back ( conn );
    lock.unlock();

    boost::mutex::scoped_lock lock2 ( conn_id_map_mutex );
    conn_id_map.insert ( make_pair<string, boost::weak_ptr<ConnectionServer>> ( conn->conn_id, conn ) );

    lock2.unlock();

    string port = boost::lexical_cast<string> ( conn->socket_.remote_endpoint().port() );
    string address = conn->socket_.remote_endpoint().address().to_string();
    string out = "connected with " + address + ": " + port;
    Utility::global_output ( out );
    conn->ip = address;

    conn->connected = true;
    conn->start();

    start_accept();
}
//通过role_id找conn_id,从conn_id_map取conn，conn发消息
void ConnectionManager::unicast_by_role_id ( std::string role_id, boost::shared_ptr<protobuf::Message> msg )
{
    //     boost::mutex::scoped_lock lock( conn_id_map_mutex );
    //
    //     string conn_id = svr_ptr->role_mgr_ptr->get_conn_id_by_role_id( role_id );
    //     if( conn_id != "" )
    //     {
    //         boost::shared_ptr<ConnectionServer> conn = conn_id_map[conn_id].lock();
    //         if( conn ) conn->push_message( msg );
    //     }
}

void ConnectionManager::unicast_msgs ( std::string role_id, std::vector<boost::shared_ptr<protobuf::Message> > msgs )
{
    //     boost::mutex::scoped_lock lock( conn_id_map_mutex );
    //
    //     string conn_id = svr_ptr->role_mgr_ptr->get_conn_id_by_role_id( role_id );
    //     if( conn_id != "" )
    //     {
    //         boost::shared_ptr<ConnectionServer> conn = conn_id_map[conn_id].lock();
    //         if( conn ) conn->push_messages( msgs );
    //     }
}

void ConnectionManager::broadcast ( boost::shared_ptr<protobuf::Message> msg )
{
    boost::mutex::scoped_lock lock ( connections_mutex );

    for ( int i = 0; i < connections_.size(); i++ )
    {
        connections_[i]->push_message ( msg );
    }
}

void ConnectionManager::broadcast_except_by_role_id ( boost::shared_ptr<protobuf::Message> msg, std::string role_id )
{
    //boost::mutex::scoped_lock lock( connections_mutex );

    //string conn_id = svr_ptr->role_mgr_ptr->get_conn_id_by_role_id( role_id );

    //for( int i = 0; i < connections_.size(); i++ )
    //{
    //	if( conn_id != connections_[i]->conn_id )
    //		connections_[i]->push_message( msg );
    //}
}

/************************************************************************/
/* 单个消息                                                                     */
/************************************************************************/
void ConnectionManager::unicast_by_conn_id ( std::string conn_id, boost::shared_ptr<protobuf::Message> msg )
{
    boost::mutex::scoped_lock lock ( conn_id_map_mutex );

    if ( conn_id != "" )
    {
        boost::shared_ptr<ConnectionServer> conn = conn_id_map[conn_id].lock();

        if ( conn ) conn->push_message ( msg );
    }
}
/************************************************************************/
/* 消息数组
 * */
/************************************************************************/
void ConnectionManager::unicast_msgs_by_conn_id ( std::string conn_id, std::vector<boost::shared_ptr<protobuf::Message>> msgs )
{
    boost::mutex::scoped_lock lock ( conn_id_map_mutex );

    if ( conn_id != "" )
    {
        boost::shared_ptr<ConnectionServer> conn = conn_id_map[conn_id].lock();

        if ( conn ) conn->push_messages ( msgs );
    }
}

void ConnectionManager::unicast_to_ui ( boost::shared_ptr<protobuf::Message> msg )
{
    if ( ui_conn_id_ != "-1" )
    {
        std::cout << "unicast to ui:conn_id = " << ui_conn_id_ << endl;
        unicast_by_conn_id ( ui_conn_id_, msg );
    }
}



void ConnectionManager::broadcast_except_by_conn_id ( boost::shared_ptr<protobuf::Message> msg, std::string conn_id )
{
    boost::mutex::scoped_lock lock ( connections_mutex );

    for ( int i = 0; i < connections_.size(); i++ )
    {
        if ( conn_id != connections_[i]->conn_id )
            connections_[i]->push_message ( msg );
    }
}

void ConnectionManager::handle_timeout ( std::string conn_id )
{
    //if( server_ptr.lock()->role_mgr_ptr->conn_id_map.find( conn_id ) != svr_ptr->role_mgr_ptr->conn_id_map.end() )
    //{
    //    string role_id = svr_ptr->role_mgr_ptr->conn_id_map[conn_id];
    //    svr_ptr->role_mgr_ptr->on_cancel_role( conn_id );
    //    svr_ptr->scp_mgr->set_func_npc( role_id );
    //    //del_conn, at last, in case of dangling ptr;
    //}
}

void ConnectionManager::handle_login ( const boost::shared_ptr<protobuf::Message> msg )
{

    //     if ( svr_ptr->version == "Training" )
    //     {
    //         msg_ret.type = msg_type::req_verify;
    //         msg_ret.strings.push_back( msg.conn_id );
    //         msg_ret.strings.push_back( msg.strings[0] );
    //         unicast_to_ui( msg_ret );
    //     }
    //     else
    //     {
    //         msg_ret.type = msg_type::resp_login;
    //         msg_ret.p_ints.push_back( 1 );
    //         unicast_by_conn_id( msg.conn_id, msg_ret );
    //     }
}

void ConnectionManager::handle_verify ( const boost::shared_ptr<protobuf::Message> msg )
{
    //msg_ret.type = msg_type::resp_login;
    //msg_ret.p_ints.push_back( msg.p_ints[0] );
    //std::string conn_id = msg.strings[0];

    //unicast_by_conn_id( conn_id, msg_ret );

    //if ( msg.p_ints[0] == 0 ) // 拒绝连接
    //{
    //    typedef vector<boost::shared_ptr<ConnectionServer> >::iterator SeverConnVecIter;
    //    SeverConnVecIter first = connections_.begin();
    //    SeverConnVecIter last = connections_.end();
    //    SeverConnVecIter result = first;
    //    // 类似于earse(remove_if)用法
    //    while ( first != last )
    //    {
    //        if ( ( *first )->conn_id != conn_id )
    //        {
    //            // 所有server_connection使用的是同一个socket
    //            // 所以不能关闭socket
    //            // (*first)->stop();
    //            *result = *first;
    //            ++result;
    //        }
    //        ++first;
    //    }
    //    // 移除连接和索引
    //    boost::mutex::scoped_lock lock( connections_mutex );
    //    connections_.erase( result, last );
    //    lock.unlock();

    //    boost::mutex::scoped_lock lock2( conn_id_map_mutex );
    //    conn_id_map.erase( conn_id );
    //    lock2.unlock();
    //}
}
