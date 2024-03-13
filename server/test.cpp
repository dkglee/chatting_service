#include <iostream>

template <typename Func>
struct Handler {
	Func func_;
};

int main(void) {
	int x = 3;

	auto handler = [x]() {
		std::cout << "hello: " << x << std::endl;
	};
	Handler<decltype(handler)> h{handler};
	h.func_();
	return 0;
}
