#include "stdafx.h"
#include "MysqlDataHelper.h"
#include "DBConfig.h"

using namespace std;
MysqlDataHelper::MysqlDataHelper ( mysqlMapType mMap )
{
    for ( mysqlMapType::iterator it = mMap.begin(); it != mMap.end(); ++it )
    {
        sql_conn_map[it->first] = mMap[it->first];
    }
    
    try
    {
        p_driver =  get_driver_instance() ;
        p_conn = boost::shared_ptr<sql::Connection> ( p_driver->connect ( sql_conn_map ) );
        p_conn->setSchema ( DBConfig::mysql_schema );
        p_stmt = boost::shared_ptr<sql::Statement> ( p_conn->createStatement() );
    }
    catch ( sql::SQLException &e )
    {
        manageException ( e );
    }
}

MysqlDataHelper::~MysqlDataHelper ( void )
{
}

MysqlDataHelper& MysqlDataHelper::GetInstance()
{
    static MysqlDataHelper instance ( DBConfig::mysql_cfg_map );
    return instance;
}



boost::shared_ptr<sql::Connection> MysqlDataHelper::GetConnection()
{
    return p_conn;
}

void MysqlDataHelper::manageException ( sql::SQLException &e )
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

void MysqlDataHelper::switchDb ( const string& db_name )
{
    try
    {
        p_conn->setSchema ( db_name );
    }
    catch ( sql::SQLException &e )
    {
        manageException ( e );
    }
    
    try
    {
        p_stmt = boost::shared_ptr<sql::Statement> ( p_conn->createStatement() );
    }
    catch ( sql::SQLException &e )
    {
        manageException ( e );
    }
}

boost::shared_ptr<sql::ResultSet> MysqlDataHelper::excuteQuery ( string query )
{
      boost::shared_ptr<sql::ResultSet> res;
    
    try
    {
        res = boost::shared_ptr<sql::ResultSet> ( p_stmt->executeQuery ( query ) );
    }
    catch ( sql::SQLException &e )
    {
        manageException ( e );
    }
    
    return res;
}
bool MysqlDataHelper::excuteNonQuery ( string query )
{
    bool bResult = false;
    
    try
    {
        bResult =  p_stmt->execute ( query );
    }
    catch ( sql::SQLException &e )
    {
        manageException ( e );
    }
    
    return bResult;
}

int MysqlDataHelper::excuteUpdate ( string query )
{
    int iResult = 0;
    
    try
    {
        iResult =  p_stmt->executeUpdate ( query );
    }
    catch ( sql::SQLException &e )
    {
        manageException ( e );
    }
    
    return iResult;
}
