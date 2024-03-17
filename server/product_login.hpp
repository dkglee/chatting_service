#ifndef PRODUCT_LOGIN_HPP
# define PRODUCT_LOGIN_HPP

# include "./product.hpp"
# include <string>

class ProductLogin : public Product {
private:
	std::string data_;
public:
	ProductLogin(std::string data);
	virtual ~ProductLogin();
	virtual int execute(User& user);
};

#endif
