#include "executor.hpp"
#include "basic_socket.hpp"
#include <errno.h>
#include <unistd.h>

Global::Executor::Executor()
	: epollFd(epoll_create1(0)), running(false)
{
	std::cout << "Executor constructor" << std::endl;
}

Global::Executor::~Executor() {
	std::cout << "Executor destructor" << std::endl;
	close(epollFd);
}

void Global::Executor::executeOne() {
	int ret;
	while (running || !acceptQueue.empty() || !rwQueue.empty()) {
		if (!acceptQueue.empty()) {
			running = true;
			IOperation* op = acceptQueue.front();
			acceptQueue.pop();
			if (epollCtl(op) == -1) {
				// error
			}
		}
		if (!rwQueue.empty()) {
			while (!rwQueue.empty()) {
				IOperation* op = rwQueue.front();
				rwQueue.pop();
				if (epollCtl(op) == -1) {
					// error
				}
			}
		}
		ret = epoll_wait(epollFd, events, MAX_EVENTS, -1);
		callHandler(ret);
	}
}

int Global::Executor::epollCtl(IOperation* op) {
	epoll_event event;
	event.data.ptr = (void*)op;
	event.events = EPOLLIN | EPOLLET | EPOLLPRI;
	if (op->op() == ACCEPT) {
		printf("before event ACCEPT op=%p\n", op);
	} else if (op->op() == READ) {
		printf("before event READ op=%p\n", op);
	} else {
		printf("before event Write op=%p\n", op);
	}
	if (op->op() == WRITE) {
		event.events |= EPOLLOUT;
	}
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, op->fd(), &event) == -1) {
		if (errno == EEXIST && op->op() == WRITE) {
			if (epoll_ctl(epollFd, EPOLL_CTL_MOD, op->fd(), &event) == -1) {
				// error
			}
		} else if (errno == EEXIST) {
			if (epoll_ctl(epollFd, EPOLL_CTL_MOD, op->fd(), &event) == -1) {
				// error
			}
		} else if (errno == EBADF) {
			if (op->op() == ACCEPT) {
				std::cout << "accept fd is bad" << std::endl;
			}
			else {
				std::cout << "new fd: " << op->fd() << " is bad" << std::endl;
				// error
			}
		}
		// else if {

		// }
	}
	return 0;
}

void Global::Executor::callHandler(int ret) {
	for (int i = 0; i < ret; i++) {
		IOperation* op = static_cast<IOperation*>(events[i].data.ptr);
		printf("after event op=%p\n", op);
		if (events[i].events & EPOLLIN) {
			if (op->op() == ACCEPT) {
				while (1) {
					int fd = accept(op->fd(), nullptr, nullptr);
					if (fd == -1) {
						if (errno == EAGAIN || errno == EWOULDBLOCK) {
							break;
						}
						BasicSocket null;
						op->acceptHandler(FAIL, null);
					}
					
					BasicSocket newSocket;
					// newSocket.setIoService(*this);
					newSocket.setSocket(fd);
					std::cout << "new fd: " << newSocket.getSocket() << std::endl;
					op->acceptHandler(SUCCESS, newSocket);
				}
				delete op;
				continue;
			} else {
				int n = recv(op->fd(), op->buf(), op->len(), 0);
				if (n == -1) {
					op->socketHandler(FAIL, n);
					delete op;
					continue;
				} else if (n == 0) {
					delFD(op->fd());
					op->socketHandler(CLOSE, n);
					delete op;
					continue;
				}
				op->socketHandler(SUCCESS, n);
				delete op;
			}
		}
		if (events[i].events & EPOLLOUT) {
			int n = send(op->fd(), op->buf(), op->len(), 0);
			if (n == -1) {
				op->socketHandler(FAIL, n);
				delete op;
				continue;
			}
			op->aluLen(n);
			op->aluBuf(n);
			if (op->len() <= 0) {
				epoll_event ev;
				ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
				epoll_ctl(epollFd, EPOLL_CTL_MOD, op->fd(), &ev);
				op->socketHandler(SUCCESS, n);
				delete op;
				continue;
			} else {
				rwQueue.push(op);
			}
		}
	}
}

void Global::Executor::delFD(int fd) {
	epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr);
	close(fd);
}
