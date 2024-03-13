#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include <queue>
# include <sys/epoll.h>
# include <sys/types.h>
# include <socket.hpp>

# include "global_namespace.hpp"
# include "io_context.hpp"

namespace Global {
	class Executor;
};

class Global::Executor {
public:
	Executor();
	virtual ~Executor();
	void addEvent(int fd, acceptHandler handler);
	void addEvent(int fd, char* buf, size_t len, socketHandler handler, int op_flag);
	void executeOne();
private:
	Executor& operator=(const Executor&) = delete;

	int epollCtl(Operation& op, int op_flag);
	void callHandler(int ret);

	int epollFd;
	epoll_event events[MAX_EVENTS];
	std::queue<Operation> acceptQueue;
	std::queue<Operation> rwQueue;
};

#endif
