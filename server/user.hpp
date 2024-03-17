#ifndef USER_HPP
# define USER_HPP

# include <string>

class User {
private:
	int socket_;
	std::string nickname_;
	std::string channel_;
public:
	User();
	User(int socket, std::string username);
	User(const User& user);
	~User();
	std::string getNickname();
	std::string getChannel();
	int getSocket();
	void setChannel(std::string channel);
	void setNickname(std::string nickname);
	void setSocket(int fd);
};

#endif
