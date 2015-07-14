/************************************************************************/
/*      wifisignal
*/
/************************************************************************/
#ifndef __ODB_WIFI_H__
#define __ODB_WIFI_H__


#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/unordered_set.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <odb/core.hxx>

#include <odb/boost/lazy-ptr.hxx>


using boost::shared_ptr;
using boost::weak_ptr;

using odb::boost::lazy_shared_ptr;
using odb::boost::lazy_weak_ptr;

using boost::uuids::uuid;
using boost::gregorian::date;


//#pragma db value
//class WifiMap
//{
//
//public:
//    WifiMap ( const std::map<std::string,double> vec ) :
//      wifi_vec_ ( vec )
//      {
//      }
//
//      WifiMap () {}
//      ~WifiMap ( void ) {}
//
//      std::map<std::string ,double > wifi_vec() const { return wifi_vec_; }
//      void wifi_vec ( std::map<std::string ,double > val ) { wifi_vec_ = val; }
//
//private:
//
//    friend class odb::access;
//    std::map<std::string ,double > wifi_vec_;
//};
#pragma db object
class WifiData
{

    public :
        typedef std::map<std::string ,double > WifiMap;
        WifiData ( const std::string &pos,
                   const WifiMap &wmap )
            :
            pos_ ( pos ),
            wmap_ ( wmap )
        {
        
        }
        WifiData() {}
        ~WifiData() {}
        std::string Pos() const { return pos_; }
        void Pos ( const std::string &val ) { pos_ = val; }
        
        WifiMap Wmap() const { return wmap_; }
        void Wmap ( const WifiMap &val ) { wmap_ = val; }
    private:
        friend class odb::access;
#pragma db id auto
        unsigned long id_;
        std::string pos_;
        
        WifiMap wmap_;
        
};

#pragma db object
class WifiSample
{

    public :
        WifiSample ( const long &raw_id )
            :
            raw_id_ ( raw_id )
        {
        
        }
        WifiSample() {}
        ~WifiSample() {}
        unsigned long Raw_id() const { return raw_id_; }
        void Raw_id ( unsigned long val ) { raw_id_ = val; }
    private:
        friend class odb::access;

#pragma db id
        unsigned long raw_id_;
        
        
};

#pragma db object
class WifiRaw
{

    public:
        WifiRaw ( const std::string &identification,
                  const std::string &name,
                  const std::string &device_id,
                  const std::string &nowtime,
                  const int &intensity,
                  const std::string &pos_string )
            : id_ ( 0 ),
              identification_ ( identification ),
              name_ ( name ),
              device_ ( device_id ),
              nowtime_ ( nowtime ),
              intensity_ ( intensity ),
              pos_ ( pos_string )
        {
        }
        
        WifiRaw () {}
        ~WifiRaw ( void ) {}
        
        std::string Identification() const {return identification_;}
        void Identification ( std::string val ) {identification_ = val;}
        
        int Intensity() const {return intensity_;}
        void Intensity ( int val ) {intensity_ = val;}
        
        std::string Device() const {return device_;}
        void Device ( std::string val ) {device_ = val;}
        
        int Id() const {return id_;}
        void Id ( int val ) {id_ = val;}
        
        
        
        std::string Pos() const { return pos_; }
        void Pos ( std::string val ) { pos_ = val; }
        
        std::string Name() const {return name_;}
        void Name ( std::string val ) {name_ = val;}
        
        std::string Nowtime() const {return nowtime_;}
        void Nowtime ( std::string val ) {nowtime_ = val;}
    private:
    
        friend class odb::access;
#pragma db id auto
        unsigned long id_;
        
        std::string identification_;
        
        std::string name_;
        std::string device_;
        std::string nowtime_;
        
        int intensity_;
        std::string pos_;
};

//#pragma db object
//class WifiQuery
//{
//
//public:
//    WifiQuery ( const std::string &identification,
//        const std::string &name,
//        const std::string &device_id,
//        const std::string &nowtime,
//        const int &intensity,
//        const std::string &pos_string )
//        : id_ ( 0 ),
//        identification_ ( identification ),
//        name_ ( name ),
//        device_ ( device_id ),
//        nowtime_ ( nowtime ),
//        intensity_ ( intensity ),
//        pos_ ( pos_string )
//    {
//    }
//
//    WifiQuery () {}
//    ~WifiQuery ( void ) {}
//
//    std::string Identification() const {return identification_;}
//    void Identification ( std::string val ) {identification_ = val;}
//
//    int Intensity() const {return intensity_;}
//    void Intensity ( int val ) {intensity_ = val;}
//
//    std::string Device() const {return device_;}
//    void Device ( std::string val ) {device_ = val;}
//
//    int Id() const {return id_;}
//    void Id ( int val ) {id_ = val;}
//
//
//
//    std::string Pos() const { return pos_; }
//    void Pos ( std::string val ) { pos_ = val; }
//
//    std::string Name() const {return name_;}
//    void Name ( std::string val ) {name_ = val;}
//
//    std::string Nowtime() const {return nowtime_;}
//    void Nowtime ( std::string val ) {nowtime_ = val;}
//private:
//
//    friend class odb::access;
//#pragma db id auto
//    unsigned long id_;
//
//    std::string identification_;
//
//    std::string name_;
//    std::string device_;
//    std::string nowtime_;
//
//    int intensity_;
//    std::string pos_;
//};

#pragma db object
class WifiQuery
{

public :
    typedef std::map<std::string ,double > QueryMap;
    WifiQuery ( const std::string &pos,
        const QueryMap &wmap )
        :
    pos_ ( pos ),
        wmap_ ( wmap )
    {

    }
    WifiQuery() {}
    ~WifiQuery() {}
    std::string Pos() const { return pos_; }
    void Pos ( const std::string &val ) { pos_ = val; }

    QueryMap Wmap() const { return wmap_; }
    void Wmap ( const QueryMap &val ) { wmap_ = val; }
private:
    friend class odb::access;
#pragma db id auto
    unsigned long id_;
    std::string pos_;

    QueryMap wmap_;

};


#pragma db view object(WifiRaw) \
    query ((?) + "GROUP BY" + WifiRaw::pos_)
struct wifi_view_pos
{
#pragma db column(WifiRaw::pos_)
    std::string pos;
};
#pragma db view object(WifiRaw) \
    query ((?) + "GROUP BY" + WifiRaw::device_)
struct wifi_view_device
{
#pragma db column(WifiRaw::device_)
    std::string device;
};
#pragma db view object(WifiRaw) \
    query ((?) + "GROUP BY" + WifiRaw::identification_)
struct wifi_view_identification
{
#pragma db column(WifiRaw::identification_)
    std::string identification;
};

#endif