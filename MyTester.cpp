#include "stdafx.h"
#include "MyTester.h"
#include "WifiSignalManager.h"

#include "message.h"
#include "geom.h"

#include <ctime>
#include <cstdlib>

#include <odb/query.hxx>
#include <odb/result.hxx>
#include "database.hxx"
#include "wifi-odb.hxx"

#include "server.h"

#include "ObjectManager.h"
#include "KnowledgeSystem.h"
#include "utility.h"

using std::cout;
using std::endl;
MyTester::MyTester ( void )
{
}

MyTester::MyTester ( boost::shared_ptr<DServer> p_server ) :
    svr ( p_server )
{

}


MyTester::~MyTester ( void )
{
}

void MyTester::testAdd()
{
    /*  WifiSignalManager wManager;
    
      for ( int i = 0; i < 10; i++ )
      {
          vrmsg::WifiSignal *msg_signal = new vrmsg::WifiSignal();
          msg_signal->set_id ( string ( "AP_" ) + boost::str ( boost::format ( "%1%" ) % i ) );
          msg_signal->set_intensity ( rand() % 30 );
          geom::vector3 pos ( 3, 4, 3 );
          msg_signal->set_allocated_mypos ( pos.toProtobufVector3() );
          msg_signal->set_device ( "xiaomi_001" );
          boost::shared_ptr<CWifiSignal> cw ( new CWifiSignal ( *msg_signal ) );
          wManager.add ( cw );
      }*/
}

void MyTester::testGet()
{
    svr.lock()->know_sys_ptr->ProcessRawData ( );
  

    boost::shared_ptr<std::vector<WifiData>> p_res = CObjectManager<WifiData>::getInstance()->get ( odb::query<WifiData>());
    
    for ( std::vector<WifiData>::iterator it=p_res->begin(); it!=p_res->end(); ++it )
    {
        cout<<Utility::toString ( *it );
    }
    
}
void MyTester::Vector2File ( array2_type ar2,std::string filename )
{
    /*ofstream fs(filename);
    
    for ( array2_type::iterator it=ar2.begin(); it!=ar2.end(); ++it )
    {
        for (std::vector<int>::iterator jt = it->begin();jt!=it->end();++jt )
        {
            fs<<*jt<<"\t";
        }
    
    }
    fs.close();*/
}
