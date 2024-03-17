#include "user.hpp"

User::User() 
{
}

User::User(int socket, std::string username)
	: socket_(socket), nickname_(username), channel_("") 
{
}

User::User(const User& user)
	: socket_(user.socket_), nickname_(user.nickname_), channel_(user.channel_) 
{
}

User::~User() 
{
}

std::string User::getNickname() 
{
	return nickname_;
}

std::string User::getChannel() 
{
	return channel_;
}

int User::getSocket() 
{
	return socket_;
}

void User::setChannel(std::string channel) 
{
	channel_ = channel;
}

void User::setNickname(std::string nickname) 
{
	nickname_ = nickname;
}

void User::setSocket(int fd) 
{
	socket_ = fd;
}
