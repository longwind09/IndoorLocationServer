/************************************************************************/
/*

*/
/************************************************************************/
#ifndef __COBJECT_MANAGER_H__
#define __COBJECT_MANAGER_H__


#pragma once

#include "stdafx.h"
#include "ObjectService.h"
#include "odb/query.hxx"
#include "wifi-odb.hxx"
template<typename T>
class CObjectManager
{
    public:
        virtual ~CObjectManager ( void ) {}
        static boost::shared_ptr<CObjectManager<T>> getInstance()
        {
            static boost::shared_ptr<CObjectManager<T>> p_mgr ( new CObjectManager<T> );
            return p_mgr;
        }
        
        virtual bool add ( boost::shared_ptr<T> obj );
        
        //add and return it's id
        virtual unsigned long persist ( boost::shared_ptr<T> obj );
        
        virtual bool addVec ( boost::shared_ptr<std::vector<boost::shared_ptr<T>>> obj_vec );
        
        virtual bool remove ( boost::shared_ptr<T> obj );
        
        virtual bool update ( boost::shared_ptr<T> obj );
        
        virtual bool clear();
        
        virtual boost::shared_ptr<std::vector<T>> get ( const odb::query<T>& qry  );
    private:
        boost::shared_ptr<CObjectService<T>> p_service;
        CObjectManager ( void ) :
            p_service ( boost::shared_ptr<CObjectService<T>> ( new CObjectService<T> ) )
        {
        
        }
};

template<typename T>
bool CObjectManager<T>::clear()
{
    return p_service->clear();
}

template<typename T>
unsigned long CObjectManager<T>::persist ( boost::shared_ptr<T> obj )
{
    return p_service->persist ( obj );
}


template<typename T>
boost::shared_ptr<std::vector<T>> CObjectManager<T>::get ( const odb::query<T>& qry )
{
    return p_service->get ( qry );
}

template<typename T>
bool CObjectManager<T>::update ( boost::shared_ptr<T> obj )
{
    return p_service->update ( obj );
}

template<typename T>
bool CObjectManager<T>::remove ( boost::shared_ptr<T> obj )
{
    return p_service->remove ( obj );
}

template<typename T>
bool CObjectManager<T>::addVec ( boost::shared_ptr<std::vector<boost::shared_ptr<T>>> obj_vec )
{
    return p_service->addVec ( obj_vec );
}

template<typename T>
bool CObjectManager<T>::add ( boost::shared_ptr<T> obj )
{
    return p_service->add ( obj );
}


#endif