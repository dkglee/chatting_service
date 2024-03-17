#include "./server/server.hpp"

int main(void) {
	Global::IoContext io_context;
	Global::BasicEndpoint ep(Global::Tcp::v4(), 8080);
	Server server(io_context, ep);
	io_context.run(3);
	return 0;
}
