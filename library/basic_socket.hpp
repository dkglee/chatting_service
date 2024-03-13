#ifndef BASIC_SOCKET_HPP
# define BASIC_SOCKET_HPP

# include "socket.hpp"
# include "basic_endpoint.hpp"
# include "service.hpp"
# include "io_context.hpp"
# include "global_namespace.hpp"

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <cstring>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

# define MAX_BACKLOG 10
# define READ 0
# define WRITE 1


namespace Global {
	class BasicSocket;
}

class Global::BasicSocket  {
private:
	// enum RW_TYPE {
	// 	READ = 0,
	// 	WRITE
	// } rw;
public:
	typedef BasicEndpoint endpoint;
	BasicSocket() noexcept;
	~BasicSocket();
	BasicSocket(IoContext& io_context)
		: io_service_(io_context)
	{
	}
	BasicSocket(IoContext&& io_context)
		: io_service_(io_context)
	{
	}
	explicit BasicSocket(Socket&& socket) noexcept
		: io_service_(this->io_service_), socket_(std::move(socket))
	{
	}
	BasicSocket listen(endpoint& ep) {
		return BasicSocket(socket_.listenTcp(ep.domain(), ep.type(), ep.port()));
	}
	int socket() const noexcept {
		return socket_.getSocket();
	}
	void socket(int socket) noexcept {
		socket_.setSocket(socket);
	}
	void async_read(char* buf, size_t len, socketHandler handler) {
		io_service_.addEvent(socket_.getSocket(), buf, len, handler, READ);
	}
	void async_write(int fd, char* buf, size_t len, socketHandler handler) {
		io_service_.addEvent(fd, buf, len, handler, WRITE);
	}

private:
	Service io_service_;
	Socket socket_;
};

#endif
