/************************************************************************/
/*
*/
/************************************************************************/
#ifndef __COBJECT_SERVICE_H__
#define __COBJECT_SERVICE_H__


#pragma once

#include "stdafx.h"
#include "DBHelper.h"
#include "odb/query.hxx"
#include "wifi-odb.hxx"
#include "wifi.hxx"
template<typename T>
class CObjectService
{
    public:
        CObjectService ( void ) :
            db ( DBHelper::getDbInstance() )
        {
        
        }
        virtual ~CObjectService ( void ) {}
        
        
        virtual  bool add ( boost::shared_ptr<T> obj );
        //add and return it's id
        virtual unsigned long persist ( boost::shared_ptr<T> obj );
        
        virtual bool addVec ( boost::shared_ptr<std::vector<boost::shared_ptr<T>>> obj_vec );
        
        virtual bool remove ( boost::shared_ptr<T> obj );
        
        virtual bool update ( boost::shared_ptr<T> obj );
        
        virtual bool clear();
        
        virtual boost::shared_ptr<std::vector<T>> get ( const odb::query<T>& qry  );
        
    private:
        boost::shared_ptr<odb::database> db;
};

template<typename T>
bool CObjectService<T>::clear()
{
    try
    {
        odb::transaction t ( db->begin() );
        unsigned long long count = ( unsigned long long) ( db->erase_query<T> () );
        t.commit();
        return ( count > 0 ? true : false );
    }
    
    catch ( const odb::exception& e )
    {
        cerr << e.what() << endl;
        return false;
    }
}

template<typename T>
unsigned long CObjectService<T>::persist ( boost::shared_ptr<T> obj )
{
    try
    {
        odb::transaction t ( db->begin() );
        unsigned long my_id = ( unsigned long ) ( db->persist ( *obj ) );
        t.commit();
        return my_id;
    }
    
    catch ( const odb::exception& e )
    {
        cerr << e.what() << endl;
        return false;
    }
}

template<typename T>
boost::shared_ptr<std::vector<T>> CObjectService<T>::get ( const odb::query<T>& qry )
{
    try
    {
        odb::transaction t ( db->begin() );
        odb::result<T> res ( db->query<T> ( qry ) );
        boost::shared_ptr<std::vector<T>> t_vec ( new std::vector<T>() );
        
        for ( odb::result<T>::iterator it = res.begin(); it != res.end(); ++it )
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

template<typename T>
bool CObjectService<T>::update ( boost::shared_ptr<T> obj )
{
    try
    {
        odb::transaction t ( db->begin() );
        db->update ( *obj );
        
        t.commit();
        return true;
    }
    
    catch ( const odb::exception& e )
    {
        cerr << e.what() << endl;
        return false;
    }
}

template<typename T>
bool CObjectService<T>::remove ( boost::shared_ptr<T> obj )
{
    try
    {
        odb::transaction t ( db->begin() );
        
        db->erase ( *obj );
        t.commit();
        return true;
    }
    
    catch ( const odb::exception& e )
    {
        cerr << e.what() << endl;
        return false;
    }
}

template<typename T>
bool CObjectService<T>::addVec ( boost::shared_ptr<std::vector<boost::shared_ptr<T>>> obj_vec )
{
    try
    {
        odb::transaction t ( db->begin() );
        
        for ( int i = 0; i < obj_vec->size(); ++i )
        {
            db->persist ( * ( *obj_vec ) [i] );
        }
        
        t.commit();
        return true;
    }
    
    catch ( const odb::exception& e )
    {
        cerr << e.what() << endl;
        return false;
    }
}

template<typename T>
bool CObjectService<T>::add ( boost::shared_ptr<T> obj )
{
    try
    {
        odb::transaction t ( db->begin() );
        db->persist ( *obj );
        t.commit();
        return true;
    }
    
    catch ( const odb::exception& e )
    {
        cerr << e.what() << endl;
        return false;
    }
}

#endif