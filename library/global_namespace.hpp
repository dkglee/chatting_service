#ifndef GLOBAL_NAMESPACE_HPP
# define GLOBAL_NAMESPACE_HPP

#include <iostream>
#include <queue>
#include <mutex>
#include <thread>

namespace Global {
	class IOperation;
	struct Thread
	{
		std::mutex mtx;
		std::queue<IOperation*> rwQueue;
	};
	inline Thread shm;
};

namespace Global {
	#define READ 0
	#define WRITE 1
	#define ACCEPT 2
	#define READ_WRITE 3
	#define MAX_EVENTS 10
	#define SUCCESS 0
	#define FAIL 1
	#define CLOSE 2

	
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
			handler_(error, socket);
		}

		virtual void socketHandler(int error, int byte) {
			(void)error;
			(void)byte;
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
		}

		virtual void socketHandler(int error, int byte) {
			handler_(error, byte);
		}
	};
	// 1. 전역으로 가져갈 수 있나? // 전역으로 가져가야만 함.
	// 
	// Thread shm;
	// class Schedular;

	template <typename Func>
	void async_write(int fd, char* buf, size_t len, Func handler) {
		// Schedular schedular_;
		// schedular_.addEvent(fd, buf, len, handler, WRITE);
		std::lock_guard<std::mutex> lock(shm.mtx);
		shm.rwQueue.push(new OperationSocket(fd, len, buf, handler, WRITE));
	}
};


#endif
