#include "stdafx.h"

#include "utility.h"

boost::mutex Utility::output_mutex;
boost::mutex Utility::gen_id_mutex;
boost::mutex Utility::gen_groupID_mutex;
boost::mutex Utility::gen_roleID_mutex;

void Utility::global_output ( const char* str )
{
    boost::mutex::scoped_lock lock ( output_mutex );
    using namespace std;
    cout << str << endl;
}

void Utility::global_output ( const std::string &str )
{
    boost::mutex::scoped_lock lock ( output_mutex );
    using namespace std;
    cout << str << endl;
}

std::string Utility::gen_id()
{
    boost::mutex::scoped_lock lock ( gen_id_mutex );
    static int id = 1;
    return boost::lexical_cast<std::string> ( id++ );
}
std::string Utility::getSytemTime( )
{
    // Get current time from the clock, using microseconds resolution
    //Get today
    //const boost::gregorian::date today = boost::gregorian::day_clock::local_day();
    const boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
    // const boost::posix_time::time_duration td = now.time_of_day();
    cout << now << endl;
    stringstream ss;
    string temp;
    ss << now;
    ss >> temp;
    return temp;
}
int Utility::toInt ( string str )
{
    std::stringstream ss;
    ss << str;
    int iValue;
    ss >> iValue;
    return iValue;
    
}

int Utility::hexStringToInt ( string str )
{
    std::stringstream ss;
    ss << str;
    int iValue;
    ss >> hex >> iValue;
    return iValue;
    
}

int Utility::toInt ( char * buf )
{
    std::stringstream ss;
    string str ( buf );
    return toInt ( str );
}

std::string Utility::toString ( int iValue )
{
    std::stringstream ss;
    ss << iValue;
    string str;
    ss >> str;
    return str;
}

std::string Utility::toString ( int iValue, int width )
{
    std::stringstream ss;
    ss << iValue;
    string str;
    ss >> str;
    
    if ( str.length() < width )
        str.resize ( width );
        
    return str;
}

std::string Utility::toString ( double dValue )
{
    std::stringstream ss;
    ss << dValue;
    string str;
    ss >> str;
    return str;
}

std::string Utility::toHexString ( int iValue )
{

    std::stringstream ss;
    ss << hex << iValue;
    string str;
    ss >> str;
    return str;
}
std::string Utility::toHexString ( int iValue, int width )
{
    std::ostringstream ss;
    
    ss << std::setfill ( '0' ) << std::setw ( width ) << std::hex << iValue;
    
    std::string str;
    str = ss.str();
    /* if( str.length() < width )
         str.resize( width );*/
    return str;
}


std::string Utility::string_To_UTF8 ( const std::string & str )
{
    int nwLen = ::MultiByteToWideChar ( CP_ACP, 0, str.c_str(), -1, NULL, 0 );
    
    wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴
    ZeroMemory ( pwBuf, nwLen * 2 + 2 );
    
    ::MultiByteToWideChar ( CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen );
    
    int nLen = ::WideCharToMultiByte ( CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL );
    
    char * pBuf = new char[nLen + 1];
    ZeroMemory ( pBuf, nLen + 1 );
    
    ::WideCharToMultiByte ( CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL );
    
    std::string retStr ( pBuf );
    
    delete []pwBuf;
    delete []pBuf;
    
    pwBuf = NULL;
    pBuf  = NULL;
    
    return retStr;
}


std::string Utility::gen_groupID()
{
    boost::mutex::scoped_lock lock ( gen_groupID_mutex );
    static int id = 1;
    string groupId = "group_";
    return groupId.append ( boost::lexical_cast<std::string> ( id++ ) );
}

std::string Utility::gen_RoleID()
{
    boost::mutex::scoped_lock lock ( gen_roleID_mutex );
    static int id = 1;
    std::string groupId = "role_";
    return groupId.append ( boost::lexical_cast<std::string> ( id++ ) );
}

string Utility::toString ( const WifiRaw &val )
{
    std::string new_line= "\n";
    std::string str_t =  toString ( val.Id() )+new_line+val.Identification()+new_line+val.Name()+new_line+val.Nowtime()+new_line+ toString ( val.Intensity() )+new_line+val.Pos()+new_line;
    return str_t;
}

std::string Utility::toString ( const WifiData &val )
{
    std::string new_line= "\n";
    std::string tab = "\t";
    std::string pos=val.Pos()+new_line;
    std::string wmap="";
    WifiData::WifiMap _map = val.Wmap();
    
    for ( WifiData::WifiMap::iterator it=_map.begin(); it!=_map.end(); ++it )
    {
        std::string key = it->first;
        std::string value = toString ( it->second );
        wmap+= key+tab+value+new_line;
    }
    
    return pos+wmap;
}
