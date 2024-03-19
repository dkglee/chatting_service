#ifndef IO_CONTEXT_HPP
# define IO_CONTEXT_HPP

# include <functional>
# include <variant>
# include <queue>
# include <memory>

# include "executor.hpp"

namespace Global {
	class IoContext;
}

class Global::IoContext 
{
private:
	std::shared_ptr<Executor> executor_;
public:
	IoContext();
	IoContext(const IoContext&);

	void setExecutor(std::shared_ptr<Executor> executor);
	std::shared_ptr<Executor> getExecutor();

	template <typename Func>
	void addEvent(int fd, Func handler) {
		executor_->addEvent(fd, handler);
	}

	template <typename Func>
	void addEvent(int fd, char* buf, size_t len, Func handler, int op_flag) {
		executor_->addEvent(fd, buf, len, handler, op_flag);
	}

	~IoContext();
	// single thread run
	void run();
	// multi thread run
	void run(int num);
};


#endif
