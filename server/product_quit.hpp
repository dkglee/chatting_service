#ifndef PRODUCT_QUIT_HPP
# define PRODUCT_QUIT_HPP

# include "./product.hpp"
# include <memory>
# include <string>

class ProductQuit : public Product {
private:
	std::string data_;
public:
	ProductQuit(std::string data);
	~ProductQuit();
	int execute(Session* thisPtr, std::shared_ptr<Session> selfPtr, User& user);
};

#endif
