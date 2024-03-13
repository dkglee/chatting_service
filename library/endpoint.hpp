#ifndef ENDPOINT_HPP
# define ENDPOINT_HPP

typedef struct data_type {
	int domain;
	unsigned short port;
} data;

class EndpointImpl {
public:
	EndpointImpl() noexcept;
	EndpointImpl(int domain, unsigned short port_num) noexcept
		: data_({domain, port_num})
	{
	}
	int domain() const noexcept {
		return data_.domain;
	}
	unsigned short port() const noexcept {
		return data_.port;
	}
private:
	data data_;
};

#endif
