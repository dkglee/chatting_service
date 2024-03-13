#include "executor.hpp"
#include <errno.h>
#include <unistd.h>

Global::Executor::Executor()
	: epollFd(epoll_create1(0))
{
}

Global::Executor::~Executor() {
	close(epollFd);
}

void Global::Executor::executeOne() {
	int ret;
	while (!acceptQueue.empty() && !rwQueue.empty()) {
		if (!acceptQueue.empty()) {
			std::unique_ptr<IOperation> op = std::move(acceptQueue.front());
			acceptQueue.pop();
			if (epollCtl(op, ACCEPT) == -1) {
				// error
			}
		}
		if (!rwQueue.empty()) {
			while (!rwQueue.empty()) {
				std::unique_ptr<IOperation> op = std::move(rwQueue.front());
				rwQueue.pop();
				if (epollCtl(op, READ_WRITE) == -1) {
					// error
				}
			}
		}
		ret = epoll_wait(epollFd, events, MAX_EVENTS, -1);
		callHandler(ret);
	}
}

int Global::Executor::epollCtl(std::unique_ptr<IOperation>& temp, int op_flag) {
	epoll_event event;
	IOperation* op = temp.release();
	event.data.ptr = &op;
	event.events = EPOLLIN | EPOLLET | EPOLLPRI;
	if (op->op() == WRITE) {
		event.events |= EPOLLOUT;
	}
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, op->fd(), &event) == -1) {
		if (errno = EEXIST && op->op() == WRITE) {
			if (epoll_ctl(epollFd, EPOLL_CTL_MOD, op->fd(), &event) == -1) {
				// error
			}
		}
	}
	return 0;
}

void Global::Executor::callHandler(int ret) {
	for (int i = 0; i < ret; i++) {
		std::unique_ptr<IOperation> op(static_cast<IOperation*>(events[i].data.ptr));
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
					// new Socket을 만들어서 보내줘야함! 해당 부분 오류 고칠것!
					BasicSocket newSocket(IoContext(*this));
					newSocket.socket(fd);
					op->acceptHandler(SUCCESS, newSocket);
				}
				continue;
			} else {
				int n = recv(op->fd(), op->buf(), op->len(), 0);
				if (n == -1) {
					op->socketHandler(FAIL, n);
					continue;
				}
				op->socketHandler(SUCCESS, n);
			}
		}
		if (events[i].events & EPOLLOUT) {
			int n = send(op->fd(), op->buf(), op->len(), 0);
			if (n == -1) {
				op->socketHandler(FAIL, n);
				continue;
			}
			op->len(n);
			op->buf(n);
			if (op->len() <= 0) {
				epoll_event ev;
				ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
				epoll_ctl(epollFd, EPOLL_CTL_MOD, op->fd(), &ev);
				op->socketHandler(SUCCESS, n);
				continue;
			}
			rwQueue.push(std::move(op));
		}
	}
}

