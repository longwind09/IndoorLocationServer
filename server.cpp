#include "stdafx.h"

#include "server.h"
#include "BootstrapServer.h"
#include "ConnectionServer.h"
#include "ConnectionManager.h"
#include "MessageForwarder.h"
#include "MessageHandler.h"

//#include "timer_scheduler.h"

#include "log.h"

//#include "operation_loader.h"
//#include "config_file.h"
//#include "datasource_manager.h"
//#include "configuration.h"

#include "DBConfig.h"
#include "MysqlDataHelper.h"

#include "MyTester.h"
#include "WifiSignalManager.h"
#include "KnowledgeSystem.h"
#include "ServerCommand.h"


using namespace std;

DServer::DServer()
    : io_service_ptr ( new boost::asio::io_service() ),
      bootp_service_ptr ( new boost::asio::io_service() ),
      log ( Log::getInstance() )
{
}

void DServer::init ( const std::string& work_path,
                     const std::string& version,
                     const std::string& drill )
{
    if ( !DBConfig::loadConfig() ) return ;
    
    //// version: Demo, Study or Training.
    //this->version = version;
    //ConfigFile cfg;
    ////载入server.cfg到cfg
    //string seperator = string ( "\t:= " );
    //cfg.Load ( work_path + "server.cfg", seperator );
    ////从cfg获取对应版本的配置信息，drill is north or south
    //ConfigFile::SettingsMultiMap* data_source_settings = cfg.GetSettingsMultiMap ( version + " " + drill );
    //ds_manager = DataSourceManager::GetSington();
    //
    ////如果单例获取成功就加载数据，为什么要用单例模式？其他的为什么不用？
    //if ( data_source_settings != nullptr )
    //{
    //    //把相对路径写成绝对路径
    //    ds_manager->SetDataSourcePath ( work_path, data_source_settings );
    //}
    //
    ////加载文件,opinfo.json,questions.xml,stdops.json
    //ds_manager->LoadDataSource();
    //config.configure(path + "\\drill\\config.xml", "training.lua");
    //读入了实体信息和状态信息，脚本没提
    //config.configure ( ds_manager->GetFileLocation ( DataSourceManager::kEntityCfg ),
    //                   "training.lua" );
    // config.configure ( ds_manager->GetFileLocation ( DataSourceManager::kEntityCfg ),"without_lua" );
    conn_mgr_ptr = boost::make_shared<ConnectionManager> ( shared_from_this() );
    wifi_mgr_ptr = boost::shared_ptr<WifiSignalManager> ( new WifiSignalManager );
    know_sys_ptr = boost::make_shared<KnowledgeSystem> ( shared_from_this() );

    msg_handler_ptr = boost::make_shared<MessageHandler> ( shared_from_this() );
    msg_forwarder = boost::make_shared<MessageForwarder> ( shared_from_this() );
    msg_forwarder->registerHandler ( *msg_handler_ptr );
    
    
    //log test
    /*  Log myLog = Log::getInstance();
    myLog.pushNDC ( "Main Thread" );*/
    
    /* for ( int i = 0; i < 1000; i++ )
         myLog.Debug ( "This is a DEBUG Message" );
    
     myLog.popNDC();
     myLog.removeNDC();*/
    bootp_server_ptr = boost::make_shared<BootstrapServer> ( *bootp_service_ptr );
    bootp_server_ptr->start_service();
    boost::thread t ( boost::bind ( &boost::asio::io_service::run, bootp_service_ptr ) );
    
    //cmd_ptr = boost::make_shared<ServerCommand> ( shared_from_this() );
    //cmd_ptr->run();
    

    know_sys_ptr->ProcessRawData();
    know_sys_ptr->showWifiData();
    know_sys_ptr->init();
    
    cout << "Server start" << endl << "version: " << version << endl << "drill: " << drill  << endl;
}

void DServer::load_scripts()
{
}

void DServer::output_error ( std::string err_msg, const boost::system::error_code &e )
{
    cerr << err_msg << ", error message: " << e.message() << endl;
}

void DServer::start_service()
{
    conn_mgr_ptr->start_accept();
    boost::asio::io_service::work work ( *io_service_ptr );
    
    // all threads that have joined an io_service's pool are considered equivalent,
    // and the io_service may distribute work across them in an arbitrary fashion
    for ( int i = 0; i < THREADS_NUM; i++ )
    {
        tg.create_thread ( boost::bind ( &boost::asio::io_service::run, io_service_ptr ) );
    }
    
    tg.join_all();
}



std::string DServer::gen_id()
{
    boost::mutex::scoped_lock lock ( gen_id_mutex );
    static int id = 1;
    return boost::lexical_cast<std::string> ( id++ );
}



void DServer::del_conn ( boost::shared_ptr<ConnectionServer> conn )
{
    boost::mutex::scoped_lock lock ( endpoint_map_mutex );
    boost::mutex::scoped_lock lock2 ( clients_list_mutex );
    endpoint_map.erase ( conn->endpoint );
    auto iter = std::find ( begin ( clients_list ), end ( clients_list ), conn );
    
    if ( iter != clients_list.end() ) clients_list.erase ( iter );
}



void DServer::handle_timeout ( std::string role_id )
{
    // scp_mgr->set_func_npc ( role_id );
}

void DServer::startTraining()
{

}

void DServer::__DataTest()
{
    MyTester *test = new MyTester ( shared_from_this() );
    // test->testAdd();
    test->testGet();
    delete test;
    test = NULL;
}

void DServer::exitServer()
{
    cout << "bye" << endl;
    exit ( 0 );
}

void DServer::runCmd()
{
    cmd_ptr->run();
}
