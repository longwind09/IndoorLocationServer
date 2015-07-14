#include "stdafx.h"
#include "OdbWifiService.h"
#include "DBHelper.h"
#include "wifi.hxx"
#include "wifi-odb.hxx"
#include "WifiResult.h"
#include "WifiQuery.h"
using namespace std;

OdbWifiService::OdbWifiService ( void ) :
    db ( DBHelper::getDbInstance() )
{

}

OdbWifiService::~OdbWifiService ( void )
{
}

bool OdbWifiService::add ( boost::shared_ptr<CDataObject> obj )
{
    try
    {
        odb::transaction t ( db->begin() );
        boost::shared_ptr<OdbWifi> val=boost::dynamic_pointer_cast<OdbWifi> ( obj );
        db->persist ( *val );
        t.commit();
        return true;
    }
    
    catch ( const odb::exception& e )
    {
        cerr << e.what() << endl;
        return false;
    }
}

bool OdbWifiService::add ( std::vector<boost::shared_ptr<CDataObject>> obj_vec )
{
    try
    {
        odb::transaction t ( db->begin() );
        for(int i=0;i<obj_vec.size();++i)
        {
            boost::shared_ptr<OdbWifi> val=boost::dynamic_pointer_cast<OdbWifi> ( obj_vec[i] );
            db->persist ( *val );
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

bool OdbWifiService::remove ( boost::shared_ptr<CDataObject> obj )
{
    try
    {
        odb::transaction t ( db->begin() );
        boost::shared_ptr<OdbWifi> val=boost::dynamic_pointer_cast<OdbWifi> ( obj );
        db->erase ( *val );
        t.commit();
        return true;
    }
    
    catch ( const odb::exception& e )
    {
        cerr << e.what() << endl;
        return false;
    }
}

bool OdbWifiService::update ( boost::shared_ptr<CDataObject> obj )
{
    try
    {
        odb::transaction t ( db->begin() );
        boost::shared_ptr<OdbWifi> val=boost::dynamic_pointer_cast<OdbWifi> ( obj );
        db->update ( *val );
        
        t.commit();
        return true;
    }
    
    catch ( const odb::exception& e )
    {
        cerr << e.what() << endl;
        return false;
    }
}
boost::shared_ptr<CResult> OdbWifiService::get ( boost::shared_ptr<CQuery> query )
{
    boost::shared_ptr<WifiQuery> p_query = boost::dynamic_pointer_cast<WifiQuery> ( query );
    
    try
    {
        odb::transaction t ( db->begin() );
        //用odb操作，用自定义类传递
        boost::shared_ptr<odb::result<OdbWifi>> ptr ( new odb::result<OdbWifi> ( db->query<OdbWifi> ( p_query->query_ ) ) );
        int count = ptr->size();
        boost::shared_ptr<WifiResult> p_result ( new WifiResult ( ptr ) );
        t.commit ();
        return p_result;
        
    }
    
    catch ( const odb::exception& e )
    {
        cerr << e.what() << endl;
        return boost::shared_ptr<WifiResult>();
    }
}
