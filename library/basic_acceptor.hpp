#ifndef BASIC_ACCEPTOR_HPP
# define BASIC_ACCEPTOR_HPP

#include "io_context.hpp"
#include "basic_socket.hpp"
#include "basic_endpoint.hpp"
#include "service.hpp"

namespace Global {
	class BasicAcceptor;
}

class Global::BasicAcceptor {
public:
	typedef BasicSocket socket;
	typedef BasicEndpoint endpoint;

	BasicAcceptor() noexcept;
	BasicAcceptor(IoContext& io_context, endpoint&& ep) noexcept;

	template <typename Func>
	void async_accept(Func handler) {
		io_service_.addEvent(listen_fd_.socket(), handler);
	}
	
private:
	BasicAcceptor(const BasicAcceptor&) = delete;
	BasicAcceptor& operator=(const BasicAcceptor&) = delete;
	Service io_service_;
	socket listen_fd_;
	socket* tool;
};

#endif
