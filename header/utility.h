#ifndef __UTILITY_H__
#define __UTILITY_H__

#pragma  once

#include "iostream"
#include "message.h"
#include "odbdata.h"
using namespace std;



class Utility
{
    public:
        static void global_output ( const char* str );
        static void global_output ( const std::string &str );
        static std::string gen_id();//id, from "1"
        static std::string gen_groupID();//id, from "1"
        static std::string gen_RoleID();//id, from "1"
        static std::string getSytemTime( );
        static std::string string_To_UTF8 ( const std::string & str );
        static std::string toHexString ( int iValue, int width );
        static std::string toHexString ( int iValue );
        static std::string toString ( int iValue, int width );
        static std::string toString ( int iValue );
        static std::string toString ( double dValue );
        static int toInt ( char * buf );
        static int toInt ( string str );
        static int hexStringToInt ( string str );
        
        static std::string toString ( const WifiRaw &val );
        static std::string toString ( const WifiData &val );
        
        
    private:
        static boost::mutex output_mutex;
        static boost::mutex gen_id_mutex;
        static boost::mutex Utility::gen_groupID_mutex;
        static boost::mutex Utility::gen_roleID_mutex;
        
};

#endif
