#ifndef THREAD_SCHEDULAR_HPP
# define THREAD_SCHEDULAR_HPP

// 해야 하는 일. push pop 할 때 lock을 걸어야 함.
// 이벤트에가 감지되면, race 문제를 해결해줘야함.

# include "global_namespace.hpp"

namespace Global {
	class Schedular;
	// Thread shm;
};

class Global::Schedular {
public:
	Schedular() {}
	~Schedular() {}

	template <typename Func>
	void addEvent(int fd, char* buf, size_t len, Func handler, int op_flag) {
		std::lock_guard<std::mutex> lock(shm.mtx);
		shm.rwQueue.push(new OperationSocket(fd, len, buf, handler, op_flag));
	}

	int rwqueueEmpty() {
		std::lock_guard<std::mutex> lock(shm.mtx);
		return shm.rwQueue.empty();
	}

	IOperation* rwpopEvent() {
		IOperation* ret;
		std::lock_guard<std::mutex> lock(shm.mtx);
		ret = shm.rwQueue.front();
		shm.rwQueue.pop();
		return ret;
	}

	void rwpushEvent(IOperation* op) {
		std::lock_guard<std::mutex> lock(shm.mtx);
		shm.rwQueue.push(op);
	}
};

#endif
