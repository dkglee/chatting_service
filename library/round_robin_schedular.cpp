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
			thread_pool[threadId].detach();
		}
		this->run(0);
	} else {
		// std::cout << "hello" << std::endl;
		this->run(0);
	}
}

// private member func below

void Global::RoundRobinSchedular::run(int threadId) {
	int ret;
	while (this->isRunnable()) {
		if (!this->acceptQueueEmpty()) {
			this->setRunning(true);
			IOperation* op = this->accPopEvent();
			if (this->addEpollEvent(op) == -1) {
				// error
			}
		}
		if (!this->rwqueueEmpty()) {
			while (!this->rwqueueEmpty()) {
				IOperation* op = this->rwpopEvent();
				if (this->addEpollEvent(op) == -1) {
					// error
				}
			}
		}
		ret = epoll_wait(this->getEfd(), this->events, MAX_EVENTS, -1);
		this->callHandler(ret, threadId);
	}
}

// 추후에 handler를 시그널로 할지 또 다른 방식으로 처리할지 생각해 봐야겠다.
void Global::RoundRobinSchedular::async_write_aio(IOperation* op) {
	struct aiocb* cb;
	cb = (struct aiocb*)malloc(sizeof(struct aiocb));
	// memset(&cb, 0, sizeof(struct aiocb));

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
	if (this->fd_mtx.find(op->fd()) == this->fd_mtx.end()) {
		this->fd_mtx[op->fd()];
	}
	if (op->op() == WRITE) {
		// event.events |= EPOLLOUT;
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
	}
	op->socketHandler(SUCCESS, n);
}

void Global::RoundRobinSchedular::deActivateFd(epoll_event* event, int ret) {
	for (int i = 0; i < ret; i++) {
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
		IOperation* op = static_cast<IOperation*>(this->events[i].data.ptr);
		if (this->events[i].events & EPOLLIN) {
			if (op->op() == ACCEPT) {
				if (this->fd_mtx[op->fd()].try_lock()) {
					this->acceptSocket(op);
					this->fd_mtx[op->fd()].unlock();
				} else {
					this->re_Register(&(this->events[i]));
				}
			} else {
				if (this->fd_mtx[op->fd()].try_lock()) {
					this->readSocket(op);
					this->fd_mtx[op->fd()].unlock();
				} else {
					this->re_Register(&(this->events[i]));
				}
			}
			delete op;
		} 
	}
}

// read 입력이 들어오는 경우
// 1. 클라이언트의 작업이 끝나면 스스로 읽기 요청을 건네는 경우
// 2. 스레드가 자신의 차례가 아니라 넘기는 경우

// read(cli)를 하고 있는데 write(ano) 요청이 들어옴. 반대로 write(ano)를 하고 있는데 read(cli) 요청이 들어옴. write(ano)를 하고 있는데 write(cli) 요청이 들어옴
// 그냥 다시 보내면 됨. 어차피 한번 epoll_wait을 한 순간 내용이 변경되지 않음.
// 근데 이건 테스트 해봐야할듯.

// 이미 처리중인 데이터가 중간에 바뀔 수 있는지 확인해 봐야겠다. 커널에서 복사한 데이터를 건네주는 건지 아니면 포인터만 넘기는건지.
// ==> 복사한 데이터를 건네주는 것으로 확인됨. 그러므로 중간에 바뀔 일은 없다.
