#include "stdafx.h"
#include "ConnectionBase.h"



//初始化列表原则，能放在初始化列表的多放，只能初始化不能赋值的必须放
ConnectionBase::ConnectionBase ( boost::shared_ptr<boost::asio::io_service> io_service_ptr )
    : socket_ ( *io_service_ptr ), strand_ ( *io_service_ptr )
{
    //一个io_service_ptr 创建 connection
    //初始化成员socket strand(strand串行，io_service并发)
}

ConnectionBase::~ConnectionBase( )
{
    //虚析构函数是为了解决这样的一个问题：基类的指针指向派生类对象，并用基类的指针删除派生类对象。
}

boost::asio::ip::tcp::socket& ConnectionBase::socket()
{
    return socket_;
}

