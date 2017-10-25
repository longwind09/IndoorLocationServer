#include "stdafx.h"
#include "KnowledgeSystem.h"
#include "server.h"
#include "InternalMessage.h"
#include "ConnectionManager.h"
#include "WifiSignalManager.h"

#include "ObjectManager.h"
#include "ViewManager.h"

#include "odbdata.h"

#include "adaptor.h"
#include "utility.h"

using namespace std;
KnowledgeSystem::KnowledgeSystem ( boost::shared_ptr<DServer> svr_ptr ) :
    server_ptr ( svr_ptr )
{
}


KnowledgeSystem::~KnowledgeSystem ( void )
{
}

void KnowledgeSystem::onQueryPosition ( boost::shared_ptr<InternalMessage> msg )
{
    //**********************************
    boost::shared_ptr<vrmsg::QueryPostionRequest> query_request  = boost::dynamic_pointer_cast<vrmsg::QueryPostionRequest> ( msg->message );
    
    cout << query_request->DebugString() << endl;
    
    if ( query_request->signals().size() < 3 ) return;
    

     vrmsg::Vector3* v_true = new vrmsg::Vector3();
    //init the  query map
    std::map<std::string, double> query_map;
    
    for ( int i = 0; i < query_request->signals().size(); ++i )
    {
        std::string id = query_request->signals ( i ).id();
        *v_true = query_request->signals(i).mypos();
        int tensity = query_request->signals ( i ).intensity();
        query_map[id] = tensity + 100;
    }
    boost::shared_ptr<WifiQuery> signal ( new WifiQuery)  ;
    signal->Wmap(query_map);
    CObjectManager<WifiQuery>::getInstance()->add ( signal );
    //normalize ( query_map );
    
    
    
    //**********************************
    
    
    
    
    std::string dest_pos;
    std::map<std::string, double> pos_map;
    
    for ( std::vector<WifiData>::iterator it = p_res->begin(); it != p_res->end(); ++it )
    {
        std::map<std::string, double> t_map = it->Wmap();
        double sim = Similarity ( query_map, t_map );
        
        pos_map[it->Pos()] = sim;
    }
    
    
    if ( pos_map.size() < 1 ) return;
    
    //find the max of key_count_map
    std::map<std::string, double>::iterator it_max =  std::max_element ( pos_map.begin(), pos_map.end(), comp<double>() );
    
    boost::shared_ptr<vrmsg::QueryPostionResponse> response ( new vrmsg::QueryPostionResponse );
    
    vrmsg::Vector3* v3 = response->add_postions();
    vrmsg::Vector3* v3t = ( EntityMessageAdapter::StringToVector3 ( it_max->first ) );
    *v3 = *v3t;
    delete ( v3t );
    v3t = NULL;
    
    cout << it_max->first << endl;
    
    server_ptr.lock()->conn_mgr_ptr->unicast_by_conn_id ( msg->conn_id, response );
    
    signal->Pos(EntityMessageAdapter::Vector3toString(*v_true) );
    CObjectManager<WifiQuery>::getInstance()->update ( signal );
}

// 找出一个设备在一个点的所有采样，进行处理，得到一个特征向量
// 这里已经对信号强度加了100
int KnowledgeSystem::ProcessRawData2 ( const std::string pos, boost::shared_ptr<std::vector<WifiRaw>> raw_vec )
{

    typedef std::map<std::string, int> si_map_type;;
    typedef std::multimap<std::string, int> si_mmap_type;
    typedef std::pair<si_mmap_type::iterator, si_mmap_type::iterator>  range_type;
    //这里可能效率不好，取了多余的数据
    boost::shared_ptr<std::vector<WifiRaw>> wifi_vec  = raw_vec;
    si_mmap_type original_wifi;
    std::map<std::string, double> chosen_wifi;
    si_map_type key_count_map;
    
    
    for ( std::vector<WifiRaw>::iterator it = wifi_vec->begin(); it != wifi_vec->end(); ++it )
    {
        original_wifi.insert ( std::make_pair ( it->Identification(), it->Intensity() ) );
        //保存map的key
        key_count_map[it->Identification()] = key_count_map[it->Identification()] + 1;
    }
    
    //find the max of key_count_map
    si_map_type::iterator it_max =  std::max_element ( key_count_map.begin(), key_count_map.end(), comp<int>() );
    int max_count = it_max->second;
    
    //按key遍历multimap original_wifi
    for ( si_map_type::iterator it = key_count_map.begin(); it != key_count_map.end(); ++it )
    {
        //如果出现频率小于最大频率的一半则忽略它
        if ( it->second < max_count / 2.0 ) continue;
        
        //取multimap中某个有效值的范围
        range_type range = original_wifi.equal_range ( it->first );
        //待计算的中值数组
        std::vector<int> median_vec;
        
        //遍历original_map的一部分,信号加100
        for ( si_mmap_type::iterator jt = range.first; jt != range.second; ++jt )
        {
            median_vec.push_back ( jt->second + 100 );
        }
        
        //得到采集信号的中值
        chosen_wifi[it->first] =  IAverage ( median_vec ) ;
    }
    
    //normalize ( chosen_wifi );
    boost::shared_ptr<WifiData> p_data ( new WifiData ( pos, chosen_wifi ) );
    CObjectManager<WifiData>::getInstance()->add ( p_data );
    return 0;
}

