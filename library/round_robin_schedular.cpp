#include "round_robin_schedular.hpp"
#include "basic_socket.hpp"
#include <utility>

Global::RoundRobinSchedular::RoundRobinSchedular()
	: idx(0)
{
	std::cout << "RoundRobinSchedular constructor" << std::endl;
}

Global::RoundRobinSchedular::~RoundRobinSchedular() {
	std::cout << "RoundRobinSchedular destructor" << std::endl;
}


void Global::RoundRobinSchedular::runOneOrAll(int num) {
	if (num > 0) {
		for (int threadId = 1; threadId < num; threadId++) {
			thread_pool.emplace_back([this, threadId] {
				this->run(threadId);
			});
			thread_pool[threadId - 1].detach();
		}
	}
	this->run(0);
}

// private member func below
void Global::RoundRobinSchedular::run(int threadId) {
	int ret;
	while (this->isRunnable()) {
		if (!this->acceptQueueEmpty()) {
			this->setRunning(true);
			IOperation* op = this->accPopEvent();
			if (this->addEpollEvent(op) == -1) {
				this->acceptPushEvent(op);
			}
		}
		if (!this->rwqueueEmpty()) {
			while (!this->rwqueueEmpty()) {
				IOperation* op = this->rwpopEvent();
				if (this->addEpollEvent(op) == -1) {
					this->rwpushEvent(op);
				}
			}
		}
		ret = epoll_wait(this->getEfd(), this->events, MAX_EVENTS, -1);
		this->callHandler(ret, threadId);
	}
}

// 추후에 핸들러를 시그널로 바꿀까..?
void Global::RoundRobinSchedular::async_write_aio(IOperation* op) {
	struct aiocb* cb;
	cb = (struct aiocb*)malloc(sizeof(struct aiocb));

	cb->aio_fildes = op->fd();
	cb->aio_buf = op->buf();
	cb->aio_nbytes = op->len();
	cb->aio_offset = 0;
	cb->aio_sigevent.sigev_notify = SIGEV_NONE;
	aio_write(cb);
	aio_suspend(&cb, 1, nullptr);
	free(cb);
	op->socketHandler(SUCCESS, op->len());
	delete op;
}

int Global::RoundRobinSchedular::addEpollEvent(IOperation* op) {
	epoll_event event;
	event.data.ptr = (void*)op;
	event.events = EPOLLET | EPOLLPRI;
	{
		std::lock_guard<std::mutex> lock(this->mtx_map);
		if (this->fd_mtx.find(op->fd()) == this->fd_mtx.end()) {
			this->fd_mtx[op->fd()];
		}
	}
	if (op->op() == WRITE) {
		this->async_write_aio(op);
		return 0;
	} else {
		event.events |= EPOLLIN;
	}
	if (epoll_ctl(this->getEfd(), EPOLL_CTL_ADD, op->fd(), &event) != 0) {
		if (errno == EEXIST)
			epoll_ctl(this->getEfd(), EPOLL_CTL_MOD, op->fd(), &event);
		else
			return -1;
	}
	return 0;
}

bool Global::RoundRobinSchedular::isRunnable() {
	return (this->getRunning() || !this->acceptQueueEmpty() || !this->rwqueueEmpty());
}

void Global::RoundRobinSchedular::re_RegisterAll(epoll_event* event, int ret) {
	for (int i = 0; i < ret; i++) {
		if (event[i].events & EPOLLIN && event[i].data.fd == Global::evfd) {
			continue;
		}
		IOperation* op = static_cast<IOperation*>(event[i].data.ptr);
		this->addEpollEvent(op);
	}
}

void Global::RoundRobinSchedular::acceptSocket(IOperation* op) {
	int fd = op->fd();
	int newFd;
	while (1) {
		newFd = accept(fd, nullptr, nullptr);
		if (newFd == -1) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				break;
			}
			BasicSocket null;
			op->acceptHandler(FAIL, null);
			return ;
		}
		BasicSocket newSock;
		newSock.setSocket(newFd);
		op->acceptHandler(SUCCESS, newSock);
	}
}

void Global::RoundRobinSchedular::re_Register(epoll_event* event) {
	IOperation* op = static_cast<IOperation*>(event->data.ptr);
	this->addEpollEvent(op);
}

void Global::RoundRobinSchedular::delFd(int fd) {
	epoll_ctl(this->getEfd(), EPOLL_CTL_DEL, fd, nullptr);
	close(fd);
}

void Global::RoundRobinSchedular::readSocket(IOperation* op) {
	int n = recv(op->fd(), op->buf(), op->len(), 0);
	if (n == -1) {
		op->socketHandler(FAIL, n);
	} else if (n == 0) {
		delFd(op->fd());
		op->socketHandler(CLOSE, n);
	} else {
		op->socketHandler(SUCCESS, n);
	}
}

void Global::RoundRobinSchedular::deActivateFd(epoll_event* event, int ret) {
	for (int i = 0; i < ret; i++) {
		if (event[i].events & EPOLLIN && event[i].data.fd == Global::evfd) {
			continue;
		}
		IOperation* op = static_cast<IOperation*>(event[i].data.ptr);
		epoll_event ev;
		ev.events = 0;
		epoll_ctl(this->getEfd(), EPOLL_CTL_MOD, op->fd(), &ev);
	}
}

void Global::RoundRobinSchedular::callHandler(int ret, int threadId) {
	deActivateFd(this->events, ret);
	int myTurn = this->idx.fetch_add(1) % (thread_pool.size() + 1);
	if (myTurn != threadId) {
		re_RegisterAll(this->events, ret);
		return ;
	}
	for (int i = 0; i < ret; i++) {
		if (this->events[i].events & EPOLLIN && this->events[i].data.fd == Global::evfd) {
			uint64_t u;
			read(Global::evfd, &u, sizeof(uint64_t));
			continue;
		} else {
			IOperation* op = static_cast<IOperation*>(this->events[i].data.ptr);
			if (this->events[i].events & EPOLLIN) {
				if (op->op() == ACCEPT) {
					{
						std::lock_guard<std::mutex> lock(this->mtx_map);
						if (this->fd_mtx[op->fd()].try_lock()) {
							this->acceptSocket(op);
							this->fd_mtx[op->fd()].unlock();
						} else {
							this->re_Register(&(this->events[i]));
						}
					}
				} else {
					{
						std::lock_guard<std::mutex> lock(this->mtx_map);
						if (this->fd_mtx[op->fd()].try_lock()) {
							this->readSocket(op);
							this->fd_mtx[op->fd()].unlock();
						} else {
							this->re_Register(&(this->events[i]));
						}
					}
				}
			}
			delete op;
		}
	}
}
