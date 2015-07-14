#include "stdafx.h"
#include "DBConfig.h"
#include "boost/algorithm/string/trim.hpp"

using namespace std;

map<string, string> DBConfig::mysql_cfg_map = map<string, string>();
string DBConfig::mysql_schema = "";
DBConfig::DBConfig ( void )
{
}


DBConfig::~DBConfig ( void )
{
}

bool DBConfig::loadConfig()
{
    if ( !mysql_cfg_map.empty() ) mysql_cfg_map.clear();
    std::ifstream fs;
    fs.open ( "mysql.cfg", std::ios::in | std::ios::binary );
    if ( !fs )
    {
        cerr << "can not open file mysql.cfg!" << endl;
        return false;
    }
    else
    {
        string line;

        while ( getline ( fs, line ) )
        {
            std::vector<std::string> spvec;
            boost::trim ( line );
            //skip the blank line
            if ( line.length() < 1 ) continue;

            boost::split ( spvec, line, boost::is_any_of ( "=" ), boost::token_compress_on );
            if ( spvec.size() != 2 )
            {
                cerr << "mysql.cfg error " << endl;
                return false;
            }
            else
            {
                if ( spvec[0] == "schema" )
                {
                    mysql_schema = spvec[1];
                }
                mysql_cfg_map[spvec[0]] = spvec[1];

            }
        }

    }
    if ( mysql_schema == "" )
    {
        cerr << "the database schema was not specified!" << endl;
        return false;
    }
    return true;
}
