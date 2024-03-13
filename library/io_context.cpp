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

void Global::IoContext::run() {
	executor_->executeOne();
}
