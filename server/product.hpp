#ifndef PRODUCT_HPP
# define PRODUCT_HPP

# include <string>
# include <memory>
# include "user.hpp"

class Session;

class Product {
public:
	Product();
	virtual ~Product();
	virtual int execute(Session* thisPtr, std::shared_ptr<Session> self, User& user) = 0;
};

#endif
