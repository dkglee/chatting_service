#ifndef IO_CONTEXT_HPP
# define IO_CONTEXT_HPP

# include <functional>
# include <variant>
# include <queue>
# include <memory>

# include "executor.hpp"
# include "global_namespace.hpp"

namespace Global {
	class IoContext;
}

class Global::IoContext 
{
public:
	IoContext()
		: executor_(new Global::Executor())
	{
	}
	IoContext(Global::Executor& executor)
		: executor_(&executor)
	{
	}
	void addEvent(int fd, acceptHandler handler) {
		executor_->addEvent(fd, handler);
	}
	void addEvent(int fd, char* buf, size_t len, socketHandler handler, int op_flag) {
		executor_->addEvent(fd, buf, len, handler, op_flag);
	}
	~IoContext() {
		delete executor_;
	}
	void run();
	void run(int num);
private:
	Global::Executor* executor_;
};

#endif
