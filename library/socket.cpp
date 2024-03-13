#include "socket.hpp"

Socket::Socket()
{}

Socket::~Socket()
{}

Socket::Socket(const Socket& socket)
	: socket_(socket.socket_)
{}

Socket Socket::listenTcp(int domain, int type, unsigned short port_) {
	if (instlSocket(domain, type, port_) == -1) {
		throw std::runtime_error("instlSocket() failed");
	}
	if (listen(socket_, MAX_BACKLOG) == -1) {
		throw std::runtime_error("listen() failed");
	}
	return *this;
}

int Socket::getSocket() const noexcept {
	return socket_;
}

void Socket::setSocket(int socket) noexcept {
	fcnNb(socket);
	socket_ = socket;
}

int Socket::fcnNb(int socket) {
	int flags = fcntl(socket, F_GETFL, 0);
	if (flags == -1) {
		return -1;
	}
	if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1) {
		return -1;
	}
	return 0;
}

int Socket::instlSocket(int family, int type, unsigned short port_) {
	int rc_gai;
	struct addrinfo hints, *res;
	const char* port = std::to_string(port_).c_str();
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = family;
	hints.ai_socktype = type;
	hints.ai_flags = AI_ADDRCONFIG | AI_PASSIVE;
	rc_gai = getaddrinfo(NULL, port, &hints, &res);
	if (rc_gai != 0) {
		return -1;
	}
	socket_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socket_ == -1) {
		return -1;
	}
	if (bind(socket_, res->ai_addr, res->ai_addrlen) == -1) {
		return -1;
	}
	freeaddrinfo(res);
	return 0;
}
