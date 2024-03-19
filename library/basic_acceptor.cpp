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
	if (listen_fd_.getSocket() == -1) {
		std::cerr << "listen_fd_ is -1" << std::endl;
		exit(1);
	}
}
