#ifndef GLOBAL_NAMESPACE_HPP
# define GLOBAL_NAMESPACE_HPP

# include "basic_socket.hpp"
# include <utility>

namespace Global {
	# define READ 0
	# define WRITE 1
	# define ACCEPT 2
	# define READ_WRITE 3
	# define MAX_EVENTS 10
	# define SUCCESS 0
	# define FAIL 1


	// typedef void (*acceptHandler)(int&&, BasicSocket&);
	// typedef void (*socketHandler)(int, size_t);
	// socketHandler socketHandler_;

	class IOperation {
	public:
		virtual ~IOperation() {}
		// virtual void execute() = 0;
		virtual int fd() = 0;
		virtual int op() = 0;
		virtual char* buf(int n = 0) = 0;
		virtual int len(int n = 0) = 0;
		virtual void acceptHandler(int, BasicSocket&) = 0;
		virtual void socketHandler(int, size_t) = 0;
	};

	template <typename Func>
	class Operation : public IOperation{
	public:
		int fd_;
		size_t len_;
		char* buf_;
		Func handler_;
		int op_;

		Operation(int fd, size_t len, char* buf, Func handler, int op)
			: fd_(fd), len_(len), buf_(buf), handler_(handler), op_(op) {}
		
		virtual int fd() override {
			return fd_;
		}

		virtual int op() override {
			return op_;
		}

		virtual char* buf(int n = 0) override {
			return (buf_ += n);
		}

		virtual int len(int n = 0) override {
			return (len_ += n);
		}

		virtual void acceptHandler(int error, BasicSocket& socket) {
			handler_(error, socket);
		}

		virtual void socketHandler(int error, size_t socket) {
			handler_(error, socket);
		}
	};
};

#endif
