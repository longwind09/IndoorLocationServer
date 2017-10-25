#ifndef __STDAFX_H__
#define __STDAFX_H__

#pragma once

#define WIN32_LEAN_AND_MEAN
//for odb_mysql definition
#define DATABASE_MYSQL


//#include <cmath>
//#include <cassert>
//#include <cstring>  // std::memcpy
//io
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <istream>
#include <ostream>

//stl container
#include <vector>
#include <string>
#include <set>
#include <hash_map>
#include <map>
#include <list>
#include <utility>
#include <queue>

//network framework
#include <boost/asio.hpp>

//multithread
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/condition_variable.hpp>

//for serialization
//#include <boost/archive/text_iarchive.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/binary_oarchive.hpp>
//#include <boost/archive/binary_iarchive.hpp>
//#include <boost/serialization/vector.hpp>

//time
#include <boost/date_time/posix_time/posix_time.hpp>

//三态布尔值
//#include <boost/logic/tribool.hpp>
//一对
#include <boost/tuple/tuple.hpp>
//断言
#include <boost/assert.hpp>

//bind and callback
#include <boost/bind.hpp>
#include <boost/function.hpp>

//smart pointer
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/weak_ptr.hpp>

//类型转换 int-string .etc
#include <boost/lexical_cast.hpp>
//涉及到字符串的算法比如？？反转字符串
#include <boost/algorithm/string.hpp>


//log4cplus
#include <log4cplus/logger.h>
#include <log4cplus/fileappender.h>
#include <log4cplus/hierarchy.h>
#include <log4cplus/layout.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/ndc.h>
#include <log4cplus/configurator.h>


//lua
//extern "C"
//{
//#include <lua.h>
//#include <lualib.h>
//#include <lauxlib.h>
//}
//
//#include <luabind/luabind.hpp>
//#include <luabind/function.hpp>
//#include <luabind/operator.hpp>

//xml resolve
//#include <tinyxml.h>
//#include <xpath_static.h>

//target version
#include "targetver.h"
////sql database
//#include <icrsint.h>

//google protobuf
#include <google/protobuf/message.h>


//mysql
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <mysql_error.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>



//boost part for odb orm

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/unordered_set.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

//odb-mysql-lib
#include <odb/mysql/traits.hxx>
#include <odb/mysql/database.hxx>
#include <odb/mysql/transaction.hxx>
#include <odb/mysql/connection.hxx>
#include <odb/mysql/statement.hxx>
#include <odb/mysql/statement-cache.hxx>
#include <odb/mysql/simple-object-statements.hxx>
#include <odb/mysql/container-statements.hxx>
#include <odb/mysql/exceptions.hxx>
#include <odb/mysql/simple-object-result.hxx>
#include <odb/mysql/enum.hxx>

//odb-boost-lib
#include <odb/boost/lazy-ptr.hxx>


//odb-common-lib
#include <odb/core.hxx>
#include <odb/database.hxx>
#include <odb/session.hxx>
#include <odb/transaction.hxx>


typedef unsigned char Byte;


#define DISALLOW_COPY_AND_ASSIGN(TypeName) TypeName(const TypeName&); void operator=(const TypeName&);


#endif