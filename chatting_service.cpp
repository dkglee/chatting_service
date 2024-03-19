#include "./server/server.hpp"
#include "./server/factory.hpp"
#include "./server/product.hpp"
#include "./server/product_login.hpp"
#include "./server/product_msg.hpp"
#include "./server/product_private_msg.hpp"
#include "./server/product_channel.hpp"
#include "./server/product_quit.hpp"
#include <string>
#include <utility>

void init_factory() {
	typedef std::pair<std::string, std::string> ParseData;
	Factory<ParseData, Product>::registerFactory("login", [](std::string data) -> std::unique_ptr<Product> {
		return std::make_unique<ProductLogin>(data);
	});
	Factory<ParseData, Product>::registerFactory("message", [](std::string data) -> std::unique_ptr<Product> {
		return std::make_unique<ProductMsg>(data);
	});
	Factory<ParseData, Product>::registerFactory("/msg", [](std::string data) -> std::unique_ptr<Product> {
		return std::make_unique<ProductPrivateMsg>(data);
	});
	Factory<ParseData, Product>::registerFactory("/channel", [](std::string data) -> std::unique_ptr<Product> {
		return std::make_unique<ProductChannel>(data);
	});
	Factory<ParseData, Product>::registerFactory("/quit", [](std::string data) -> std::unique_ptr<Product> {
		return std::make_unique<ProductQuit>(data);
	});
}

int main(void) {
	Global::IoContext io_context;
	Global::BasicEndpoint ep(Global::Tcp::v4(), 8080);
	init_factory();
	Server server(io_context, ep, 3);
	io_context.run(3);
	return 0;
}
