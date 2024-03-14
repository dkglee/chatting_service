#ifndef GLOBAL_NAMESPACE_HPP
# define GLOBAL_NAMESPACE_HPP

#include <iostream>

// #include "executor.hpp"
// #include "io_context.hpp"
// #include "service.hpp"
// #include "tcp.hpp"
// #include "basic_endpoint.hpp"
// #include "basic_socket.hpp"
// #include "basic_acceptor.hpp"

namespace Global {
	#define READ 0
	#define WRITE 1
	#define ACCEPT 2
	#define READ_WRITE 3
	#define MAX_EVENTS 10
	#define SUCCESS 0
	#define FAIL 1

	// std::pair<char*, int> make_pair(char* buf, int len) {
	// 	return std::make_pair(buf, len);
	// }

	class BasicSocket;
	class IOperation {
	public:
		virtual ~IOperation() {}
		virtual int fd() = 0;
		virtual int op() = 0;
		virtual char* buf() = 0;
		virtual int len() = 0;
		virtual void aluLen(int n) = 0;
		virtual void aluBuf(int n) = 0;
		virtual void acceptHandler(int, BasicSocket&) = 0;
		virtual void socketHandler(int, int) = 0;
	};

	template <typename Func>
	class OperationAccept : public IOperation {
	public:
		int fd_;
		int len_;
		char* buf_;
		Func handler_;
		int op_;

		OperationAccept(int fd, size_t len, char* buf, Func handler, int op)
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
			// std::cout << "acceptHandler in OperationAccept" << std::endl;
			handler_(error, socket);
		}

		virtual void socketHandler(int error, int byte) {
			(void)error;
			(void)byte;
			// handler_(error, byte);
		}
	};
	
	template <typename Func>
	class OperationSocket : public IOperation {
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
			// handler_(error, socket);
		}

		virtual void socketHandler(int error, int byte) {
			handler_(error, byte);
		}
	};

};

#endif
