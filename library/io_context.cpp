#include "io_context.hpp"

Global::IoContext::IoContext()
	: executor_(new Global::Executor())
{}

Global::IoContext::IoContext(Global::Executor& executor)
	: executor_(&executor)
{}

Global::IoContext::~IoContext() {
	delete executor_;
}

void Global::IoContext::addEvent(int fd, acceptHandler handler) {
	executor_->addEvent(fd, handler);
}

void Global::IoContext::addEvent(int fd, char* buf, size_t len, socketHandler handler, int op_flag) {
	executor_->addEvent(fd, buf, len, handler, op_flag);
}

void Global::IoContext::run() {
	executor_->executeOne();
}
