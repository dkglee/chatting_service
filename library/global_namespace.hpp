#ifndef GLOBAL_NAMESPACE_HPP
# define GLOBAL_NAMESPACE_HPP

#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>
#include <unistd.h>

namespace Global {
	class IOperation;
	struct Thread_Global_Variables {
		std::mutex mtx;
		std::queue<IOperation*> rwQueue;
	};
	inline Thread_Global_Variables shm;
};

namespace Global {
	class Session;
	struct WorkerTask {
		int bytes_read;
		std::function<void(int)> task;
		WorkerTask(int bytes_read_, std::function<void(int)> task_)
			: bytes_read(bytes_read_), task(task_)
		{}
	};

	struct WorkerManager {
		std::mutex mtx;
		std::condition_variable cv;
		std::queue<WorkerTask*> workQueue;
	};
	inline WorkerManager workerManager;
};

namespace Global {
	#define READ 0
	#define WRITE 1
	#define ACCEPT 2
	#define READ_WRITE 3
	#define MAX_EVENTS 10
	#define SUCCESS 0
	#define FAIL 1
	#define CLOSE 2

	inline int evfd;
};

namespace Global {

	class Schedular;
	
	template <typename Func>
	class OperationSocket;

	template <typename Func>
	void async_write(int fd, char* buf, size_t len, Func handler) {
		{
			std::lock_guard<std::mutex> lock(shm.mtx);
			shm.rwQueue.push(new OperationSocket(fd, len, buf, handler, WRITE));
		}
		uint64_t u = 1;
		while (1) {
			if (write(evfd, &u, sizeof(uint64_t)) == -1) {
				if (errno == EAGAIN)
					continue;
			} else {
				break;
			}
		}
	}
};

#endif
