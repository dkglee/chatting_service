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
private:
	BasicAcceptor(const BasicAcceptor&) = delete;
	BasicAcceptor& operator=(const BasicAcceptor&) = delete;
	Service* io_service_;
	BasicSocket listen_fd_;

public:
	~BasicAcceptor();
	BasicAcceptor() noexcept;
	BasicAcceptor(IoContext& io_context, BasicEndpoint& ep) noexcept;
	Service* getIoService() {
		return io_service_;
	}
	template <typename Func>
	void async_accept(Func handler) {
		std::cout << "async_accept" << std::endl;
		io_service_->addEvent(listen_fd_.getSocket(), handler);
	}
};

#endif
