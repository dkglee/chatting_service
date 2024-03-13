#include "service.hpp"

Global::Service::Service() noexcept
{}

Global::Service::Service(IoContext& io_context) noexcept
	: io_context_(&io_context)
{}

Global::Service::Service(const Service& service)
	: io_context_(service.io_context_)
{}

void Global::Service::addEvent(int fd, acceptHandler handler)
{
	io_context_->addEvent(fd, handler);
}

void Global::Service::addEvent(int fd, char* buf, size_t len, socketHandler handler, int op_flag)
{
	io_context_->addEvent(fd, buf, len, handler, op_flag);
}


void Global::Service::setIoContext(IoContext& io_context)
{
	io_context_ = &io_context;
}
