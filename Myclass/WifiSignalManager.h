/************************************************************************/
/*  wifisignal data access layer
*/
/************************************************************************/

#ifndef __WIFISIGNAL_MANAGER_H__
#define __WIFISIGNAL_MANAGER_H__

#pragma once
#include "stdafx.h"
#include "datamanager.h"


class CDataObject;
class CDataService;
class CQuery;
class InternalMessage;

class WifiSignalManager : public CDataManager
{
    public:
        WifiSignalManager();
        bool onMsgAdd ( boost::shared_ptr<InternalMessage> in_msg );
        ~WifiSignalManager();

        
};

#endif