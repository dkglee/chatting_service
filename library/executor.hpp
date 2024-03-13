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
	
	template <typename Func>
	void addEvent(int fd, Func handler) {
		acceptQueue.push(std::make_shared<Operation<decltype<handler>>>(fd, 0, nullptr, handler, ACCEPT));
	}

	template <typename Func>
	void addEvent(int fd, char* buf, size_t len, Func handler, int op_flag) {
		rwQueue.push(std::make_shared<Operation<decltype<handler>>>(fd, len, buf, handler, op_flag));
	}

	void executeOne();
private:
	Executor& operator=(const Executor&) = delete;

	int epollCtl(std::unique_ptr<IOperation>& op, int op_flag);
	void callHandler(int ret);

	int epollFd;
	epoll_event events[MAX_EVENTS];
	std::queue<std::unique_ptr<IOperation>> acceptQueue;
	std::queue<std::unique_ptr<IOperation>> rwQueue;
};

#endif
