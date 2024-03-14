#ifndef SERVICE_HPP
# define SERVICE_HPP

# include "global_namespace.hpp"
# include "io_context.hpp"

namespace Global {
	class Service;
}

class Global::Service {
private:
	IoContext io_context_;
	Service& operator=(const Service&) = delete;
public:
	Service();
	Service(IoContext& io_context);
	Service(const Service&);
	void setIoContext(IoContext& io_context);
	IoContext& getIoContext();

	template <typename Func>
	void addEvent(int fd, Func handler) {
		io_context_.addEvent(fd, handler);
	}

	template <typename Func>
	void addEvent(int fd, char* buf, size_t len, Func handler, int op_flag) {
		io_context_.addEvent(fd, buf, len, handler, op_flag);
	}
};


#endif
