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

class Global::BasicSocket {
private:
	Service io_service_;
	Socket socket_;

public:
	typedef Global::BasicEndpoint endpoint;
	BasicSocket() noexcept;
	~BasicSocket();
	BasicSocket(IoContext& io_context);
	BasicSocket(IoContext&& io_context);
	explicit BasicSocket(Socket&& socket) noexcept;
	BasicSocket listen(endpoint& ep);
	int socket() const noexcept;
	void socket(int socket) noexcept;

	template <typename Func>
	void async_read(char* buf, size_t len, Func handler) {
		io_service_.addEvent(socket_.getSocket(), buf, len, handler, READ);
	}

	template <typename Func>
	void async_write(int fd, char* buf, size_t len, Func handler) {
		io_service_.addEvent(fd, buf, len, handler, WRITE);
	}
};

#endif
