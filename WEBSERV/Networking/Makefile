
NAME = webserv

CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98 -g

VPATH = Sockets \
		Servers

SRCS = main.cpp \
	SimpleSocket.cpp \
	BindingSocket.cpp \
	ListeningSocket.cpp \
	ConnectingSocket.cpp \
	SimpleServer.cpp \
	HttpServer.cpp \
	TestServer.cpp \
	HttpRequest.cpp \
	HttpResponse.cpp \

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
# cleaning the objects right after make
	make clean

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean
