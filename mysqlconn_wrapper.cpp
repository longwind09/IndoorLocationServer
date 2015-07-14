/*
 * File:   mysqlconn_wrapper.h
 * Author: Eduardo Casas (www.eduardocasas.com)
 *
 * Created on February 24, 2013, 5:07 PM
 *
 * This is a wrapper class for the MySQL Connector/C++ Library
 * see: http://dev.mysql.com/doc/connector-cpp/en/index.html
 */
#include "stdafx.h"


#include "mysqlconn_wrapper.h"
#include "DBConfig.h"
using namespace std;

MySQLConnWrapper::~MySQLConnWrapper()
{
    delete res;
    delete prep_stmt;
    delete stmt;
    delete con;
}
MySQLConnWrapper::MySQLConnWrapper()
{
    host     = DBConfig::mysql_cfg_map["hostName"];
    user     = DBConfig::mysql_cfg_map["userName"];
    password = DBConfig::mysql_cfg_map["password"];
}

void MySQLConnWrapper::manageException ( sql::SQLException& e )
{
    if ( e.getErrorCode() != 0 )
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

void MySQLConnWrapper::connect()
{
    try
    {
        driver = get_driver_instance();
    }
    catch ( sql::SQLException &e )
    {
        manageException ( e );
    }
    try
    {
        con = driver->connect ( host, user, password );
    }
    catch ( sql::SQLException &e )
    {
        manageException ( e );
    }
}

void MySQLConnWrapper::switchDb ( const string& db_name )
{
    try
    {
        con->setSchema ( db_name );
    }
    catch ( sql::SQLException &e )
    {
        manageException ( e );
    }
    try
    {
        stmt = con->createStatement();
    }
    catch ( sql::SQLException &e )
    {
        manageException ( e );
    }
}

void MySQLConnWrapper::prepare ( const string& query )
{
    try
    {
        prep_stmt = con->prepareStatement ( query );
    }
    catch ( sql::SQLException &e )
    {
        manageException ( e );
    }
}

void MySQLConnWrapper::setInt ( const int& num, const int& data )
{
    prep_stmt->setInt ( num, data );
}

void MySQLConnWrapper::setString ( const int& num, const string& data )
{
    prep_stmt->setString ( num, data );
}

void MySQLConnWrapper::execute ( const string& query )
{
    try
    {
        res = ( query != "" ) ? stmt->executeQuery ( query ) : prep_stmt->executeQuery();
    }
    catch ( sql::SQLException &e )
    {
        manageException ( e );
    }
}

bool MySQLConnWrapper::fetch()
{
    return res->next();
}

string MySQLConnWrapper::print ( const string& field )
{
    return res->getString ( field );
}

string MySQLConnWrapper::print ( const int& index )
{
    return res->getString ( index );
}
