#include "stdafx.h"
#include "DBHelper.h"
#include "database.hxx"
#include "DBConfig.h"
using namespace std;

DBHelper::DBHelper ( void )
{
}


DBHelper::~DBHelper ( void )
{
}

boost::shared_ptr<odb::database> DBHelper::getDbInstance()
{
    static boost::shared_ptr<odb::database> db = generateDB();
    return db;
}

boost::shared_ptr<odb::database> DBHelper::generateDB()
{

    string user = DBConfig::mysql_cfg_map["user"];
    string password = DBConfig::mysql_cfg_map["password"];
    string schema = DBConfig::mysql_cfg_map["schema"];
    string host = DBConfig::mysql_cfg_map["host"];
    
    try
    {
        static boost::shared_ptr<odb::database> db ( new odb::mysql::database ( user, password, schema, host ) );
        return db;
    }
    catch ( const odb::exception& e )
    {
        cerr << e.what () << endl;
        return  boost::shared_ptr<odb::database> ( NULL );
    }
}

void DBHelper::switchDb ( const std::string& db_name )
{

}

