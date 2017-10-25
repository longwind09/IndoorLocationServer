#ifndef __DB_CONFIG_h__
#define __DB_CONFIG_h__
/************************************************************************/
/*
    临时的全局配置文件，以后用server.cfg配置
*/
/************************************************************************/

#include "stdafx.h"


using namespace std;
#pragma once
class DBConfig
{
    public:
        static bool loadConfig();
        DBConfig ( void );
        ~DBConfig ( void );

        static map<string, string> mysql_cfg_map;
        static string mysql_schema;
};

#endif