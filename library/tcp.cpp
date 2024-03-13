#include "tcp.hpp"

Global::Tcp::Tcp()
{}

Global::Tcp::~Tcp()
{}

Global::Tcp::Tcp(int domain) noexcept
	: domain_(domain)
{}

Global::Tcp Global::Tcp::v4() noexcept
{
	return Tcp(AF_INET);
}

Global::Tcp Global::Tcp::v6() noexcept
{
	return Tcp(AF_INET6);
}

int Global::Tcp::type() noexcept
{
	return SOCK_STREAM;
}

int Global::Tcp::domain() const noexcept
{
	return domain_;
}

