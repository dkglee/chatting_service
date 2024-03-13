#include "basic_acceptor.hpp"

Global::BasicAcceptor::BasicAcceptor() noexcept
{}

Global::BasicAcceptor::BasicAcceptor(IoContext& io_context, endpoint&& ep) noexcept
	: io_service_(io_context), listen_fd_(tool->listen(ep))
{
}

// Path: library/basic_socket.cpp
