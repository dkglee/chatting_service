#include "parser.hpp"

Parser::Parser() {
}

Parser::~Parser() {
}

Parser::ParseData Parser::parseForLogin(char* buf, int bytes_read) {
	std::string command("login");
	std::string message;
	int i = 0;
	while (i < bytes_read) {
		message += buf[i];
		i++;
	}
	return std::make_pair(command, message);
};

Parser::ParseData Parser::parseForMessage(char* buf, int bytes_read) {
	std::string command;
	std::string message;
	int i = 0;
	if (buf[0] == '/') {
		while (buf[i] != ' ' && i < bytes_read && buf[i] != '\n') {
			command += buf[i];
			i++;
		}
		i++;
	} else {
		command = "message";
	}
	while (i < bytes_read) {
		message += buf[i];
		i++;
	}
	if (message.size() == 0) {
		message = "none";
	}
	return std::make_pair(command, message);
};
