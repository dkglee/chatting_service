#ifndef SESSION_HPP
# define SESSION_HPP

# include <memory>
# include <iostream>
# include "./user.hpp"
# include "./parser.hpp"
# include "./product.hpp"
# include "./factory.hpp"
# include "../library/basic_socket.hpp"

class Session : public std::enable_shared_from_this<Session> {
public:
	Session(Global::BasicSocket socket);
	~Session();
	void start();
private:
	char buf_[1024];
	void do_write(std::size_t bytes);
	void do_read();
	bool setUser();
	User user_;
	Global::BasicSocket socket_;
	template <typename Product>
	bool execute(std::unique_ptr<Product> product, User& user) {
		if (product) {
			if (product->execute(user) == -1) {
				std::cout << "product execution failed" << std::endl;
				return false;
			}
			return true;
		} else {
			std::cout << "product is null" << std::endl;
			return false;
		}
	}
};

#endif
