/************************************************************************/
/*  wifisignal data access layer
*/
/************************************************************************/

#ifndef __WIFISIGNAL_MANAGER_H__
#define __WIFISIGNAL_MANAGER_H__

#pragma once
#include "stdafx.h"

class InternalMessage;
class WifiSignalManager
{
    public:
        WifiSignalManager();
        bool onMsgAdd ( boost::shared_ptr<InternalMessage> in_msg );
        ~WifiSignalManager();
        
};

#endif