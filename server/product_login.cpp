#include "./product_login.hpp"
#include "./server.hpp"
#include "./session.hpp"

ProductLogin::ProductLogin(std::string data)
	: data_(data) 
{
}

ProductLogin::~ProductLogin() 
{
}

int ProductLogin::execute(Session* thisPtr, std::shared_ptr<Session> selfPtr, User& user) 
{
	std::string username = this->data_.substr(0, this->data_.find('\n'));
	std::string channel("general");
	user.setNickname(username);
	user.setChannel(channel);
	if (Server::addUser(username, user) == -1) {
		std::cout << "Error: could not add user to server" << std::endl;
		return -1;
	}
	if (Server::addUserToChannel(username, channel) == -1) {
		std::cout << "Error: could not add user to channel(general)" << std::endl;
		return -1;
	}
	std::string message = username + " has joined " + channel + "\n";
	thisPtr->setBuf(message);
	Global::async_write(user.getSocket(), thisPtr->getBuf(), message.size(), [thisPtr, selfPtr](int error, int bytes_written){
		(void)bytes_written;
		if (!error) {
			thisPtr->do_read();
		}
	});
	return 0;
}
