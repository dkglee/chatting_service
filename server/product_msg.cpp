#include "../library/global_namespace.hpp"
#include "./session.hpp"
#include "./server.hpp"
#include "./product_msg.hpp"

ProductMsg::ProductMsg(std::string data)
	: data_(data) 
{
}

ProductMsg::~ProductMsg() 
{
}

void ProductMsg::broadcastMessage(Session* thisPtr, std::shared_ptr<Session> selfPtr, const std::vector<std::string>& users, const std::string& msg) 
{
	int i = 0;
	for (auto userIt : users) {
		int targetFd = Server::getSocketFromUser(userIt);
		if (targetFd == -1) {
			std::cout << "Error: could not find user(" << userIt << ")" << std::endl;
			return;
		}
		thisPtr->setBuf(msg);
		Global::async_write(targetFd, thisPtr->getBuf(), msg.size(), [thisPtr, selfPtr, i](int error, int bytes_written){
			(void)bytes_written;
			if (!error) {
				if (i == 0)
					thisPtr->do_read();
			}
		});
		i++;
	}
}


int ProductMsg::execute(Session* thisPtr, std::shared_ptr<Session> selfPtr, User& user) 
{
	std::string msg = user.getNickname() + ": " + this->data_.substr(0, this->data_.find('\n')) + "\n";
	std::cout << msg;
	const std::vector<std::string> users = Server::getUsersFromChannel(user.getChannel());
	this->broadcastMessage(thisPtr, selfPtr, users, msg);
	return 0;
}
