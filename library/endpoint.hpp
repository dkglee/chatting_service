#ifndef ENDPOINT_HPP
# define ENDPOINT_HPP

typedef struct data_type {
	int domain;
	unsigned short port;
} data;

class EndpointImpl {
public:
	EndpointImpl() noexcept;
	~EndpointImpl() = default;
	EndpointImpl(int domain, unsigned short port_num) noexcept;
	int domain() const noexcept;
	unsigned short port() const noexcept;
private:
	EndpointImpl(const EndpointImpl& endpoint) = delete;
	EndpointImpl& operator=(const EndpointImpl& endpoint) = delete;
	data data_;
};

#endif
