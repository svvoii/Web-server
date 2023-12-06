#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

//#include "HttpServer.hpp"

//#include <iostream>
#include <sstream>
#include <string>
#include <map>

class HttpServer;

enum requestMethod {
	NONE,
	GET,
	POST,
	HEAD,
	PUT,
	DELETE
};

enum getRequestType {
	NORMAL,
	FAVICON
};

class HttpRequest {
	private:

		enum requestMethod	_method; // To store the type of the request from the browser, GET, POST etc.
		//enum getRequestType	_typeGET; // To distinguish between favicon and normal request from browser // To distinguish between favicon and normal GET request from browser
		std::string			_uriPath; // To store the requested path from the browser
		std::string			_httpVersion;
		std::string			_body;
		std::map<std::string, std::string>	_headers;

		void parseRequestLine(const std::string& line);
		void parseHeader(const std::string& line);
		void parseBody(const std::string& body);

	public:

		std::string			buffer; // Will hold raw request from the browser

		HttpRequest();
		~HttpRequest();

		// Getters
		std::string	getMethod();
		std::string getUri();
		std::string getHttpVersion();
		std::string getBody();
		std::map<std::string, std::string> getHeaders();
};

#endif
