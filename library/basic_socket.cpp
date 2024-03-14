#include "basic_socket.hpp"

Global::BasicSocket::BasicSocket() noexcept
{
}

Global::BasicSocket::~BasicSocket()
{
}

Global::BasicSocket::BasicSocket(IoContext& io_context)
	: io_service_(io_context)
{
}

// Global::BasicSocket::BasicSocket(IoContext&& io_context)
// {
// 	// IoContext& lvalue_io_context = io_context;
// 	io_service_.setIoContext(std::move(io_context));
// }

void Global::BasicSocket::setIoService(Global::Service& io_service)
{
	io_service_.setIoContext(io_service.getIoContext());
}

Global::BasicSocket::BasicSocket(const BasicSocket&& other)
	: io_service_(other.io_service_), socket_(other.socket_)
{
	std::cout << "BasicSocket move constructor" << std::endl;
}

Global::BasicSocket::BasicSocket(int socket)
	: socket_(socket)
{
}

Global::BasicSocket::BasicSocket(BasicEndpoint& ep)
	: socket_(ep.domain(), ep.type(), ep.port())
{
}

// Global::BasicSocket::BasicSocket(Socket&& socket) noexcept
// 	: io_service_(this->io_service_), socket_(std::move(socket))
// {
// }

Global::BasicSocket::BasicSocket(const BasicSocket& other)
	: io_service_(other.io_service_), socket_(other.socket_)
{
}

// Global::BasicSocket Global::BasicSocket::listen(Global::BasicEndpoint& ep)
// {
// 	// BasicSocket ret;
// 	// ret.socket_.setSocket(socket_.listenTcp(ep.domain(), ep.type(), ep.port()).getSocket());
// 	// ret.io_service_.setIoContext(this->io_service_.getIoContext());
// 	// return ret;
// }

int Global::BasicSocket::getSocket() const noexcept
{
	return socket_.getSocket();
}

void Global::BasicSocket::setSocket(int socket) noexcept
{
	socket_.setSocket(socket);
}
