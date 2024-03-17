#include "./server.hpp"

Server::Server(Global::IoContext& io_context, Global::BasicEndpoint& ep)
	: acceptor_(io_context, ep)
{
	std::cout << "server started" << std::endl;
	start_accept();
}

void Server::start_accept() {
	acceptor_.async_accept([this](int error, Global::BasicSocket& socket){
		std::cout << "accept start" << std::endl;
		if (!error) {
			socket.setIoService(acceptor_.getIoService());
			std::make_shared<Session>(socket)->start();
		}
		start_accept();
	});
	std::cout << "accept end" << std::endl;
}

const std::vector<std::string> Server::getUsersFromChannel(std::string channel) {
	std::lock_guard<std::mutex> lock(userMutex_);
	std::vector<std::string> users;
	auto it = channels_.find(channel);
	if (it != channels_.end()) {
		users = it->second;
	}
	return users;
}

int Server::getSocketFromUser(std::string username) {
	std::lock_guard<std::mutex> lock(userMutex_);
	auto it = users_.find(username);
	if (it != users_.end()) {
		return it->second.getSocket();
	} else {
		std::cout << "Error: could not find user" << std::endl;
	}
	return -1;
}

int Server::addUser(std::string username, User user) {
	std::lock_guard<std::mutex> lock(userMutex_);
	auto it = users_.find(username);
	if (it == users_.end()) {
		users_.insert(std::make_pair(username, user));
		return 0;
	} else {
		std::cout << "Error: user already exists" << std::endl;
	}
	return -1;
}

int Server::removeUser(std::string username) {
	std::lock_guard<std::mutex> lock(userMutex_);
	auto it = users_.find(username);
	if (it != users_.end()) {
		users_.erase(it);
		return 0;
	} else {
		std::cout << "Error: could not find user" << std::endl;
	}
	return -1;
}

int Server::addUserToChannel(std::string username, std::string channel) {
	std::lock_guard<std::mutex> lock(channelMutex_);
	auto it = channels_.find(channel);
	if (it != channels_.end()) {
		it->second.push_back(username);
	} else {
		std::cout << "creating new channel" << std::endl;
		std::vector<std::string> users;
		users.push_back(username);
		channels_.insert(std::make_pair(channel, users));
	}
	return 0;
}

int Server::deleteUserFromChannel(std::string username, std::string channel) {
	std::lock_guard<std::mutex> lock(channelMutex_);
	auto chnIt = channels_.find(channel);
	if (chnIt != channels_.end()) {
		auto usrIt = std::find(chnIt->second.begin(), chnIt->second.end(), username);
		if (usrIt != chnIt->second.end()) {
			chnIt->second.erase(usrIt);
			return 0;
		}
		std::cout << "Error: could not find user in channel" << std::endl;
		return -1;
	}
	std::cout << "Error: could not find channel" << std::endl;
	return -1;
}
