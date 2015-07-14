#ifndef __ODB_WIFI_SERVICE_H__
#define __ODB_WIFI_SERVICE_H__

#pragma once

#include "stdafx.h"
#include "DataService.h"



class OdbWifiService :public CDataService
    
{
    public:
        OdbWifiService ( void );
        ~OdbWifiService ( void );
        
        virtual bool add ( boost::shared_ptr<CDataObject> obj );

        virtual bool add( std::vector<boost::shared_ptr<CDataObject>> obj_vec );

        virtual bool remove ( boost::shared_ptr<CDataObject> obj );
        virtual bool update ( boost::shared_ptr<CDataObject> obj );
        virtual boost::shared_ptr<CResult> get ( boost::shared_ptr<CQuery> query );
    private :
        boost::shared_ptr<odb::database> db;
};

#endif