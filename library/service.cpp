#include "service.hpp"

Global::Service::Service()
{}

Global::Service::Service(IoContext& io_context)
	: io_context_(io_context)
{
	std::cout << "hi" << std::endl;
}

Global::Service::Service(const Service& service)
	: io_context_(service.io_context_)
{}


void Global::Service::setIoContext(Global::IoContext& io_context)
{
	io_context_.setExecutor(io_context.getExecutor());
}

Global::IoContext& Global::Service::getIoContext()
{
	return io_context_;
}
