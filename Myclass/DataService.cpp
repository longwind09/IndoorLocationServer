#include "stdafx.h"
#include "DataService.h"
#include "DataObject.h"
#include "Query.h"
#include "Result.h"


bool CDataService::remove ( boost::shared_ptr<CDataObject> obj )
{
    return false;
}

bool CDataService::update ( boost::shared_ptr<CDataObject> obj )
{
    return false;
}

bool CDataService::add ( boost::shared_ptr<CDataObject> obj )
{
    return false;
}

bool CDataService::add( std::vector<boost::shared_ptr<CDataObject>> obj_vec )
{
    return false;
}

 boost::shared_ptr<CResult> CDataService::get ( boost::shared_ptr<CQuery> query )
{
    return  boost::shared_ptr<CResult>();
 }


