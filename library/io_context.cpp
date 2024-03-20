#include "io_context.hpp"

Global::IoContext::IoContext()
	: executor_(std::make_shared<Global::Executor>())
{
	// std::cout << "Shared pointer count(CONSTRUCTOR): ";
	// std::cout << executor_.use_count() << std::endl;
}

Global::IoContext::IoContext(const Global::IoContext& io_context)
{
	executor_ = io_context.executor_;
	// std::cout << "Shared pointer count(GLOBAL): ";
	// std::cout << executor_.use_count() << std::endl;
}

Global::IoContext::~IoContext() {
	// std::cout << "Shared pointer count(~DESTRUCTOR): ";
	// std::cout << executor_.use_count() << std::endl;
}

void Global::IoContext::run() {
	executor_->execute();
}

void Global::IoContext::run(int num) {
	executor_->execute(num);
}

void Global::IoContext::setExecutor(std::shared_ptr<Global::Executor> executor) {
	executor_ = executor;
	// std::cout << "Shared pointer count(SET): ";
	// std::cout << executor_.use_count() << std::endl;
}

std::shared_ptr<Global::Executor> Global::IoContext::getExecutor() {
	return executor_;
}
