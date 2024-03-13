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

void Global::Executor::addEvent(int fd, acceptHandler handler) {
	acceptQueue.push(Operation{fd, 0, nullptr, { .acceptHandler_ = handler }, ACCEPT});
}

void Global::Executor::addEvent(int fd, char* buf, size_t len, socketHandler handler, int op_flag) {
	rwQueue.push(Operation{fd, len, buf, { .socketHandler_ = handler }, op_flag});
}

void Global::Executor::executeOne() {
	int ret;
	while (!acceptQueue.empty() && !rwQueue.empty()) {
		if (!acceptQueue.empty()) {
			Operation op = acceptQueue.front();
			acceptQueue.pop();
			if (epollCtl(op, ACCEPT) == -1) {
				// error
			}
		}
		if (!rwQueue.empty()) {
			Operation op = rwQueue.front();
			rwQueue.pop();
			if (epollCtl(op, READ_WRITE) == -1) {
				// error
			}
		}
		ret = epoll_wait(epollFd, events, MAX_EVENTS, -1);
		callHandler(ret);
	}
}

int Global::Executor::epollCtl(Operation& op, int op_flag) {
	epoll_event event;
	event.data.ptr = &op;
	event.events = EPOLLIN | EPOLLET | EPOLLPRI;
	if (op.op_ == WRITE) {
		event.events |= EPOLLOUT;
	}
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, op.fd_, &event) == -1) {
		if (errno = EEXIST && op.op_ == WRITE) {
			if (epoll_ctl(epollFd, EPOLL_CTL_MOD, op.fd_, &event) == -1) {
				// error
			}
		}
	}
	return 0;
}

void Global::Executor::callHandler(int ret) {
	for (int i = 0; i < ret; i++) {
		Operation* op = static_cast<Operation*>(events[i].data.ptr);
		if (events[i].events & EPOLLIN) {
			if (op->op_ == ACCEPT) {
				while (1) {
					int fd = accept(op->fd_, nullptr, nullptr);
					if (fd == -1) {
						if (errno == EAGAIN || errno == EWOULDBLOCK) {
							break;
						}
						BasicSocket null;
						op->handler_.acceptHandler_(FAIL, null);
					}
					// new Socket을 만들어서 보내줘야함! 해당 부분 오류 고칠것!
					BasicSocket newSocket(IoContext(*this));
					newSocket.socket(fd);
					op->handler_.acceptHandler_(SUCCESS, newSocket);
				}
				continue;
			} else {
				int n = recv(op->fd_, op->buf_, op->len_, 0);
				if (n == -1) {
					op->handler_.socketHandler_(FAIL, n);
					continue;
				}
				op->handler_.socketHandler_(SUCCESS, n);
			}
		}
		if (events[i].events & EPOLLOUT) {
			int n = send(op->fd_, op->buf_, op->len_, 0);
			if (n == -1) {
				op->handler_.socketHandler_(FAIL, n);
				continue;
			}
			op->len_ -= n;
			op->buf_ += n;
			if (op->len_ <= 0) {
				epoll_event ev;
				ev.events = EPOLLIN | EPOLLET | EPOLLPRI;
				epoll_ctl(epollFd, EPOLL_CTL_MOD, op->fd_, &ev);
				op->handler_.socketHandler_(SUCCESS, n);
				continue;
			}
			addEvent(op->fd_, op->buf_, op->len_, op->handler_.socketHandler_, WRITE);
		}
	}
}

