#include "stdafx.h"
#include "EntityMessageAdapter.h"
#include "utility.h"
#include "message.h"
#include "wifi.hxx"
using std::string;

EntityMessageAdapter::EntityMessageAdapter ( void )
{
}


EntityMessageAdapter::~EntityMessageAdapter ( void )
{
}

boost::shared_ptr<WifiRaw> EntityMessageAdapter::toWifiRaw ( const vrmsg::WifiSignal &val )
{
    boost::shared_ptr<WifiRaw> wifi ( new WifiRaw );
    wifi->Identification ( val.id() );
    wifi->Intensity ( val.intensity() );
    wifi->Pos ( Vector3toString ( val.mypos() ) );
    wifi->Name ( val.name() );
    wifi->Device ( val.device() );
    wifi->Nowtime ( val.nowtime() );
    return wifi;
}

//boost::shared_ptr<OdbWifi> EntityMessageAdapter::toMsgWifi ( boost::shared_ptr<OdbWifi> val )
//{
//
//}

string EntityMessageAdapter::Vector3toString ( const vrmsg::Vector3 &val )
{
    string comma = ",";
    string sx = boost::lexical_cast<string> ( val.x() );
    string sy = boost::lexical_cast<string> ( val.y() );
    string sz = boost::lexical_cast<string> ( val.z() );
    return sx + comma + sy + comma + sz;
}

vrmsg::Vector3* EntityMessageAdapter::StringToVector3 ( const std::string &val )
{
    std::vector<std::string> spvec;
    boost::split ( spvec, val, boost::is_any_of ( ", " ), boost::token_compress_on );
    
    if ( spvec.size() != 3 ) return NULL;
    
    vrmsg::Vector3 *v3 = new vrmsg::Vector3();
    double x,y,z;
    
    try
    {
        std::vector<std::string>::iterator iter = spvec.begin();
        x = boost::lexical_cast<double> ( *iter++ );
        y = boost::lexical_cast<double> ( *iter++ );
        z = boost::lexical_cast<double> ( *iter );
    }
    
    catch ( const boost::bad_lexical_cast & e )
    {
        std::cout << "can not cast to double from string!";
    }
    
    v3->set_x ( x );
    v3->set_y ( y );
    v3->set_z ( z );
    return v3;
}

//boost::shared_ptr<WifiQuery> EntityMessageAdapter::toWifiQuery( const vrmsg::WifiSignal &val )
//{
//    boost::shared_ptr<WifiQuery> wifi ( new WifiQuery );
//    wifi->Identification ( val.id() );
//    wifi->Intensity ( val.intensity() );
//    wifi->Pos ( Vector3toString ( val.mypos() ) );
//    wifi->Name ( val.name() );
//    wifi->Device ( val.device() );
//    wifi->Nowtime ( val.nowtime() );
//    return wifi;
//}


