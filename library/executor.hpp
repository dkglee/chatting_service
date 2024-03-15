#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include <queue>
# include <sys/epoll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <memory>

# include "global_namespace.hpp"

namespace Global {
	class Executor;
};

class Global::Executor {
public:
	Executor();
	virtual ~Executor();
	
	template <typename Func>
	void addEvent(int fd, Func handler) {
		acceptQueue.push(new OperationAccept(fd, 0, nullptr, handler, ACCEPT));
	}

	template <typename Func>
	void addEvent(int fd, char* buf, size_t len, Func handler, int op_flag) {
		rwQueue.push(new OperationSocket(fd, len, buf, handler, op_flag));
	}

	void executeOne();
private:
	Executor& operator=(const Executor&) = delete;

	int epollCtl(IOperation* op);
	void callHandler(int ret);
	void delFD(int fd);

	int epollFd;
	bool running;
	epoll_event events[MAX_EVENTS];
	std::queue<IOperation*> acceptQueue;
	std::queue<IOperation*> rwQueue;
};

#endif
