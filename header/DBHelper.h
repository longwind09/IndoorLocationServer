/************************************************************************/
/*      数据库访问初级接口
*/
/************************************************************************/
#ifndef __DB_HELPER_H_
#define __DB_HELPER_H_

#include "stdafx.h"


#pragma once
class DBHelper
{
    public:
        DBHelper ( void );
        ~DBHelper ( void );
        
        static boost::shared_ptr<odb::database> getDbInstance();
        void switchDb ( const std::string& db_name );
        
    private:
        static boost::shared_ptr<odb::database> generateDB();
};

#endif