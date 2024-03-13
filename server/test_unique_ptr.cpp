#include <memory>
#include <iostream>
#include <queue>
#include <cxxabi.h>

class IOperation {
public:
	virtual ~IOperation() {}
	virtual void handling(int) = 0;
};

template <typename Func>
class Operation : public IOperation {
public:
	Func handler;
	Operation(Func handler) : handler(handler) {}
	virtual void handling(int x) override {
		handler(x);
	}
};

// 맹글링된 이름을 디맹글링하는 함수
std::string demangle(const char* mangledName) {
    int status = -1; // demangling 상태를 초기화

    // __cxa_demangle 호출로 demangled 이름을 얻음
    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(mangledName, NULL, NULL, &status),
        std::free
    };

    return (status == 0) ? res.get() : mangledName; // demangling이 성공하면 demangled 이름 반환, 실패하면 원래 이름 반환
}

int main(void) {
	int x = 3;
	auto handler = [](int y) {
		std::cout << "hello: " << y << std::endl;
	};
	IOperation *op = new Operation<decltype(handler)>(handler);
	op->handling(x);
	return 0;
}
