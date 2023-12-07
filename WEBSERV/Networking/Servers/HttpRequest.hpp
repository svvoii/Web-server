#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

//#include "HttpServer.hpp"
#include "../HeaderSockets.hpp"

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

class HttpRequest {
	private:

		enum requestMethod	_method; // To store the type of the request from the browser, GET, POST etc.
		std::string			_uriPath; // To store the requested path from the browser
		std::string			_httpVersion;
		std::map<std::string, std::string>	_headers;

		void parseLine(const std::string& line);
		void parseRequestLine();

	public:

		HttpRequest(const std::string& request);
		~HttpRequest();

		// Getters
		std::string	getMethod();
		std::string getUri();
		std::string getHttpVersion();
		std::map<std::string, std::string> getHeaders();

		// Clean parsing helpers
		std::string			trim(const std::string& str);
		enum requestMethod	isMethod(const std::string& str);

};

#endif
