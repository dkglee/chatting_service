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
	IoContext();
	IoContext(Global::Executor& executor);
	void addEvent(int fd, acceptHandler handler);
	void addEvent(int fd, char* buf, size_t len, socketHandler handler, int op_flag);
	virtual ~IoContext();
	// single thread run
	void run();
	// multi thread run
	void run(int num);
private:
	Global::Executor* executor_;
};

#endif
