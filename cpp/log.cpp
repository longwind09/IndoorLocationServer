#include "stdafx.h"

#include "log.h"

using namespace std;
using namespace log4cplus;
using namespace log4cplus::helpers;

//static vars
Log* Log::log = 0;
Logger Log::logger = Logger::getRoot();
NDC& Log::ndc = getNDC();

Log::Log()
{
    init();
}

Log::~Log()
{
    //delete log;
}

Log& Log::getInstance()
{
    //线程不安全
    if ( !log ) log = new Log();

    return *log;
}

void Log::init()
{
    SharedAppenderPtr appender ( new FileAppender ( LOG4CPLUS_TEXT ( "server.log" ), ofstream::app, true ) );
    string pattern = "%D - NDC[%x] %m [%l]%n";
    auto_ptr<Layout> layout ( new PatternLayout ( tstring ( pattern.begin(), pattern.end() ) ) );
    appender->setLayout ( layout );
    logger.addAppender ( appender );
    logger.setLogLevel ( ALL_LOG_LEVEL );
}

void Log::configure ( const string& cfg )
{
    if ( cfg == "" ) return;

    logger.getDefaultHierarchy().resetConfiguration();
    PropertyConfigurator::doConfigure ( tstring ( cfg.begin(), cfg.end() ) );
}

void Log::setLogLevel ( const string &level )
{
    if ( level == "TRACE" )
        logger.setLogLevel ( TRACE_LOG_LEVEL );

    else if ( level == "DEBUG" )
        logger.setLogLevel ( DEBUG_LOG_LEVEL );

    else if ( level == "INFO" )
        logger.setLogLevel ( INFO_LOG_LEVEL );

    else if ( level == "WARN" )
        logger.setLogLevel ( WARN_LOG_LEVEL );

    else if ( level == "ERROR" )
        logger.setLogLevel ( ERROR_LOG_LEVEL );

    else if ( level == "FATAL" )
        logger.setLogLevel ( FATAL_LOG_LEVEL );
}


void Log::pushNDC ( const string& msg )
{
    getNDC().push ( tstring ( msg.begin(), msg.end() ) );
}

void Log::popNDC()
{
    getNDC().pop();
}

void Log::removeNDC()
{
    getNDC().remove();
}

void Log::Trace ( const std::string& msg )
{
    LOG4CPLUS_TRACE ( logger, tstring ( msg.begin(), msg.end() ) );
}
void Log::Debug ( const std::string& msg )
{
    LOG4CPLUS_DEBUG ( logger, tstring ( msg.begin(), msg.end() ) );
}
void Log::Info ( const std::string& msg )
{
    LOG4CPLUS_INFO ( logger, tstring ( msg.begin(), msg.end() ) );
}
void Log::Warn ( const std::string& msg )
{
    LOG4CPLUS_WARN ( logger, tstring ( msg.begin(), msg.end() ) );
}
void Log::Error ( const std::string& msg )
{
    LOG4CPLUS_ERROR ( logger, tstring ( msg.begin(), msg.end() ) );
}
void Log::Fatal ( const std::string& msg )
{
    LOG4CPLUS_FATAL ( logger, tstring ( msg.begin(), msg.end() ) );
}