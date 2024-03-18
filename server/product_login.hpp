#ifndef PRODUCT_LOGIN_HPP
# define PRODUCT_LOGIN_HPP

# include "./product.hpp"
# include <string>
# include <memory>

class ProductLogin : public Product {
private:
	std::string data_;
public:
	ProductLogin(std::string data);
	virtual ~ProductLogin();
	virtual int execute(Session* thisPtr, std::shared_ptr<Session> self, User& user);
};

#endif
