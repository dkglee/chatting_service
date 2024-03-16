#include "basic_acceptor.hpp"

Global::BasicAcceptor::BasicAcceptor() noexcept
	: io_service_(nullptr)
{}

Global::BasicAcceptor::~BasicAcceptor()
{
	if (io_service_ != nullptr)
		delete io_service_;
}

Global::BasicAcceptor::BasicAcceptor(IoContext& io_context, BasicEndpoint& ep) noexcept
	: io_service_(new Service(io_context)), listen_fd_(io_context, ep)
{
}

// Path: library/basic_socket.cpp
