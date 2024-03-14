#ifndef BASIC_ENDPOINT_HPP
# define BASIC_ENDPOINT_HPP

# include "tcp.hpp"
# include "endpoint.hpp"
# include "global_namespace.hpp"

namespace Global {
	class BasicEndpoint;
}

class Global::BasicEndpoint {
public:
	BasicEndpoint() noexcept;
	BasicEndpoint(Tcp&& transport_protocol, unsigned short port_num) noexcept;
	int type() const noexcept;
	unsigned short port() const noexcept;
	int domain() const noexcept;
private:
	typedef EndpointImpl endpoint;
	endpoint impl_;
};

#endif
