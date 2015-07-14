#include "stdafx.h"
#include "DataManager.h"
#include "DataService.h"
#include "Query.h"
#include "DataObject.h"


CDataManager::CDataManager ( void )
{
}

CDataManager::CDataManager ( boost::shared_ptr<CDataService> p_svc ) :p_service ( p_svc )
{

}


CDataManager::~CDataManager ( void )
{
}

bool CDataManager::remove ( boost::shared_ptr<CDataObject> obj )
{
    return p_service->remove ( obj );
}

bool CDataManager::update ( boost::shared_ptr<CDataObject> obj )
{
    return p_service->update ( obj );
}

bool CDataManager::add ( boost::shared_ptr<CDataObject> obj )
{
    return p_service->add ( obj );
}

bool CDataManager::add( std::vector<boost::shared_ptr<CDataObject>> obj_vec )
{
      return p_service->add ( obj_vec );
}

boost::shared_ptr<CResult> CDataManager::get ( boost::shared_ptr<CQuery> query )
{
    return p_service->get ( query );
}


