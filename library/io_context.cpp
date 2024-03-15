#include "io_context.hpp"

Global::IoContext::IoContext()
	: executor_(std::make_shared<Global::Executor>())
{
	std::cout << "I'm Born" << std::endl;
}

// Global::IoContext::IoContext(Global::Executor& executor)
// 	: executor_(&executor)
// {}

Global::IoContext::IoContext(const Global::IoContext& io_context)
{
	executor_ = io_context.executor_;
}

Global::IoContext::~IoContext() {
	std::cout << "why? I'm Died? Count: ";
	std::cout << executor_.use_count() << std::endl;
}

void Global::IoContext::run() {
	executor_->executeOne();
}

void Global::IoContext::setExecutor(std::shared_ptr<Global::Executor> executor) {
	executor_ = executor;
}

std::shared_ptr<Global::Executor> Global::IoContext::getExecutor() {
	return executor_;
}
