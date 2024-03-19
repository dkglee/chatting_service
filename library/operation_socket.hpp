#ifndef OPERATION_SOCKET_HPP
# define OPERATION_SOCKET_HPP

# include "./ioperation.hpp"

namespace Global {
	template <typename Func>
	class OperationSocket;
};

template <typename Func>
class Global::OperationSocket : public IOperation {
public:
	int fd_;
	int len_;
	char* buf_;
	Func handler_;
	int op_;

	OperationSocket(int fd, size_t len, char* buf, Func handler, int op)
		: fd_(fd), len_(len), buf_(buf), handler_(handler), op_(op) {}

	virtual int fd() override {
		return fd_;
	}

	virtual int op() override {
		return op_;
	}

	virtual char* buf() override {
		return buf_;
	}

	virtual int len() override {
		return len_;
	}

	virtual void aluLen(int n) override {
		len_ -= n;
	}

	virtual void aluBuf(int n) override {
		buf_ += n;
	}

	virtual void acceptHandler(int error, BasicSocket& socket) {
		(void)error;
		(void)socket;
	}

	virtual void socketHandler(int error, int byte) {
		handler_(error, byte);
	}
};

#endif
