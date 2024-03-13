#ifndef TCP_HPP
# define TCP_HPP

# include "basic_endpoint.hpp"
# include "basic_socket.hpp"
# include "basic_acceptor.hpp"
# include <sys/types.h>
# include <sys/socket.h>

class Tcp {
public:
	Tcp();
	~Tcp();

	typedef BasicEndpoint endpoint;
	
	static Tcp v4() noexcept {
		return Tcp(AF_INET);
	}
	static Tcp v6() noexcept {
		return Tcp(AF_INET6);
	}
	int type() const noexcept {
		return SOCK_STREAM;
	}
	int domain() const noexcept {
		return domain_;
	}

	typedef BasicSocket socket;
	typedef BasicAcceptor acceptor;

private:
	explicit Tcp(int domain) noexcept 
		: domain_(domain)
	{
	}
	int domain_;
};

#endif
