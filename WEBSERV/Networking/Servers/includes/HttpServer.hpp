#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include "SimpleServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#include <arpa/inet.h> // inet_ntoa
#include <fcntl.h> // fcntl
#include <map> // std::map
#include <ctime> // std::time_t

//class HttpRequest;

//typedef void (*HandlerFunction)(const std::string& request, std::string& response);

class HttpServer : public SimpleServer {
	private:

		std::string			_requestBuffer; // To store the request from the browser
		std::string			_responseBuffer; // To store the response to the browser

		int					_serverSocketFd; // Server socket FD
		int					_newSocketFd; // To store the new socket FD created by accept(), client socket

		fd_set				_recv_fd_pool; // To store the socket FDs of the clients
		fd_set				_send_fd_pool; // To store the socket FDs of the clients
		int					_max_fd; // To store the max socket FD

		void				_initFdSets();
		void				_addToSet(int fd, fd_set *set);
		void				_removeFromSet(int fd, fd_set *set);
		void				_closeConnection(int fd);

		void				_accept();
		void				_handle(int fd);
		void				_respond(int fd);

		// alternative to handle() and respond()
		void				_handleRequestAndResponse();

	public:

		// Pointer to a Class to parse the request from the browser
		// `HttpRequest` object instantiated on the heap in the `_handle()` method
		//HttpRequest			*httpRequest;

		HttpServer(int domain, 
				int service, 
				int protocol, 
				int port, 
				u_long interface, 
				int backlog);
		~HttpServer();

		void		run();
		std::string	timeStamp();
		void		checkErrorAndExit(int returnValue, const std::string& msg);
};

#endif
