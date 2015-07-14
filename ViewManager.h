/************************************************************************/
/*
*/
/************************************************************************/
#ifndef __CVIEW_MANAGER_H__
#define __CVIEW_MANAGER_H__

#pragma once


#include "stdafx.h"
#include "ViewService.h"
#include "odb/query.hxx"
#include "wifi-odb.hxx"

template<typename T>
class CViewManager
{
    public:
    
        virtual ~CViewManager ( void ) {}
        static boost::shared_ptr<CViewManager<T>> getInstance()
        {
            static boost::shared_ptr<CViewManager<T>> p_mgr ( new CViewManager<T> );
            return p_mgr;
        }
        virtual boost::shared_ptr<std::vector<T>> get ( const odb::query<T>& qry  );
    private:
        boost::shared_ptr<CViewService<T>> p_service;
        CViewManager ( void ) :
            p_service ( boost::shared_ptr<CViewService<T>> ( new CViewService<T> ) )
        {
        
        }
};
template<typename T>
boost::shared_ptr<std::vector<T>> CViewManager<T>::get ( const odb::query<T>& qry )
{
    return p_service->get ( qry );
}
#endif