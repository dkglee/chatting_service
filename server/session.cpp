#include "./session.hpp"
#include "./server.hpp"
#include "../library/global_namespace.hpp"
#include "../working_thread/worker.hpp"

Session::Session(Global::BasicSocket socket)
	: socket_(std::move(socket)), user_()
{
}

Session::~Session() {
	std::cout << "session destructed" << std::endl;
}

void Session::start() {
	this->setUser();
}

void Session::setBuf(std::string buf) {
	int i = 0;
	for (auto it : buf) {
		buf_[i++] = it;
	}
}

char* Session::getBuf() {
	return buf_;
}

// 작업을 집어넣을 때,, 뭐가 필요한 것인가.
// this, self를 스레드로 넘기게 되면 해제가 될까..?
void Session::setUser() {
	auto self(shared_from_this());
	socket_.async_read(buf_, 1024, [this, self](int error, int bytes_read){
		if (error == SUCCESS) {
			{
				std::cout << "Create worker task\n";
				std::lock_guard<std::mutex> lock(Global::workerManager.mtx);
				Global::workerManager.workQueue.push(new Global::WorkerTask(bytes_read, [this, self](int bytes_read) -> void {
					this->user_.setSocket(socket_.getSocket());
					auto data = Parser::parseForLogin(buf_, bytes_read);
					auto product = Factory<Parser::ParseData, Product>::createProduct(data);
					if (!(this->execute(this, self, std::move(product), this->user_))) {
						std::cout << "setUser failed" << std::endl;
						this->setUser();
					}
				}));
			}
			Global::workerManager.cv.notify_one();
		} else {
			std::cout << "user disconnected" << std::endl;
		}
	});
}

// void Session::do_write(std::size_t bytes) {
// 	auto self(shared_from_this());
// 	Global::async_write(socket_.getSocket(), buf_, bytes, [this, self](int error, int bytes_written){
// 		(void)bytes_written;
// 		if (!error) {
// 			do_read();
// 		}
// 	});
// }

void Session::do_read() {
	auto self(shared_from_this());
	socket_.async_read(buf_, 1024, [this, self](int error, int bytes_read){
		if (!error) {
			{
				std::lock_guard<std::mutex> lock(Global::workerManager.mtx);
				Global::workerManager.workQueue.push(new Global::WorkerTask(bytes_read, [this, self](int bytes_read) -> void {
					auto data = Parser::parseForMessage(buf_, bytes_read);
					auto product = Factory<Parser::ParseData, Product>::createProduct(data);
					if (!(this->execute(this, self, std::move(product), this->user_))) {
						// for error read again from client
						this->do_read();
					}
				}));
			}
			Global::workerManager.cv.notify_one();
		} else {
			std::cout << "user disconnected" << std::endl;
			Server::removeUser(this->user_.getNickname());
		}
	});
}

// async write를 보내긴 했지만 로직상.. 입력을 받아들이는 thread가 epoll상태라서 무한 대기중..
// 해결 방법
// 1. epoll에 타입을 두는 것
// 2. write 작업을 epoll 후에 넣는것.
// 3. eventfd - 간단한 이벤트를 알리는 것에 특화되어 있음. 즉, 해당 문제에서는 write event를 발생시키는 것만으로도 도움이 됨.
// 4. signalfd
// 5. timerfd

// self 되어 있으면 아직 살아있음. 핸들러 내부에서 ㅎ
// 다른 스레드에서 핸들러를 넣고 끝나게 되면, 다른 스레드에서 핸들러를 호출할 수 있지. 왜냐하면 thread가 동적 할당해서 쓰기 작업을 집어넣은 거거든

// 남은 기능 - /channel | /msg | and just message
