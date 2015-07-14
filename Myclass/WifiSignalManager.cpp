#include "stdafx.h"
#include "WifiSignalManager.h"
#include "message.h"
#include "InternalMessage.h"

#include "OdbWifiService.h"

#include "wifi.hxx"
#include "EntityMessageAdapter.h"



WifiSignalManager::WifiSignalManager ( void ) :
    CDataManager ( boost::shared_ptr<OdbWifiService> ( new OdbWifiService ) )
{
}



bool WifiSignalManager::onMsgAdd ( boost::shared_ptr<InternalMessage> in_msg )
{

    boost::shared_ptr<vrmsg::AddWifiSignalRequest> msg = boost::dynamic_pointer_cast<vrmsg::AddWifiSignalRequest> ( in_msg->message );
    
    for ( int i = 0; i < msg->signals().size(); ++i )
    {
        boost::shared_ptr<OdbWifi> signal ( EntityMessageAdapter::toOdbWifi ( msg->signals ( i ) ) ) ;
        add ( signal ); 
    }
    
    return true;
}

WifiSignalManager::~WifiSignalManager()
{

}


