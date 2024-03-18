#include "./session.hpp"
#include "./server.hpp"

Session::Session(Global::BasicSocket socket)
	: socket_(std::move(socket)), user_()
{
}

Session::~Session() {
	std::cout << "session destructed" << std::endl;
}

void Session::start() {
	// if (this->setUser()) {
	// 	this->do_read();
	// } else {
	// 	// close();
	// }
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
		if (error == SUCCESS) {
			this->user_.setSocket(socket_.getSocket());
			auto data = Parser::parseForLogin(buf_, bytes_read);
			auto product = Factory<Parser::ParseData, Product>::createProduct(data);
			if (!(this->execute(this, self, std::move(product), this->user_))) {
				// for error read again from client
				std::cout << "setUser failed" << std::endl;
				this->setUser();
			}
		} else {
			std::cout << "user disconnected" << std::endl;
		}
	});
}

void Session::do_write(std::size_t bytes) {
	auto self(shared_from_this());
	Global::async_write(socket_.getSocket(), buf_, bytes, [this, self](int error, int bytes_written){
		(void)bytes_written;
		if (!error) {
			do_read();
		}
	});
}

void Session::do_read() {
	auto self(shared_from_this());
	socket_.async_read(buf_, 1024, [this, self](int error, int bytes_read){
		if (!error) {
			auto data = Parser::parseForMessage(buf_, bytes_read);
			auto product = Factory<Parser::ParseData, Product>::createProduct(data);
			if (!(this->execute(this, self, std::move(product), this->user_))) {
				// for error read again from client
				this->do_read();
			}
		} else {
			std::cout << "user disconnected" << std::endl;
			Server::removeUser(this->user_.getNickname());
		}
	});
}

// self 되어 있으면 아직 살아있음. 핸들러 내부에서 ㅎ
// 다른 스레드에서 핸들러를 넣고 끝나게 되면, 다른 스레드에서 핸들러를 호출할 수 있지. 왜냐하면 thread가 동적 할당해서 쓰기 작업을 집어넣은 거거든

// 남은 기능 - /channel | /msg | and just message
