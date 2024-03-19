#ifndef SCHEDULAR_HPP
# define SCHEDULAR_HPP

// 해야 하는 일. push pop 할 때 lock을 걸어야 함.
// 이벤트에가 감지되면, race 문제를 해결해줘야함.

# include <queue>
# include <sys/epoll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <memory>
# include <unistd.h>
# include <sys/eventfd.h>

# include "./global_namespace.hpp"
# include "./operation_accept.hpp"
# include "./operation_socket.hpp"

namespace Global {
	class Schedular;
	// Thread shm;
};

class Global::Schedular {
private:
	int epollFd;
	bool running;
	// static int evfd;
	// epoll_event events[MAX_EVENTS];
	std::mutex acceptMtx;
	std::queue<IOperation*> acceptQueue;
public:
	Schedular();
	virtual ~Schedular();

	template <typename Func>
	void addEvent(int fd, char* buf, size_t len, Func handler, int op_flag) {
		std::lock_guard<std::mutex> lock(shm.mtx);
		shm.rwQueue.push(new OperationSocket(fd, len, buf, handler, op_flag));
	}
	template <typename Func>
	void addEvent(int fd, Func handler) {
		std::lock_guard<std::mutex> lock(acceptMtx);
		acceptQueue.push(new OperationAccept(fd, 0, nullptr, handler, ACCEPT));
	}

	int getEfd();
	// static int getEvfd();
	void setRunning(bool);
	bool getRunning();
	bool acceptQueueEmpty();
	IOperation* accPopEvent();

	int rwqueueEmpty();
	IOperation* rwpopEvent();
	void rwpushEvent(IOperation* op);
	virtual void runOneOrAll(int num = 0) = 0;
};

#endif
