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


void Session::setUser() {
	auto self(shared_from_this());
	socket_.async_read(buf_, 1024, [this, self](int error, int bytes_read){
		this->setUserHandler(self, error, bytes_read);
	});
}

void Session::do_read() {
	auto self(shared_from_this());
	socket_.async_read(buf_, 1024, [this, self](int error, int bytes_read){
		this->readHandler(self, error, bytes_read);
	});
}

void Session::setUserHandler(std::shared_ptr<Session> self, int error, int bytes_read) {
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
}

void Session::readHandler(std::shared_ptr<Session> self, int error, int bytes_read) {
	if (!error) {
		{
			std::lock_guard<std::mutex> lock(Global::workerManager.mtx);
			Global::workerManager.workQueue.push(new Global::WorkerTask(bytes_read, [this, self](int bytes_read) -> void {
				auto data = Parser::parseForMessage(buf_, bytes_read);
				auto product = Factory<Parser::ParseData, Product>::createProduct(data);
				if (!(this->execute(this, self, std::move(product), this->user_))) {
					this->do_read();
				}
			}));
		}
		Global::workerManager.cv.notify_one();
	} else {
		std::cout << "user disconnected" << std::endl;
		Server::removeUser(this->user_.getNickname());
	}
	
}
