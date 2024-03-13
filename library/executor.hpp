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
	Executor()
		: epollFd(epoll_create1(0))
	{
	}
	~Executor() {
		close(epollFd);
	}
	void addEvent(int fd, acceptHandler handler) {
		acceptQueue.push(Operation{fd, 0, nullptr, { .acceptHandler_ = handler }, ACCEPT});
	}
	void addEvent(int fd, char* buf, size_t len, socketHandler handler, int op_flag) {
		rwQueue.push(Operation{fd, len, buf, { .socketHandler_ = handler }, op_flag});
	}
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
