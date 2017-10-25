#ifndef __DATAHELPER_H_
#define __DATAHELPER_H_

#pragma once

#include "stdafx.h"


class MysqlDataHelper
{
    public:
    
        typedef std::map<std::string, std::string> mysqlMapType;
        sql::ConnectOptionsMap sql_conn_map;
        static MysqlDataHelper& GetInstance();
        
        
        
        boost::shared_ptr<sql::ResultSet> excuteQuery ( std::string query );
        bool excuteNonQuery ( std::string query );
        int excuteUpdate ( std::string query );
        
    private:
    
        //not thread safe
        boost::shared_ptr<sql::Connection> GetConnection();
        void switchDb ( const std::string& db_name );
        void manageException ( sql::SQLException &e );
        
        ~MysqlDataHelper ( void );
        sql::Driver *p_driver;
        
        //for prepare_statement£¬not thread safe
        boost::shared_ptr<sql::Connection> p_conn;
        boost::shared_ptr<sql::Statement> p_stmt;
        
        
    protected:
        MysqlDataHelper ( mysqlMapType mMap );
        
};

#endif