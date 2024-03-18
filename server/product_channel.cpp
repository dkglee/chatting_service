#include "../library/global_namespace.hpp"
#include "./product_channel.hpp"
#include "./session.hpp"
#include "./server.hpp"

ProductChannel::ProductChannel(std::string data)
	: data_(data) 
{
}

ProductChannel::~ProductChannel() 
{
}

int ProductChannel::execute(Session* thisPtr, std::shared_ptr<Session> selfPtr, User& user) 
{
	std::string channel = this->data_.substr(0, this->data_.find('\n'));
	std::cout << "channel: " << channel << std::endl;
	std::cout << "user channel:" << user.getChannel() << std::endl;
	if (Server::deleteUserFromChannel(user.getNickname(), user.getChannel()) == -1) {
		std::cout << "Error: could not delete user from channel(" << user.getChannel() << ")" << std::endl;
		return -1;
	}
	user.setChannel(channel);
	if (Server::addUserToChannel(user.getNickname(), channel) == -1) {
		std::cout << "Error: could not add user to channel(" << channel << ")" << std::endl;
		return -1;
	}
	std::string message = user.getNickname() + " has joined " + channel + "\n";
	thisPtr->setBuf(message);
	Global::async_write(user.getSocket(), thisPtr->getBuf(), message.size(), [thisPtr, selfPtr](int error, int bytes_written){
		(void)bytes_written;
		if (!error) {
			thisPtr->do_read();
		}
	});
	return 0;
}

// buffer를 먼저 복사해서 가져갈까?
