#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include "SimpleServer.hpp"
#include <arpa/inet.h> // inet_ntoa
#include <sstream> // std::stringstream
#include <map> // std::map

#define BUF_SIZE 102400 // 100 KB to store the request from the browser

enum requestMethod {
	NONE,
	GET,
	POST,
	HEAD,
	PUT,
	DELETE,
	CONNECT,
	OPTIONS,
	TRACE,
	PATCH
};

enum getRequestType {
	NORMAL,
	FAVICON
};

typedef void (*HandlerFunction)(const std::string& request, std::string& response);

class HttpServer : public SimpleServer {
	private:

		enum requestMethod	_requestMethod; // To store the type of the request from the browser, GET, POST etc.
		enum getRequestType	_typeGET; // To distinguish between favicon and normal request from browser // To distinguish between favicon and normal GET request from browser
		std::string			_path; // To store the requested path from the browser
		std::string			_buffer; // To store the body of the request from the browser
		int					_new_socket; // To store the new socket created by accept()
		//std::map<std::string, HandlerFunction>	_routes; // To store the routes and their corresponding handler functions

		void				_setRequestMethod(std::string requestMethod);
		void				_setTypeGET(std::string typeGet);

		void				_accept();
		void				_handle();
		void				_respond();

	public:

		HttpServer(int domain, 
				int service, 
				int protocol, 
				int port, 
				u_long interface, 
				int backlog);
		~HttpServer();

		//void		addRoute(const std::string& route, HandlerFunction handler);

		void		run();
};

#endif
