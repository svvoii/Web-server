#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include "SimpleServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#include <arpa/inet.h> // inet_ntoa
#include <map> // std::map


//class HttpRequest;

//typedef void (*HandlerFunction)(const std::string& request, std::string& response);

class HttpServer : public SimpleServer {
	private:

		//std::string			_buffRequest; // To store the body of the request from the browser/client
		int					_newSocketFd; // To store the new socket FD created by accept(), client socket

		void				_accept();
		void				_handleRequestAndResponse();
		//void				_handle();
		//void				_respond();

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
};

#endif
