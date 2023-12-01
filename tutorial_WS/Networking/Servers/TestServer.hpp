#ifndef TESTSERVER_HPP
#define TESTSERVER_HPP

#include "SimpleServer.hpp"
#include <arpa/inet.h> // inet_ntoa
#include <string.h> // printf

#define BUFSIZE 10240

// FOR TESTING PURPOSES ONLY
#define DOMAIN		AF_INET
#define SERVICE		SOCK_STREAM
#define PROTOCOL	0
#define PORT		80 // 80 might not work due to privileged ports access only for root users, try 8080
#define INTERFACE	INADDR_ANY
#define BACKLOG		10

class TestServer : public SimpleServer {
	private:
		char	_buffer[BUFSIZE];
		int		_new_socket;

		void	_accept();
		void	_handle();
		void	_respond();

	public:

		TestServer();
		~TestServer();

		void	run();

};

#endif
