#ifndef PRODUCT_MSG_HPP
# define PRODUCT_MSG_HPP

# include "./product.hpp"
# include <memory>
# include <string>

class Session;

class ProductMsg : public Product {
private:
	std::string data_;
	void broadcastMessage(Session* thisPtr, std::shared_ptr<Session> selfPtr, const std::vector<std::string>& users, const std::string& msg);
public:
	ProductMsg(std::string data);
	virtual ~ProductMsg();
	virtual int execute(Session* thisPtr, std::shared_ptr<Session> self, User& user);
};

#endif
