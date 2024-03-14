#ifndef TCP_HPP
# define TCP_HPP

# include "global_namespace.hpp"
// # include "basic_endpoint.hpp"
// # include "basic_socket.hpp"
// # include "basic_acceptor.hpp"
# include <sys/types.h>
# include <sys/socket.h>

namespace Global {
	class Tcp;
}

class Global::Tcp {
public:
	Tcp();
	~Tcp();

	// typedef BasicEndpoint endpoint;
	
	static Tcp v4() noexcept;
	static Tcp v6() noexcept;
	static int type() noexcept;
	int domain() const noexcept;

	// typedef BasicSocket socket;
	// typedef BasicAcceptor acceptor;

private:
	explicit Tcp(int domain) noexcept;
	int domain_;
};

#endif
