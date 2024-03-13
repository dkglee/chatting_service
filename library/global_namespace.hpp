#ifndef GLOBAL_NAMESPACE_HPP
# define GLOBAL_NAMESPACE_HPP

# include "basic_socket.hpp"
# include <utility>

namespace Global {
	# define READ 0
	# define WRITE 1
	# define ACCEPT 2
	# define READ_WRITE 3
	# define MAX_EVENTS 10
	# define SUCCESS 0
	# define FAIL 1


	typedef void (*acceptHandler)(int&&, BasicSocket&);
	typedef void (*socketHandler)(int, size_t);

	struct Operation {
		int fd_;
		size_t len_;
		char* buf_;
		union Handler {
			acceptHandler acceptHandler_;
			socketHandler socketHandler_;
		} handler_;
		int op_; 
	};
};

#endif
