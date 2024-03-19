#include "basic_socket.hpp"

Global::BasicSocket::BasicSocket() noexcept
	: io_service_(nullptr)
{
}

Global::BasicSocket::~BasicSocket()
{
	if (io_service_ != nullptr)
		delete io_service_;
}

Global::BasicSocket::BasicSocket(IoContext& io_context)
	: io_service_(new Service(io_context))
{
}


void Global::BasicSocket::setIoService(Global::Service* io_service)
{
	io_service_ = new Service(io_service->getIoContext());
}

Global::BasicSocket::BasicSocket(const BasicSocket&& other)
	: io_service_(new Service(other.io_service_->getIoContext())), socket_(other.socket_)
{
}

Global::BasicSocket::BasicSocket(int socket)
	: socket_(socket)
{
}

Global::BasicSocket::BasicSocket(IoContext& io_context, BasicEndpoint& ep)
	: io_service_(new Service(io_context)), socket_(ep.domain(), ep.type(), ep.port())
{
}

Global::BasicSocket::BasicSocket(const BasicSocket& other)
	: io_service_(new Service(other.io_service_->getIoContext())), socket_(other.socket_)
{
}

int Global::BasicSocket::getSocket() const noexcept
{
	return socket_.getSocket();
}

void Global::BasicSocket::setSocket(int socket) noexcept
{
	socket_.setSocket(socket);
}
