#ifndef SERVER_HPP
# define SERVER_HPP

# include "../library/global_namespace.hpp"
# include "../library/io_context.hpp"
# include "../library/tcp.hpp"
# include "../library/basic_acceptor.hpp"
# include "../library/basic_socket.hpp"
# include "../library/basic_endpoint.hpp"

# include "../working_thread/worker.hpp"

# include "./user.hpp"
# include "./session.hpp"

# include <vector>
# include <memory>
# include <unordered_map>
# include <mutex>
# include <string>
# include <algorithm>

class Server {
public:
	Server(Global::IoContext& io_context, Global::BasicEndpoint& ep, int workerNum);
	void start_accept();
	static const std::vector<std::string> getUsersFromChannel(std::string channel);
	static int getSocketFromUser(std::string username);
	static int addUser(std::string username, User user);
	static int removeUser(std::string username);
	static int addUserToChannel(std::string username, std::string channel);
	static int deleteUserFromChannel(std::string username, std::string channel);
private:
	static std::mutex userMutex_;
	static std::mutex channelMutex_;
	static std::unordered_map<std::string, User> users_;
	static std::unordered_map<std::string, std::vector<std::string>> channels_;
	Global::BasicAcceptor acceptor_;
	Worker workerThread_;
	
};

#endif
