#include "HttpResponse.hpp"


HttpResponse::HttpResponse(HttpRequest *httpRequest) {

	std::cout << MAGENTA;
	std::cout << "\tHttpResponse constructor called." << std::endl;
	std::cout << RESET << std::endl;

	_generateResponse(httpRequest);
}

HttpResponse::~HttpResponse() {

	std::cout << RED;
	std::cout << "\tHttpResponse destructor called." << std::endl;
	std::cout << RESET << std::endl;

}

std::string		HttpResponse::getResponse() {
	return _response;
}

void			HttpResponse::_generateResponse(HttpRequest *httpRequest) {

	std::cout << CYAN << "in _generateResponse().." << RESET << std::endl;
	std::cout << std::endl;

	std::string body = "Wake up, Neo...</br>The Matrix has you...</br></br>Follow the white rabbit.</br></br>Knock, knock, Neo.";

	_response = httpRequest->getHttpVersion() + " ";
	_response += "200 OK\r\n";
	_response += "Content-Type: text/html\r\n";
	_response += "\r\n";
	_response += "<html><body><h1>" + body + "</h1></body></html>\r\n\r\n";

	//std::cout << GREEN << "Response:" << RESET << std::endl;
	//std::cout << _response << std::endl;

}

