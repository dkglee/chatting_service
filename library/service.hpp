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
	Service& operator=(const Service&) = delete;
public:
	Service();
	Service(IoContext& io_context);
	Service(const Service&);
	void addEvent(int fd, acceptHandler handler);
	void addEvent(int fd, char* buf, size_t len, socketHandler hanler, int op_flag);
	void setIoContext(IoContext& io_context);
};

#endif
