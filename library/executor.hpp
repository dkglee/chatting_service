#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

# include "global_namespace.hpp"
# include "schedular.hpp"

namespace Global {
	class Executor;
};

class Global::Executor {
public:
	Executor();
	virtual ~Executor();
	
	template <typename Func>
	void addEvent(int fd, Func handler) {
		schedular_->addEvent(fd, handler);
	}

	template <typename Func>
	void addEvent(int fd, char* buf, size_t len, Func handler, int op_flag) {
		schedular_->addEvent(fd, buf, len, handler, op_flag);
	}

	void execute(int num = 0);
private:
	Executor& operator=(const Executor&) = delete;

	int epollCtl(IOperation* op);
	void callHandler(int ret);
	void delFD(int fd);

	Global::Schedular* schedular_;
};

#endif
