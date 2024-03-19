#ifndef IOPERATION_HPP
# define IOPERATION_HPP

# include "./global_namespace.hpp"

namespace Global {
	class BasicSocket;
	class IOperation;
};

class Global::IOperation {
public:
	virtual ~IOperation() {}
	virtual int fd() = 0;
	virtual int op() = 0;
	virtual char* buf() = 0;
	virtual int len() = 0;
	virtual void aluLen(int n) = 0;
	virtual void aluBuf(int n) = 0;
	virtual void acceptHandler(int, BasicSocket&) = 0;
	virtual void socketHandler(int, int) = 0;
};

#endif
