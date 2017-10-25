/************************************************************************/
/*    通信管理者，应用层通信的高层
 *                                                                      */
/************************************************************************/

#ifndef __ConnectionManager_H__
#define __ConnectionManager_H__


#include "stdafx.h"
//#include "message.h"


class ConnectionServer;
class DServer;

using namespace google;

class ConnectionManager : public boost::enable_shared_from_this<ConnectionManager>
{
    public:
        ConnectionManager ( boost::shared_ptr<DServer> s_ptr );

        void output_error ( std::string err_msg, const boost::system::error_code &e );

        void start_accept();
        void handle_accept ( const boost::system::error_code& error, boost::shared_ptr<ConnectionServer> client_ptr );

        //ordered broadcast, unicast
        //conn_id,before select role
        //role_id, after select role
        //default by role_id, entities发来的消息一般role_id, 或broadcast
        // 单播
        void unicast_by_role_id ( std::string role_id, boost::shared_ptr<protobuf::Message> msg );
        void unicast_by_conn_id ( std::string conn_id, boost::shared_ptr<protobuf::Message> msg );
        void unicast_to_ui ( boost::shared_ptr<protobuf::Message> msg );
        void unicast_msgs ( std::string role_id, std::vector<boost::shared_ptr<protobuf::Message>> msgs );
        void unicast_msgs_by_conn_id ( std::string conn_id, std::vector<boost::shared_ptr<protobuf::Message>> msgs );


        //广播
        void broadcast ( boost::shared_ptr<protobuf::Message> msg );
        void broadcast_msgs ( std::vector<protobuf::Message> msgs );

        void broadcast_except_by_role_id ( boost::shared_ptr<protobuf::Message> msg, std::string role_id );
        void broadcast_except_by_conn_id ( boost::shared_ptr<protobuf::Message> msg, std::string conn_id );
        void broadcast_msgs_except ( std::vector<protobuf::Message> msgs, std::string role_id );
        void broadcast_msgs_except_by_conn_id ( std::vector<protobuf::Message> msgs, std::string conn_id );

        void handle_timeout ( std::string role_id );

        // 处理客户端的的登录请求
        // 若是演练版，则将登录请求发送到UI进行验证
        // 若是其他版本，则直接告知客户端登录成功
        void handle_login ( const boost::shared_ptr<protobuf::Message> msg );

        // 处理UI对客户端的验证
        // 将验证结果告知客户端（成功/失败）
        // 若成功则保持连接，若失败则断开连接并从connection_中移除
        void handle_verify ( const boost::shared_ptr<protobuf::Message> msg );

        void del_conn ( std::string conn_id );
        void del_conn ( boost::shared_ptr<ConnectionServer> conn );
        //private:

        std::vector<boost::shared_ptr<ConnectionServer>> connections_;
        boost::mutex connections_mutex;

        //accept conn
        std::map<std::string, boost::weak_ptr<ConnectionServer>> conn_id_map;//conn_id -> conn_ptr;
        boost::mutex conn_id_map_mutex;

        boost::weak_ptr<DServer> server_ptr;
        boost::shared_ptr<ConnectionServer> temp_conn;
        boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_ptr;

        std::string ui_conn_id_;

        //connections_ 连接的集合
        //conn_id_map 这二者有什么区别吗？<map的作用是效率考虑，查找更快>


};

#endif