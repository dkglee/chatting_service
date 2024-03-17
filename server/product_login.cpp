#include "./product_login.hpp"
#include "./server.hpp"

ProductLogin::ProductLogin(std::string data)
	: data_(data) 
{
}

ProductLogin::~ProductLogin() 
{
}

int ProductLogin::execute(User& user) 
{
	std::string username = data_.substr(0, data_.find(' '));
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
	return 0;
}
