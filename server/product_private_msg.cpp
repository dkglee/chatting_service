#include "../library/global_namespace.hpp"
#include "./session.hpp"
#include "./server.hpp"
#include "./product_private_msg.hpp"

ProductPrivateMsg::ProductPrivateMsg(std::string data)
	: data_(data) 
{
}

ProductPrivateMsg::~ProductPrivateMsg() 
{
}

int ProductPrivateMsg::execute(Session* thisPtr, std::shared_ptr<Session> selfPtr, User& user) 
{
	std::string nickname = this->data_.substr(0, this->data_.find(' '));
	std::string message = this->data_.substr(this->data_.find(' ') + 1, this->data_.find('\n'));
	std::string sender = user.getNickname();
	std::string buf = sender + " -> " + nickname + " : " + message;
	std::cout << buf << std::endl;
	int targetFd = Server::getSocketFromUser(nickname);
	if (targetFd == -1) {
		std::cout << "Error: could not find user(" << nickname << ")" << std::endl;
		return -1;
	}
	thisPtr->setBuf(buf);
	Global::async_write(targetFd, thisPtr->getBuf(), buf.size(), [thisPtr, selfPtr](int error, int bytes_written){
		(void)bytes_written;
		if (!error) {
			thisPtr->do_read();
		}
	});
	return 0;
}
