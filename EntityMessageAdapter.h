/************************************************************************/
/*
        实体和消息的适配器，提供互相转换的接口
*/
/************************************************************************/
#ifndef __ENTITY_MESSAGE_ADAPTER_H__
#define __ENTITY_MESSAGE_ADAPTER_H__
#pragma once

#include "stdafx.h"

class WifiRaw;
class WifiQuery;
namespace vrmsg
{
    class WifiSignal;
    class Vector3;
}


class EntityMessageAdapter
{
    public:
        static boost::shared_ptr<WifiRaw> toWifiRaw ( const vrmsg::WifiSignal &val );
        //static boost::shared_ptr<WifiQuery> toWifiQuery ( const vrmsg::WifiSignal &val );
        
        //static boost::shared_ptr<OdbWifi> toMsgWifi ( const OdbWifi &val );
        static std::string  Vector3toString ( const vrmsg::Vector3 &val );
        static vrmsg::Vector3*  StringToVector3 ( const std::string &val );
        EntityMessageAdapter ( void );
        ~EntityMessageAdapter ( void );
};

#endif