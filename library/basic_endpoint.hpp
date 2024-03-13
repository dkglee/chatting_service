#ifndef BASIC_ENDPOINT_HPP
# define BASIC_ENDPOINT_HPP

# include "tcp.hpp"
# include "endpoint.hpp"

namespace Global {
	class BasicEndpoint;
}

class Global::BasicEndpoint {
public:
	BasicEndpoint() noexcept;
	BasicEndpoint(Tcp& transport_protocol, unsigned short port_num) noexcept
		: impl_(transport_protocol.domain(), port_num)
	{
	}
	int type() const noexcept {
		Tcp().type();
	}
	unsigned short port() const noexcept {
		return impl_.port();
	}
	int domain() const noexcept {
		return impl_.domain();
	}
private:
	typedef EndpointImpl endpoint;
	endpoint impl_;
};

#endif
