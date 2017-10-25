#include "stdafx.h"
#include "WifiSignalManager.h"
#include "message.h"
#include "InternalMessage.h"

#include "ObjectManager.h"

#include "wifi.hxx"
#include "EntityMessageAdapter.h"
#include "wifi-odb.hxx"



WifiSignalManager::WifiSignalManager ( void )
{
}



bool WifiSignalManager::onMsgAdd ( boost::shared_ptr<InternalMessage> in_msg )
{
    //有信号来，则5分钟更新一次数据库，没有的话则不更新。在更新数据库之前，如果有效数据量太少则不更新，类似于众包

    boost::shared_ptr<vrmsg::AddWifiSignalRequest> msg = boost::dynamic_pointer_cast<vrmsg::AddWifiSignalRequest> ( in_msg->message );
    
    for ( int i = 0; i < msg->signals().size(); ++i )
    {
        boost::shared_ptr<WifiRaw> signal ( EntityMessageAdapter::toWifiRaw ( msg->signals ( i ) ) ) ;
        unsigned long id = CObjectManager<WifiRaw>::getInstance()->persist ( signal );
        boost::shared_ptr<WifiSample> sample ( new WifiSample ( id ) );
        CObjectManager<WifiSample>::getInstance()->add ( sample );
    }
    
    return true;
}

WifiSignalManager::~WifiSignalManager()
{

}


