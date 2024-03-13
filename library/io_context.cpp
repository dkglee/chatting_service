#include "io_context.hpp"

void Global::IoContext::run() {
	executor_->executeOne();
}
