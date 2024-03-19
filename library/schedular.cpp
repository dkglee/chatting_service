#include "schedular.hpp"
#include <iostream>

Global::Schedular::Schedular()
	: epollFd(epoll_create1(0)), running(false)
{
	Global::evfd = eventfd(0, EFD_NONBLOCK);
	epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = Global::evfd;
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, Global::evfd, &ev) == -1) {
		std::cerr << "epoll_ctl: eventfd" << std::endl;
	}
}

Global::Schedular::~Schedular() {
	close(Global::evfd);
	close(epollFd);
}

int Global::Schedular::rwqueueEmpty() {
	std::lock_guard<std::mutex> lock(shm.mtx);
	return shm.rwQueue.empty();
}

Global::IOperation* Global::Schedular::rwpopEvent() {
	IOperation* ret;
	std::lock_guard<std::mutex> lock(shm.mtx);
	ret = shm.rwQueue.front();
	shm.rwQueue.pop();
	return ret;
}

void Global::Schedular::rwpushEvent(IOperation* op) {
	std::lock_guard<std::mutex> lock(shm.mtx);
	shm.rwQueue.push(op);
}

void Global::Schedular::acceptPushEvent(IOperation* op) {
	std::lock_guard<std::mutex> lock(acceptMtx);
	acceptQueue.push(op);
}

int Global::Schedular::getEfd() {
	return epollFd;
}

void Global::Schedular::setRunning(bool flag) {
	running = flag;
}

bool Global::Schedular::getRunning() {
	return running;
}

bool Global::Schedular::acceptQueueEmpty() {
	std::lock_guard<std::mutex> lock(acceptMtx);
	return acceptQueue.empty();
}

Global::IOperation* Global::Schedular::accPopEvent() {
	IOperation* ret;
	std::lock_guard<std::mutex> lock(acceptMtx);
	ret = acceptQueue.front();
	acceptQueue.pop();
	return ret;
}
