#include "service.hpp"

Global::Service::Service() noexcept
{}

Global::Service::Service(IoContext& io_context) noexcept
	: io_context_(&io_context)
{}

Global::Service::Service(const Service& service)
	: io_context_(service.io_context_)
{}


void Global::Service::setIoContext(IoContext& io_context)
{
	io_context_ = &io_context;
}
