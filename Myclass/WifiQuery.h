/************************************************************************/
/*
*/
/************************************************************************/
#ifndef __WIFI_QUERY_H__
#define __WIFI_QUERY_H__

#pragma once
#include "Query.h"
#include <odb/database.hxx>
#include <odb/query.hxx>
#include "wifi.hxx"


class WifiQuery : public CQuery
{

    public:
        typedef odb::query<OdbWifi> wifi_query_type;
        WifiQuery ( boost::shared_ptr<wifi_query_type> qry )
            : query_ ( qry )
        {
        
        }
        
        WifiQuery ()
        {
            boost::shared_ptr<wifi_query_type> qry ( new wifi_query_type() );
            query_ = qry;
        }
        
        ~WifiQuery ( void ) {}
        boost::shared_ptr<wifi_query_type> query_;
};

#endif