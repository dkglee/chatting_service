#include "basic_socket.hpp"

Global::BasicSocket::BasicSocket()
{
}

Global::BasicSocket::~BasicSocket()
{
}

Global::BasicSocket::BasicSocket(IoContext& io_context)
	: io_service_(io_context)
{
}

Global::BasicSocket::BasicSocket(IoContext&& io_context)
{
	IoContext& lvalue_io_context = io_context;
	io_service_.setIoContext(lvalue_io_context);
}

Global::BasicSocket::BasicSocket(Socket&& socket)
	: io_service_(this->io_service_), socket_(std::move(socket))
{
}

Global::BasicSocket Global::BasicSocket::listen(endpoint& ep)
{
	return BasicSocket(socket_.listenTcp(ep.domain(), ep.type(), ep.port()));
}

int Global::BasicSocket::socket() const noexcept
{
	return socket_.getSocket();
}

void Global::BasicSocket::socket(int socket) noexcept
{
	socket_.setSocket(socket);
}
