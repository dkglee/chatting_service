#include "../library/global_namespace.hpp"
#include "../library/io_context.hpp"
#include "../library/tcp.hpp"
#include "../library/basic_acceptor.hpp"
#include "../library/basic_socket.hpp"
#include "../library/basic_endpoint.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <memory>

class Session : public std::enable_shared_from_this<Session> {
public:
	Session(Global::BasicSocket socket)
		: socket_(std::move(socket))
	{
	}
	~Session() {
		std::cout << "session destructed" << std::endl;
	}
	void start() {
		std::cout << "hello" << std::endl;
		do_read();
	}
private:
	char buf_[1024];
	void do_write(std::size_t bytes) {
		auto self(shared_from_this());
		socket_.async_write(socket_.getSocket(), buf_, bytes, [this, self](int error, int bytes_written){
			(void)bytes_written;
			if (!error) {
				do_read();
			}
		});

	}

	void do_read() {
		std::cout << "buf: " << (void*)buf_ << std::endl;
		auto self(shared_from_this());
		socket_.async_read(buf_, 1024, [this, self](int error, int bytes_read){
			if (!error) {
				do_write(bytes_read);
			}
		});
	}

	Global::BasicSocket socket_;
};

class Server {
public:
	Server(Global::IoContext& io_context, Global::BasicEndpoint& ep)
		: acceptor_(io_context, ep)
	{
		std::cout << "server started" << std::endl;
		start_accept();
	}
	void start_accept() {
		acceptor_.async_accept([this](int error, Global::BasicSocket& socket){
			std::cout << "accept start" << std::endl;
			if (!error) {
				socket.setIoService(acceptor_.getIoService());
				std::make_shared<Session>(socket)->start();
			}
			start_accept();
		});
		std::cout << "accept end" << std::endl;
	}
private:
	Global::BasicAcceptor acceptor_;
};

int main(void) {
	Global::IoContext io_context;
	Global::BasicEndpoint ep(Global::Tcp::v4(), 8080);
	Server server(io_context, ep);
	io_context.run();
	return 0;
}
