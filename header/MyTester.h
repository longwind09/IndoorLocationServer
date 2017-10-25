/************************************************************************/
/*
    用来测试各个功能模块的类
*/
/************************************************************************/
#ifndef __MYTESTER_H__
#define __MYTESTER_H__

#pragma once

#include "stdafx.h"


class  DServer;

class MyTester
{
        typedef std::vector<std::vector<int>> array2_type;
    public:
        MyTester ( void );
        MyTester (boost::shared_ptr<DServer> p_server );
        void testAdd();
        void testGet();
        void Vector2File ( array2_type ar2,std::string filename );
        ~MyTester ( void );
        boost::weak_ptr<DServer> svr;
};

#endif