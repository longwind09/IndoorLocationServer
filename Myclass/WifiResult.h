/************************************************************************/
/*
*/
/************************************************************************/
#ifndef _CWIFI_RESULT_H__
#define _CWIFI_RESULT_H__



#pragma once
#include "result.h"
#include <odb/database.hxx>
#include <odb/result.hxx>
#include "wifi.hxx"
#include "stdafx.h"

class WifiResult : public CResult

{
    typedef  odb::result<OdbWifi> wifi_result_type;
    public:
        WifiResult ( boost::shared_ptr<wifi_result_type> p_re ) 
        {
            result_ = p_re;
        }
        WifiResult ( void ) {}
        ~WifiResult ( void ) {}
        boost::shared_ptr<wifi_result_type> result_;
};

#endif