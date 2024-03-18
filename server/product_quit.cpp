#include "../library/global_namespace.hpp"
#include "./server.hpp"
#include "./session.hpp"
#include "./product_quit.hpp"

ProductQuit::ProductQuit(std::string data)
	: data_(data) 
{
}

ProductQuit::~ProductQuit() 
{
}

int ProductQuit::execute(Session* thisPtr, std::shared_ptr<Session> selfPtr, User& user) 
{
	std::string message = user.getNickname() + " has left the channel(" + user.getChannel() + ")\n";
	if (Server::deleteUserFromChannel(user.getNickname(), user.getChannel()) == -1) {
		std::cout << "Error: could not delete user from channel(" << user.getChannel() << ")" << std::endl;
		return -1;
	}
	if (Server::addUserToChannel(user.getNickname(), "general") == -1) {
		std::cout << "Error: could not add user to channel(general)" << std::endl;
		return -1;
	}
	user.setChannel("general");
	thisPtr->setBuf(message);
	Global::async_write(user.getSocket(), thisPtr->getBuf(), message.size(), [thisPtr, selfPtr](int error, int bytes_written){
		(void)bytes_written;
		if (!error) {
			thisPtr->do_read();
		}
	});

	return 0;
}
