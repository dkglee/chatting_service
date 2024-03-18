#ifndef PRODUCT_PRIVATE_MSG_HPP
# define PRODUCT_PRIVATE_MSG_HPP

# include "./product.hpp"
# include <memory>
# include <string>

class Session;

class ProductPrivateMsg : public Product {
private:
	std::string data_;
public:
	ProductPrivateMsg(std::string data);
	virtual ~ProductPrivateMsg();
	virtual int execute(Session* thisPtr, std::shared_ptr<Session> self, User& user);
};

#endif
