#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {

	std::stringstream ss(buffer);
	std::string line;
	std::string body;

	// Parse the request line
	std::getline(ss, line);
	parseRequestLine(line);

	// Parse the headers
	while (std::getline(ss, line) && line != "\r") {
		parseHeader(line);
	}

	// Parse the body
	while (std::getline(ss, line)) {
		body += line;
	}
	parseBody(body);
}

HttpRequest::~HttpRequest() {

}

// Getters
std::string	HttpRequest::getMethod() {
	
	if (_method == GET) {
		return "GET";
	}
	else if (_method == POST) {
		return "POST";
	}
	else if (_method == HEAD) {
		return "HEAD";
	}
	else if (_method == PUT) {
		return "PUT";
	}
	else if (_method == DELETE) {
		return "DELETE";
	}
	else {
		return "NONE";
	}
}

std::string	HttpRequest::getUri() {
	return _uriPath;
}

std::string	HttpRequest::getHttpVersion() {
	return _httpVersion;
}

std::string	HttpRequest::getBody() {
	return _body;
}

std::map<std::string, std::string>	HttpRequest::getHeaders() {
	return _headers;
}

// Private methods
void	HttpRequest::parseRequestLine(const std::string& line) {

	std::stringstream	ss(line);
	std::string			word;

	// Get the request method
	std::getline(ss, word, ' ');
	if (word == "GET") {
		_method = GET;
	}
	else if (word == "POST") {
		_method = POST;
	}
	else if (word == "HEAD") {
		_method = HEAD;
	}
	else if (word == "PUT") {
		_method = PUT;
	}
	else if (word == "DELETE") {
		_method = DELETE;
	}
	else {
		_method = NONE;
	}

	// Get the URI path
	std::getline(ss, word, ' ');
	_uriPath = word;

	// Get the HTTP version
	std::getline(ss, word, ' ');
	_httpVersion = word;
}

void	HttpRequest::parseHeader(const std::string& line) {

	std::stringstream	ss(line);
	std::string			key;
	std::string			value;

	std::getline(ss, key, ':');
	std::getline(ss, value, ':');
	_headers[key] = value;
}

void	HttpRequest::parseBody(const std::string& body) {

	_body = body;
}
