#include "HttpServer.hpp"

HttpServer::HttpServer(int domain, int service, int protocol, int port, u_long interface, int backlog) 
		: SimpleServer(domain, service, protocol, port, interface, backlog) {

	std::cout << MAGENTA << "\tHttpServer constructor called." << RESET << std::endl;
	_requestMethod = NONE;
	_typeGET = NORMAL;
	_path = "";
	_buffer = "";
	_new_socket = -1;

	// Launching the server.. `_accept()` >>> `_handle()` >>> `_respond()`
	run();
}

HttpServer::~HttpServer() {
	
	std::cout << RED << "\t[~] HttpServer destructor called." << RESET << std::endl;
}

/*
** Setters
*/
void	HttpServer::_setRequestMethod(std::string requestMethod) {

	if (requestMethod == "GET") {
		_requestMethod = GET;
	}
	else if (requestMethod == "POST") {
		_requestMethod = POST;
	}
	else if (requestMethod == "HEAD") {
		_requestMethod = HEAD;
	}
	else if (requestMethod == "PUT") {
		_requestMethod = PUT;
	}
	else if (requestMethod == "DELETE") {
		_requestMethod = DELETE;
	}
	else if (requestMethod == "CONNECT") {
		_requestMethod = CONNECT;
	}
	else if (requestMethod == "OPTIONS") {
		_requestMethod = OPTIONS;
	}
	else if (requestMethod == "TRACE") {
		_requestMethod = TRACE;
	}
	else if (requestMethod == "PATCH") {
		_requestMethod = PATCH;
	}
	else {
		_requestMethod = NONE;
	}
}

void	HttpServer::_setTypeGET(std::string typeGet) {

	if (typeGet == "/favicon.ico") {
		_typeGET = FAVICON;
	}
	else {
		_typeGET = NORMAL;
	}
}

void	HttpServer::_accept() {

	std::cout << std::endl;
	std::cout << MAGENTA << "in `_accept()`.." << RESET << std::endl;
	
	struct sockaddr_in	address = getSocket()->getAddress();
	int					addrlen = sizeof(address);

	_new_socket = accept(getSocket()->getSocketFD(), 
					(struct sockaddr *)&address, 
					(socklen_t *)&addrlen);

	if (_new_socket < 0) {
		std::cerr << RED << "\t[-] Error accepting connection.. accept() failed." << RESET << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << GREEN;
	std::cout << "[+] Connection accepted." << std::endl;
	std::cout << "IP: " << inet_ntoa(address.sin_addr) << std::endl;
	std::cout << BLUE;
	// !! emepheral port*
	std::cout << "Port (ephemeral) ntohs[" << ntohs(address.sin_port) << "], (client side)" << std::endl;
	std::cout << "Saving the request from the web-browser in `_buffer` with `recv()`." << std::endl;
	std::cout << RESET;

	char buff[BUF_SIZE] = {0};
	int bytesRead = recv(_new_socket, buff, BUF_SIZE, 0);

	if (bytesRead < 0) {
		std::cerr << RED << "\t[-] Error receiving data from the web-browser.. recv() failed." << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	else if (bytesRead == 0) {
		std::cout << RED << "\t[-] Connection closed by the web-browser." << RESET << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << GREEN << "Data received from the web-browser and added to the `_buffer`, bytesRead: [" << bytesRead << "]." << RESET << std::endl;
		_buffer.assign(buff, bytesRead);
	}
}
/*
** `ephemeral port` is a short-lived transport protocol port for Internet Protocol (IP) communications.
** It was set by the kernel on the client side, and was used by the server to send back responses.
** The client will use the same port number for the duration of the current connection.
*/

void	HttpServer::_handle() {

	std::cout << MAGENTA << "in `_handle()`.. Here is the `_buffer` (request from the web-browser):" << RESET << std::endl;
	std::cout << _buffer << std::endl;

	std::string type;

	std::istringstream iss(_buffer);
	iss >> type >> _path;

	_setRequestMethod(type);

	// If the request is a GET request, we need to check if it is a favicon request or a normal request
	_setTypeGET(_path);	

	std::cout << YELLOW;
	std::cout << "Request Method: [" << type << "], enum:[" << _requestMethod << "]" << std::endl;
	std::cout << "Path: [" << _path << "], enum:[" << _typeGET << "]" << std::endl;
	std::cout << RESET;

}

void	HttpServer::_respond() {

	std::cout << MAGENTA << "in `_respond()`.." << RESET << std::endl;

	std::string	response;
	response = "Wake up, Neo...\nThe Matrix has you...\nFollow the white rabbit.\nKnock, knock, Neo.";

	send(_new_socket, response.c_str(), response.length(), 0);

	std::cout << GREEN; 
	std::cout << "Responce message sent" << std::endl;
	std::cout << "Closing connection" << std::endl;
	std::cout << RESET;

	close(_new_socket);
}

void	HttpServer::run() {

	std::cout << GREEN << "\tHttpServer::run() called." << RESET << std::endl;

	while (1) {

		_accept();
		_handle();
		_respond();
	}
}
