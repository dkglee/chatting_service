#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <cstring>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <stdexcept>

# define MAX_BACKLOG 10

class Socket {
public:
	Socket() noexcept;
	~Socket();
	Socket(const Socket& socket) noexcept;
	Socket(const Socket&& socket) noexcept;
	Socket(int domain, int type, unsigned short port_);
	Socket(int socket);
	void listenTcp(int domain, int type, unsigned short port_);
	int getSocket() const noexcept;
	void setSocket(int socket) noexcept;
private:
	int fcnNb(int socket);
	int instlSocket(int family, int type, unsigned short port_);

	int socket_;
};

#endif
