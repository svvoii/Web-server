#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include "SimpleServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

#include <arpa/inet.h> // inet_ntoa
#include <sstream> // std::stringstream
#include <map> // std::map

#define BUF_SIZE 102400 // 100 KB to store the request from the browser

//class HttpRequest;

//typedef void (*HandlerFunction)(const std::string& request, std::string& response);

class HttpServer : public SimpleServer {
	private:

		std::string			_buffRequest; // To store the body of the request from the browser/client
		int					_new_socket; // To store the new socket created by accept()

		void				_accept();
		void				_handle();
		void				_respond();

	public:

		//HttpRequest			httpRequest(_buffRequest); // Class to parse the request from the browser
		HttpRequest			*httpRequest; // Class to parse the request from the browser

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
