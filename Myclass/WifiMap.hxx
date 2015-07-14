/************************************************************************/
/*      wifisignal
*/
/************************************************************************/
#ifndef __WIFI_MAP_H__
#define __WIFI_MAP_H__


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

#pragma db object
class WifiMap
{

    public:
        WifiMap (
            const std::map<std::string,int> vec )
            : id_ ( 0 ),
              wifi_vec_ ( vec )
        {
        }
        
        WifiMap () {}
        ~WifiMap ( void ) {}
        std::map<std::string ,int > wifi_vec_;
        std::map<std::string ,int > wifi_vec() const { return wifi_vec_; }
        void wifi_vec ( std::map<std::string ,int > val ) { wifi_vec_ = val; }
        
    private:
    
        friend class odb::access;
#pragma db id auto
        unsigned long id_;
        
        
        
};


#endif