#include "../library/io_context.hpp"
#include "../library/tcp.hpp"

class Session {
public:
	Session(Global::Tcp::socket socket)
		: socket_(std::move(socket))
	{
	}
	void start() {
		do_read();
	}
private:
	char buf_[1024];
	void do_write(std::size_t bytes) {
		socket_.async_write(socket_.socket(), buf_, bytes, [this](int error, std::size_t bytes_written){
			if (!error) {
				do_read();
			}
		});
	
	}
	void do_read() {
		socket_.async_read(buf_, 1024, [this](int error, std::size_t bytes_read){
			if (!error) {
				do_write(bytes_read);
			}
		})
	}
	Global::Tcp::socket socket_;
};

class Server {
public:
	Server(Global::IoContext& io_context, unsigned short port)
		: acceptor_(io_context, Global::Tcp::endpoint(Global::Tcp::v4(), port))
	{
		start_accept();
	}
	void start_accept() {
		acceptor_.async_accept([](int&& error, Global::Tcp::socket& socket){
			if (!error) {
				std::make_shared<Session>(socket)->start();
			}
		});
		start_accept();
	}
private:
	Global::Tcp::acceptor acceptor_;
};

int main(void) {
	Global::IoContext io_context;
	Server server(io_context, 8080);
	io_context.run();
	return 0;
}
