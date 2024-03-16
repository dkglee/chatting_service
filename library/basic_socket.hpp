#ifndef BASIC_SOCKET_HPP
# define BASIC_SOCKET_HPP

# include "basic_endpoint.hpp"
# include "service.hpp"
# include "io_context.hpp"
# include "socket.hpp"
# include "global_namespace.hpp"

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <cstring>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <optional>

# define MAX_BACKLOG 10
# define READ 0
# define WRITE 1

namespace Global {
	class BasicSocket;
}

class Global::BasicSocket {
public:
	// typedef Global::BasicEndpoint endpoint;
	// BasicSocket(IoContext&& io_context);
	// explicit BasicSocket(Socket&& socket) noexcept;
	// BasicSocket listen(BasicEndpoint& ep);
	BasicSocket() noexcept;
	~BasicSocket();
	BasicSocket(IoContext& io_context);
	BasicSocket(const BasicSocket& other);
	BasicSocket(const BasicSocket&& other);
	BasicSocket(IoContext& io_context, BasicEndpoint& ep);
	BasicSocket(int socket);

	int getSocket() const noexcept;
	void setSocket(int socket) noexcept;
	void setIoService(Service* io_service);

	template <typename Func>
	void async_read(char* buf, size_t len, Func handler) {
		std::cout << "async_read" << std::endl;
		io_service_->addEvent(socket_.getSocket(), buf, len, handler, READ);
	}

private:
	Service* io_service_;
	Socket socket_;
};

#endif
