#include "basic_acceptor.hpp"

Global::BasicAcceptor::BasicAcceptor() noexcept
{}

Global::BasicAcceptor::BasicAcceptor(IoContext& io_context, BasicEndpoint& ep) noexcept
	: io_service_(io_context), listen_fd_(io_context, ep)
{
}

// Path: library/basic_socket.cpp
