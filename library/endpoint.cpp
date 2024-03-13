#include "endpoint.hpp"

EndpointImpl::EndpointImpl() noexcept
{}

EndpointImpl::EndpointImpl(int domain, unsigned short port_num) noexcept
	: data_({domain, port_num})
{}

int EndpointImpl::domain() const noexcept
{
	return data_.domain;
}

unsigned short EndpointImpl::port() const noexcept
{
	return data_.port;
}

