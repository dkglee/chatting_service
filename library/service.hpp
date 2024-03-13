#ifndef SERVICE_HPP
# define SERVICE_HPP

# include "io_context.hpp"
# include "basic_socket.hpp"
# include "basic_acceptor.hpp"
# include "global_namespace.hpp"

namespace Global {
	class Service;
}

class Global::Service {
private:
	IoContext* io_context_;
public:
	Service(IoContext& io_context)
		: io_context_(&io_context)
	{
	}
	void addEvent(int fd, acceptHandler handler) {
		io_context_->addEvent(fd, handler);
	}
	void addEvent(int fd, char* buf, size_t len, socketHandler hanler, int op_flag) {
		io_context_->addEvent(fd, buf, len, hanler, op_flag);
	}
};

#endif
