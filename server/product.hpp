#ifndef PRODUCT_HPP
# define PRODUCT_HPP

# include <string>
# include <user.hpp>

class Product {
public:
	Product();
	virtual ~Product();
	virtual int execute(User&) = 0;
};

#endif
