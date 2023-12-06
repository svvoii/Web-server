#ifndef TESTSERVER_HPP
#define TESTSERVER_HPP

#include "SimpleServer.hpp"
#include <arpa/inet.h> // inet_ntoa
#include <string.h> // printf

/*
** The following class and all other variables are there 
** for testing purposes only.
** This class will be used to create a server side socket
** `listen()` for incoming connections and `accept()` them.
** 
** `run()` method contains the main loop of the server, 
** which will call `_accept()` to accept incoming connections
** `_handle()` to handle the request and
** `_respond()` to respond to the request
**
** see TestServer.cpp for more details on the implementation
*/

#define BUFSIZE 10240

// FOR TESTING PURPOSES ONLY
#define DOM			AF_INET
#define SERVICE		SOCK_STREAM
#define PROTOCOL	0
#define PORT		8080 // 80 might not work due to privileged ports access only for root users, try 8080
#define INTERFACE	INADDR_ANY
#define BACKLOG		10

enum requestType {
	NORM,
	FAVIC
};

class TestServer : public SimpleServer {
	private:
	 	//std::string	_requestType;
		//char	_buffer[BUFSIZE];
		enum requestType	_requestType;
		std::string			_buffer;
		int					_new_socket;

		void	_accept();
		void	_handle();
		void	_respond();

	public:

		TestServer();
		~TestServer();

		void	run();
};

#endif
