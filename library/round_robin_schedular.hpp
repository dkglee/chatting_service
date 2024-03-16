#ifndef ROUND_ROBIN_SCHEDULAR_HPP
# define ROUND_ROBIN_SCHEDULAR_HPP

#include "schedular.hpp"
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <map>
#include <atomic>
#include <aio.h>

namespace Global {
	class RoundRobinSchedular;
};

class Global::RoundRobinSchedular : public Global::Schedular {
private:
	std::vector<std::thread> thread_pool;
	std::map<int, std::mutex> fd_mtx;
	std::atomic<int> idx;
	epoll_event events[MAX_EVENTS];

	void re_RegisterAll(epoll_event* event, int ret);
	void re_Register(epoll_event* event);
	void callHandler(int ret, int threadId);
	int addEpollEvent(IOperation* op);
	bool isRunnable();
	void delFd(int fd);
	void deActivateFd(epoll_event* event, int ret);

	void run(int threadId);
	void acceptSocket(IOperation* op);
	void readSocket(IOperation* op);
	void async_write_aio(IOperation* op);
public:
	RoundRobinSchedular();
	virtual ~RoundRobinSchedular();
	virtual void runOneOrAll(int num = 0) override;
};

#endif
