/************************************************************************/
/*
    数据库视图功能实现类
*/
/************************************************************************/

#ifndef __CVIEWSERVICE_H__
#define __CVIEWSERVICE_H__

#pragma once
#include "stdafx.h"
#include "DBHelper.h"
#include "odb/query.hxx"
#include "wifi-odb.hxx"
#include "wifi.hxx"
template<typename T>
class CViewService
{
    public:
        CViewService ( void ) :
            db ( DBHelper::getDbInstance() )
        {
        
        }
        virtual ~CViewService ( void ) {}
        
        virtual boost::shared_ptr<std::vector<T>> get ( const odb::query<T>& qry  );
        
    private:
        boost::shared_ptr<odb::database> db;
};

template<typename T>
boost::shared_ptr<std::vector<T>> CViewService<T>::get ( const odb::query<T>& qry )
{
    try
    {
        odb::transaction t ( db->begin() );
        odb::result<T> res ( db->query<T> ( qry ) );
        boost::shared_ptr<std::vector<T>> t_vec ( new std::vector<T>() );
        
        for ( odb::result<T>::iterator it=res.begin(); it!=res.end(); ++it )
        {
            t_vec->push_back ( *it );
        }
        
        t.commit ();
        return t_vec;
    }
    
    catch ( const odb::exception& e )
    {
        cerr << e.what() << endl;
        return boost::shared_ptr<std::vector<T>>();
    }
}

#endif