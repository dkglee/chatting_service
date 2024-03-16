NAME = chat_server

CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -g
LDFLAGS = -pthread

RM = rm -rf

SRCS = ./server/server.cpp \
	./library/basic_acceptor.cpp \
	./library/basic_endpoint.cpp \
	./library/basic_socket.cpp \
	./library/endpoint.cpp \
	./library/executor.cpp \
	./library/io_context.cpp \
	./library/service.cpp \
	./library/socket.cpp \
	./library/tcp.cpp \
	./library/schedular.cpp \
	./library/round_robin_schedular.cpp

OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

${NAME}: ${OBJS}
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	$(RM) $(OBJS)

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY: all clean fclean re
