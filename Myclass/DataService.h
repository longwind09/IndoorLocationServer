/************************************************************************/
/*

*/
/************************************************************************/
#ifndef __CDATASERVICE_H__
#define __CDATASERVICE_H__

#pragma once
#include "stdafx.h"

class CDataObject;
class CDataService;
class CResult;
class CQuery;

class CDataService
{
    public:
        CDataService ( void ) {}
        virtual ~CDataService ( void ) {}
        
        virtual bool add ( boost::shared_ptr<CDataObject> obj );
        virtual bool add ( std::vector<boost::shared_ptr<CDataObject>> obj_vec );
        virtual bool update ( boost::shared_ptr<CDataObject> obj );
        virtual bool remove ( boost::shared_ptr<CDataObject> obj );
        virtual boost::shared_ptr<CResult> get ( boost::shared_ptr<CQuery> query );
        
};

#endif