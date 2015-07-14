/************************************************************************/
/*
          数据实体 manager base
*/
/************************************************************************/

#ifndef __CDATA_MANAGER_H__
#define __CDATA_MANAGER_H__


#pragma once

class CDataObject;
class CDataService;
class CQuery;
class CResult;

class CDataManager

{
    public:
        CDataManager ( void );
        CDataManager ( boost::shared_ptr<CDataService> p_svc );
        virtual ~CDataManager ( void );
        
        boost::shared_ptr<CDataService> p_service ;
        virtual bool add ( boost::shared_ptr<CDataObject> obj );
        virtual bool add ( std::vector<boost::shared_ptr<CDataObject>> obj_vec );
        virtual bool update ( boost::shared_ptr<CDataObject> obj );
        virtual bool remove ( boost::shared_ptr<CDataObject> obj );
        virtual boost::shared_ptr<CResult> get ( boost::shared_ptr<CQuery> query );
};
#endif