void KnowledgeSystem::ProcessRawData()
{
    //从sanmple表里取数据范围
    boost::shared_ptr<std::vector<WifiSample>> sample_vec = CObjectManager<WifiSample>::getInstance()->get ( odb::query<WifiSample>() );
    typedef std::vector<unsigned long> idvec_type;
    idvec_type id_vec;
    int sample_size = sample_vec->size();
    
    //如果采样表为空，返回不处理
    if ( sample_size < 1 ) return;
    
    for ( int i = 0; i < sample_size; ++i )
    {
        id_vec.push_back ( ( *sample_vec ) [i].Raw_id() );
    }
    
    std::pair<idvec_type::iterator, idvec_type::iterator> min_max_result = std::minmax_element ( id_vec.begin(), id_vec.end() );
    unsigned long min_id = * ( min_max_result.first );
    unsigned long max_id = * ( min_max_result.second );
    
    
    //根据范围在raw表里找分组，device
    boost::shared_ptr<std::vector<wifi_view_device>> device_vec = CViewManager<wifi_view_device>::getInstance()->get ( odb::query<wifi_view_device>::id >= min_id );
    
    for ( std::vector<wifi_view_device>::iterator it = device_vec->begin(); it != device_vec->end(); ++it )
    {
        cout << it->device << endl;
        boost::shared_ptr<std::vector<wifi_view_pos>> pos_vec = CViewManager<wifi_view_pos>::getInstance()->get ( odb::query<wifi_view_pos>::id >= min_id && odb::query<wifi_view_pos>::id <= max_id && odb::query<wifi_view_pos>::device == it->device );
        
        for ( std::vector<wifi_view_pos>::iterator jt = pos_vec->begin(); jt != pos_vec->end(); ++jt )
        {
            cout << jt->pos << endl;
            boost::shared_ptr<std::vector<WifiRaw>> raw_vec = CObjectManager<WifiRaw>::getInstance()->get ( odb::query<WifiRaw>::id >= min_id && odb::query<WifiRaw>::id <= max_id && odb::query<WifiRaw>::device == it->device && odb::query<WifiRaw>::pos == jt->pos );
            ProcessRawData2 ( jt->pos, raw_vec );
        }
    }
    
    //清空sample表
    CObjectManager<WifiSample>::getInstance()->clear();
    cout << endl;
    cout << "done" << endl;
    return;
}

void KnowledgeSystem::showWifiData()
{
    boost::shared_ptr<std::vector<WifiData>> p_res = CObjectManager<WifiData>::getInstance()->get ( odb::query<WifiData>() );
    
    for ( std::vector<WifiData>::iterator it = p_res->begin(); it != p_res->end(); ++it )
    {
        cout << Utility::toString ( *it );
    }
}

void KnowledgeSystem::init()
{
    p_res = CObjectManager<WifiData>::getInstance()->get ( odb::query<WifiData>() );
    // test();
}

void KnowledgeSystem::test()
{
    boost::shared_ptr<std::vector<WifiData>> p_test = CObjectManager<WifiData>::getInstance()->get ( odb::query<WifiData>::pos == "5,0,3" );
    std::map<std::string, double> map_1 = ( *p_test ) [0].Wmap();
    std::map<std::string, double> map_2 = ( *p_test ) [1].Wmap();
    cout << Similarity ( map_1, map_2 ) << endl;
    
}




