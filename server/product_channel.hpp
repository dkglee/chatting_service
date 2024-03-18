#ifndef PRODUCT_CHANNEL_HPP
# define PRODUCT_CHANNEL_HPP

# include "./product.hpp"
# include <memory>
# include <string>

class Session;

class ProductChannel : public Product {
private:
	std::string data_;
public:
	ProductChannel(std::string data);
	virtual ~ProductChannel();
	virtual int execute(Session* thisPtr, std::shared_ptr<Session> self, User& user);
};

#endif
