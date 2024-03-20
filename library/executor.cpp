#include "executor.hpp"
#include "basic_socket.hpp"
#include <errno.h>
#include <unistd.h>
#include "round_robin_schedular.hpp"

Global::Executor::Executor()
	: schedular_(new Global::RoundRobinSchedular())
{
}

Global::Executor::~Executor() {
	delete schedular_;
}

void Global::Executor::execute(int num) {
	schedular_->runOneOrAll(num);
}
