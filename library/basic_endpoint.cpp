#include "basic_endpoint.hpp"

Global::BasicEndpoint::BasicEndpoint() noexcept
{}

Global::BasicEndpoint::BasicEndpoint(Tcp&& transport_protocol, unsigned short port_num) noexcept
	: impl_(transport_protocol.domain(), port_num)
{
}

int Global::BasicEndpoint::type() const noexcept
{
	return Tcp::type();
}

unsigned short Global::BasicEndpoint::port() const noexcept
{
	return impl_.port();
}

int Global::BasicEndpoint::domain() const noexcept
{
	return impl_.domain();
}

