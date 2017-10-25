/************************************************************************/
/*    日志
*/
/************************************************************************/
#ifndef __LOG_H__
#define __LOG_H__

#pragma once

#include "stdafx.h"

class Log
{
    public:
        static Log& getInstance();

        //默认初始化配置
        void init();

        //通过配置文件初始化配置
        void configure ( const std::string& cfg );

        //设置日志器的等级
        void setLogLevel ( const std::string &level );

        /**
        *进入context之后，调用此方法
        *@param msg 标识符消息
        */
        void pushNDC ( const std::string& msg );

        /**
        *离开context之后，调用此方法
        */
        void popNDC();

        /**
        *将要离开thread时，调用此方法，确保释放内存
        */
        void removeNDC();

        //具体日志方法
        void Trace ( const std::string& msg );
        void Debug ( const std::string& msg );
        void Info ( const std::string& msg );
        void Warn ( const std::string& msg );
        void Error ( const std::string& msg );
        void Fatal ( const std::string& msg );

        Log::~Log();

    protected:
        Log::Log();

    private:
        static Log *log;
        static log4cplus::Logger logger;
        static log4cplus::NDC& ndc;
};

#endif